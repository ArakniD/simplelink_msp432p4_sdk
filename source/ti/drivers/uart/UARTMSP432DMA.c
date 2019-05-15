/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#include <ti/devices/DeviceFamily.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432DMA.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/timer/TimerMSP432.h>

/* driverlib header files */
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/dma.h>
#include <ti/devices/msp432p4xx/driverlib/pmap.h>
#include <ti/devices/msp432p4xx/driverlib/uart.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/timer32.h>

#include <SEGGER_SYSVIEW_uartdma.h>
#include <SEGGER_SYSVIEW.h>

/* Used to indicate more bytes required, use int to count */
#define UART_INT_RX EUSCI_A_UART_STARTBIT_INTERRUPT
#define UART_DBL_BUF_CH     3

/* Helper enum for Rx DMA scatter gatherer DMA task actions */
typedef enum {
    eTask1TriggerPingPongDMA = 0,
    eTask2ReloadIdleTimeout,
    eTask3RearmIdleTimer,
    eTask4ResetTaskCount,
    eTaskMax
} eRxDmaTasks;

#define MAX_DMA_TRANSFER_AMOUNT (1024)

#define PinConfigValue(config) (((config) >> 10) & 0x1F)
#define PinConfigModuleFunction(config) (((config) >> 8) & 0x3)
#define PinConfigPort(config) (((config) >> 4) & 0xF)
#define PinConfigPin(config) (1 << ((config) & 0x7))

/* UARTMSP432 functions */
void UARTMSP432DMA_close(UART_Handle handle);
int_fast16_t UARTMSP432DMA_control(UART_Handle handle, uint_fast16_t cmd, void *arg);
void UARTMSP432DMA_init(UART_Handle handle);
UART_Handle UARTMSP432DMA_open(UART_Handle handle, UART_Params *params);
int_fast32_t UARTMSP432DMA_read(UART_Handle handle, void *buffer, size_t size);
long UARTMSP432DMA_readCancel(UART_Handle handle);
int_fast32_t UARTMSP432DMA_readPolling(UART_Handle handle, void *buffer, size_t size);
int_fast32_t UARTMSP432DMA_write(UART_Handle handle, const void *buffer, size_t size);
void UARTMSP432DMA_transmitCancel(UART_Handle handle);
int_fast32_t UARTMSP432DMA_writePolling(UART_Handle handle, const void *buffer, size_t size);
static int32_t findBaudDividerIndex(UARTMSP432DMA_BaudrateConfig const *table, uint32_t tableSize, uint32_t baudrate,
                                    uint32_t clockFreq);
static void initHw(UARTMSP432DMA_Object *object, UARTMSP432DMA_HWAttrsV1 const *hwAttrs, uint32_t inputClkFreq);
static int perfChangeNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg);
static long UARTMSP432DMA_timeoutIntRx(Timer_Handle handle, uintptr_t arg);
inline uint32_t getDmaRemainingTXfers(UARTMSP432DMA_Object *object);
inline uint32_t getDmaRemainingRXfers(UARTMSP432DMA_Object *object);
inline void UARTMSP432DMA_ReadPingPongPeek(UART_Handle handle);
static long UARTMSP432DMA_hwiIntFxn(uintptr_t arg);
static long UARTMSP432DMA_hwiIntRx(uintptr_t arg);
static long UARTMSP432DMA_hwiIntRxDMA_done(uintptr_t arg);
static long UARTMSP432DMA_hwiIntTxDMA_done(uintptr_t arg);
static long UARTMSP432DMA_txDMA_done(uintptr_t arg);
static void mapPin(uint8_t port, uint8_t pin, uint8_t value);
static void UARTMSP432DMA_transmitDmaFill(UARTMSP432DMA_HWAttrsV1 const *hwAttrs, UARTMSP432DMA_Object *object);
static void InitDmaRxScatterGatherer(UARTMSP432DMA_Object *object, UARTMSP432DMA_HWAttrsV1 const *hwAttrs);

/* UART function table for UARTMP432 implementation */
const UART_FxnTable UARTMSP432DMA_fxnTable =
{   UARTMSP432DMA_close,
    UARTMSP432DMA_control,
    UARTMSP432DMA_init,
    UARTMSP432DMA_open,
    UARTMSP432DMA_read,
    UARTMSP432DMA_readAsync,
    UARTMSP432DMA_readPolling,
    UARTMSP432DMA_readCancel,
    UARTMSP432DMA_write,
    UARTMSP432DMA_writePolling,
    UARTMSP432DMA_transmitCancel
};

static const uint16_t stopBits[] =
{
    EUSCI_A_UART_ONE_STOP_BIT, /* UART_STOP_ONE */
    EUSCI_A_UART_TWO_STOP_BITS /* UART_STOP_TWO */
};

static const uint8_t parityTypes[] =
{
    EUSCI_A_UART_NO_PARITY, /* UART_PAR_NONE */
    EUSCI_A_UART_EVEN_PARITY, /* UART_PAR_EVEN */
    EUSCI_A_UART_ODD_PARITY, /* UART_PAR_ODD */
    0, /* UART_PAR_ZERO */
    1 /* UART_PAR_ONE */
};

/*
 *  ======== findBaudDividerIndex ========
 *  This function searchs a given array of different baudrate configurations to
 *  find the first compatible match given the desired buadrate and the currently
 *  configured clock input frequency.
 *
 *  It return the index in if the UARTMSP432DMA_BaudrateConfig table if a match was
 *  found, else it will return -1
 */
static int32_t findBaudDividerIndex(UARTMSP432DMA_BaudrateConfig const *table, uint32_t tableSize, uint32_t baudrate, uint32_t clockFreq)
{
    int32_t index;

    DebugP_assert((tableSize != 0) && (table != NULL));

    for (index = 0; index < tableSize; index++)
    {
        if ((table[index].outputBaudrate == baudrate) && (table[index].inputClockFreq == clockFreq))
        {
            return (index);
        }
    }
    return (-1);
}

/*
 *  ======== UARTMSP432DMA_hwiIntRxDMA_done ========
 *  This routine gets called when a Rx ping pong buffer has been finished
 *  and needs to be reset with valid data. The next buffer should already be
 *  switched in so it only resets the currently complete buffer.
 */
static long UARTMSP432DMA_hwiIntRxDMA_done(uintptr_t arg)
{
    UARTMSP432DMA_Object *object = ((UART_Handle) arg)->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = ((UART_Handle) arg)->hwAttrs;
    uintptr_t key;
    size_t overflow, transferSize ;
    eUDMAPingPong eBuffer;
    uint32_t dmaChannel;

    /* Pre-peek/done lock */
    key = HwiP_disable();

    /* Check which is the active DMA, primary or alternate, As this is a completion event the transfer
     * size is in the opposite to the active DMA channel
     */
    if (*object->rx_dmaAltSetBitBand) {
        dmaChannel = object->rx_transfer.dmaChannel | UDMA_PRI_SELECT;
        eBuffer = eUDMAPingPongPrimary;
    } else {
        dmaChannel = object->rx_transfer.dmaChannel | UDMA_ALT_SELECT;
        eBuffer = eUDMAPingPongAlternate;
    }
    transferSize = object->rx_transfer.specs[eBuffer].size;
    object->rx_transfer.specs[eBuffer].size = 0;

    MAP_DMA_clearInterruptFlag(hwAttrs->bfrDMAChannelIndex & 0x0F);

    /* Call the put_dma_size routine which will advance the head and enable the full size to be read,
     * also deducting the current readPutPeek variable as other calls to PEEK have incremented this value
     * and WE do not want to double count bytes, so deduct this figure, also guard against a zero size
     * by checking if the readPutPeek is less than the transfer size */
    if ( transferSize ) {
        overflow = RingBuf_free_put_contiguous(&object->readBuffer, transferSize );
    } else {
        overflow = 0;
    }

    /* Calculate the next ring buffer transfer size given the unallocated ring buffer available, accounting
     * for two buffers in size to allocate, as allocating a single 256 byte buffer then 16 byte is not wise.
     * Taking the 256+16 / 2 approach allows the size to incrementally decrease as buffer space runs out until
     * ring min is met. At that point activate the all stop before we overrun the buffer and cause all sorts of
     * problems.
     */
    transferSize = object->ringBlkSize;
    while (transferSize >= (RingBuf_getUnallocated(&object->readBuffer) >> 1 ) && transferSize >= object->ringBlkSizeMin )
    {
       transferSize >>= 1;
    }

    /* Check if we have an overflow OR a buffer full condition. In which case assert the RTS pin if it exists and then
     * mask off the DMA channel so no more requests come in to cause an overflow if we're a buffer full situation
     */
    if ( overflow != 0
     ||  transferSize <= object->ringBlkSize ) {
        /* If we're not overflowed, but we are full, we need to toggle the RTS pin if its configured,
         * to prevent more bytes from overflowing the buffer.. but it could be too late already, technically
         * the rxBuf of the eUSCI will still contain the last byte read
         */
        if (object->rtsPinBitBand != NULL ) {
           *object->rtsPinBitBand = 0x0001;
        }

        if (transferSize < object->ringBlkSizeMin) {
            /* Mask down the rxChannel because the buffer is full and we can no longer receive more bytes. As we are asserting the
            * RTS pin, the next byte should be buffered in UART_A2->rxBuf until the the channel is re-enabled, this will prevent an
            * overflow of the RX buffer
            */
            DMA_Control->REQMASKSET = 1 << (hwAttrs->rxDMAChannelIndex & 0x0F);
        }
    }

    /* Toggle the ping-pong buffer so that the secondary buffer has a valid pointer and transfer size for the auto swap-over
     * on buffer filling. The ring buffer is divided up into read sprints of a much smaller size than the total ring buffer.
     * This should allow the DMA interrupt routines to catch a potential buffer overflow before it occurs, which will pause
     * the current transaction routines, allowing the caller time to clear down the buffer before resuming, and releasing the
     * RTS signal to the remote host.
     */
    object->rx_transfer.specs[eBuffer].size =
            RingBuf_alloc_put_contiguous(&object->readBuffer, transferSize, (unsigned char **)&object->rx_transfer.specs[eBuffer].dst );
    MAP_DMA_setChannelControl(dmaChannel, object->rx_transfer.ctlOptions);
    MAP_DMA_setChannelTransfer(dmaChannel, UDMA_MODE_PINGPONG, object->rx_transfer.specs[eBuffer].src, object->rx_transfer.specs[eBuffer].dst, object->rx_transfer.specs[eBuffer].size);

    /*
     * The Ping-Pong buffer is always setup for continual transfers and can overrun if no flow control is implemented within the hardware
     * layer. This is why the above code asserts the RTS flag and always maintains at least one ping-ping block from end-of-buffer. This
     * may be a little wasteful but offers great protection against buffer overrun and allows ample time for servicing of this interrupt
     * while more data could be arriving at warp like speeds.
     */

    /* Check if we have an actual overflow condition here. If we do then we must alert the caller that this has occurred
     * so the necessary actions can be taken to restart the communications channel
     */
    if (overflow != 0) {
        /* Reduce the size of the readPrevious because the overflow has caused the tail
         * pointer to increase, and thus free the buffer. This prevents a double "free"
         * action on the ping-pong buffer, which would consume bytes where we /may/ not
         * want to. We've certainly lost bytes already so.... its not a GOOD situation to
         * be in..
         */
        if (object->readAsyncPrev <= overflow) {
            object->readAsyncPrev = 0;
        } else {
            object->readAsyncPrev-= overflow;
        }

        /* Post-peek/done unlock */
        HwiP_restore(key);

        /* Buffer overflow condition.. the callback function needs to be called */
        hwAttrs->errorFxn((UART_Handle) arg, EUSCI_A_UART_OVERRUN_ERROR);
    } else {
        /* Post-peek/done unlock */
        HwiP_restore(key);
    }

    /*
     * unblock the reader if they're waiting for data, has no effect if there are no callers waiting.
     */
    return SemaphoreP_post(object->readSem);
}

/*
 * ======== UARTMSP432DMA_ReadPingPongPeek ========
 *
 * This inline routine peaks into the ping-pong buffer DMA structures and advanced the stored bytes
 * based upon how many DMA transfers remaining for either the Alternate or Primary structure
 *
 * Note: Always call this function within a "key = Hwip_disable(); .... ; HwiP_restore(key);"
 * locking context. As the variables accessed are not locked within nested routines for speed
 * reasons.
 *
 */
inline void UARTMSP432DMA_ReadPingPongPeek(UART_Handle handle)
{
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
    size_t overflow, transfered;
    uint32_t ctrlBufr;
    eUDMAPingPong eBuffer;

    /* Check if either the Alternate or Primary DMA structure is being executed
     * by the DMA controller. If a structure is stopped, then the remaining count
     * is zero, otherwise its ((control & mask) >> 4 ) + 1
     *
     * Buffer the control structure value in case a race-condition occurs and the
     * value changes by one during calculation, as this routine cannot block DMA
     * actions for happening in parallel
     *
     * And yes this makes a difference... 750k baud confirms this occurs
     */
    if (*object->rx_dmaAltSetBitBand) {
        ctrlBufr = *object->rx_dmaCtrl[eUDMAPingPongAlternate];
        eBuffer = eUDMAPingPongAlternate;
    } else {
        ctrlBufr = *object->rx_dmaCtrl[eUDMAPingPongPrimary];
        eBuffer = eUDMAPingPongPrimary;
    }
    transfered = object->rx_transfer.specs[eBuffer].size - (ctrlBufr & 0x07 ? ((ctrlBufr & UDMA_CHCTL_XFERSIZE_M) >> 4) + 1 : 0);
    object->rx_transfer.specs[eBuffer].size -= transfered;

    /* Call the put_dma_size routine which will advance the head and enable the full size to be read,
     * also deducting the current readPutPeek variable so WE do not double up on advancement */
    if ( transfered ) {
        overflow = RingBuf_free_put_contiguous(&object->readBuffer, transfered );
    } else {
        overflow = 0;
    }

    /* Check if we have an overflow OR a buffer full condition. In which case assert the RTS pin if it exists and then
     * mask off the DMA channel so no more requests come in to cause an overflow if we're a buffer full situation.
     *
     * + DMA Requests are blocked once free space is <= ringBlkSizeMin
     * + RTS is asserted once free space is <= ringBlkSize
     */
    if ( overflow != 0
     ||  object->readBuffer.count >= ( object->readBuffer.length - object->ringBlkSize ) ) {
        /* If we're not overflowed, but we are full, we need to toggle the RTS pin if its configured,
         * to prevent more bytes from overflowing the buffer.. but it could be too late already, technically
         * the rxBuf of the eUSCI will still contain the last byte read
         */
        if (object->rtsPinBitBand != NULL) {
           *object->rtsPinBitBand = 0x0001;
        }

        if (object->readBuffer.count >= ( object->readBuffer.length - object->ringBlkSizeMin )) {
            /* Mask down the rxChannel because the buffer is full and we can no longer receive more bytes. As we are asserting the
             * RTS pin, the next byte should be buffered in UART_A2->rxBuf until the the channel is re-enabled, this will prevent an
             * overflow of the RX buffer
             */
            DMA_Control->REQMASKSET = 1 << (hwAttrs->rxDMAChannelIndex & 0x0F);
        }
    }

    /* Check if we have an actual overflow condition here. If we do then we must alert the caller that this has occurred
     * so the necessary actions can be taken to restart the communications channel
     */
    if (overflow != 0) {
        /* Reduce the size of the readPrevious because the overflow has caused the tail
         * pointer to increase, and thus free the buffer. This prevents a double "free"
         * action on the ping-pong buffer, which would consume bytes where we /may/ not
         * want to. We've certainly lost bytes already so.... its not a GOOD situation to
         * be in..
         */
        if (object->readAsyncPrev <= overflow) {
            object->readAsyncPrev = 0;
        } else {
            object->readAsyncPrev -= overflow;
        }

        /* Buffer overflow condition.. the callback function needs to be called */
        hwAttrs->errorFxn(handle, EUSCI_A_UART_OVERRUN_ERROR);
    }
}

/*
 * ======== UARTMSP432DMA_hwiIntRx ========
 * This function hook is the DMA Done call for the RX Scatter Gatherer call. If
 * this executes then something bad has happened to make the channel actually
 * complete, Its never meant to complete. It should loop indefinately without
 * return, thus this is a system error. The receive channel should be reset
 * back to its original tasking from the InitHW call. Log a message for diagnostics
 */
static long UARTMSP432DMA_hwiIntRx(uintptr_t arg)
{
    UARTMSP432DMA_Object *object = ((UART_Handle) arg)->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = ((UART_Handle) arg)->hwAttrs;
    uintptr_t key;

    /* Always manually clear down the DMA channel otherwise it wont clear */
    MAP_DMA_clearInterruptFlag(hwAttrs->rxDMAChannelIndex & 0x0F);

    key = HwiP_disable();

    /* Setup the channel again... this should never happen */
    InitDmaRxScatterGatherer(object, hwAttrs);

    HwiP_restore(key);

    return 0;
}

/*
 *  ======== UARTMSP432DMA_timeoutIntRx ========
 *  Stop any active read operations, clean up buffers and tally remaining
 *  counts so any unblocked read operation gets the correct buffers
 *
 *  Also setup DMA to transfer back in the scratchpad area
 *
 */

static long UARTMSP432DMA_timeoutIntRx(Timer_Handle handle, uintptr_t arg)
{
    UARTMSP432DMA_Object *object = ((UART_Handle) arg)->object;
    uintptr_t key;

    /* Pre-peek lock */
    key = HwiP_disable();

    /* Perform a read ping-pong peek so the true size of the receive buffer
     * is known before the semaphore is posted, unblocking the reader
     */
    UARTMSP432DMA_ReadPingPongPeek((UART_Handle)arg);

    if (object->readBuffer.count >= object->rx_readMin
    && object->rx_readMin) {
        object->state.bufTimeout = false;
    } else {
        object->state.bufTimeout = true;
    }

    /* Post-peek unlock */
    HwiP_restore(key);

    return SemaphoreP_post(object->readSem);
}

/*
 *  ======== UARTMSP432DMA_readAsync ========
 *
 *  This function will return once a single character has been read if the known size is zero,
 *  otherwise it will attempt to set timeouts or use interrupt counts to get the right return size.
 *
 *  The buffer will be passed back to the reader for consumtion, it should not be held beyond the
 *  very next read operation. It will take anoher read to remove the buffer from the held list and
 *  continue on.
 *
 */
int_fast32_t UARTMSP432DMA_readAsync(UART_Handle handle, size_t request, int release, volatile uint8_t **buffer, size_t *size)
{
    uintptr_t key;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    SEGGER_SYSVIEW_RecordU32x3(SYSVIEW_EID_uartReadAsyncEnter, (uintptr_t)handle, request, release);

    /*
     * It is possible for the object->timeoutClk and the callback function to
     * have posted the object->readSem Semaphore from the previous UART_read
     * call (if the code below didn't get to stop the clock object in time).
     * To clear this, we simply do a NO_WAIT pend on (binary) object->readSem
     * so that it resets the Semaphore count.
     */
    SemaphoreP_pend(object->readSem, SemaphoreP_NO_WAIT);
    object->state.bufTimeout = false;

    /* Pre-peek lock */
    key = HwiP_disable();

    /* Perform a read ping-pong peek so the true size of the receive buffer
     * is known before the semaphore is posted, unblocking the reader
     */
    UARTMSP432DMA_ReadPingPongPeek(handle);

    /* Free the previous buffer by advancing the ring tail forward
     * by the number of bytes specified in "release". One cannot
     * release more bytes than has been read previously.
     * Note: to empty the existing buffer the caller should use the
     * control functions to flush buffers
     */
    if (object->readAsyncPrev && release)
    {
        /* Allows an anonymouse, release everything thats been returned before
         * so the client doesn't always have to track the last read amount
         */
        if (release == -1) {
            release = object->readAsyncPrev;
        } else  if (release > object->readAsyncPrev) {
        /* Limit release to the size of readPrevious
         * as we cannot possibly release more bytes
         * than we actually how outstanding */
            release = object->readAsyncPrev;
        }

        /* Further limit the release amount to the actual size of the current ring buffer
         * as the limits are not enforced within the nested functions
         */
        if (release <= object->readBuffer.count) {
            RingBuf_get_dma(&object->readBuffer, release);
        } else {
            RingBuf_get_dma(&object->readBuffer, object->readBuffer.count);
        }
    }

    /* Check if we've met the RTS flow on control state here and not within the read-wait loop
     * because here is the only place the receive buffer is actually cleared down, because the
     * return from this call passes the receive buffer back to the caller directly, saving on
     * memory copy calls. So really here is the only place within the function that could satify
     * an RTS on condition. And we're already under a locking scenario, and just checked the ping
     * pong peek, so we're pretty sure that we have a good understanding of how much data is in
     * the receive buffer waiting to be read.
     */
    if (object->readBuffer.count < (object->readBuffer.length - object->ringBlkSizeMin))
    {
        /* Check if we can also un-mask the receive channel in case we were either disabled or
         * previously masked due to overflow condition met
         */
        if (DMA_Control->REQMASKSET & (1 << (hwAttrs->rxDMAChannelIndex & 0x0F)))
        {
            DMA_Control->REQMASKCLR = (1 << (hwAttrs->rxDMAChannelIndex & 0x0F));

            /* We have just unmasked the DMA channel, it could now have a pending transfer
             * which has not been auctioned because the channel was masked before the interrupt
             * Occurred. Manually trigger the DMA channel to spring life back into the receive
             * DMA transaction machine
             */
            if (MAP_UART_getInterruptStatus(hwAttrs->baseAddr, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
            {
                DMA_Control->SWREQ = (1 << (hwAttrs->rxDMAChannelIndex & 0x0F));
            }
        }

        /* Check to see if we also need to lower the RTS flag, always after we manually trigger the
         * DMA channel in case the RTS clearing causes more bytes to come in, in which case we could
         * receive a byte in between calling software request and returning from lowering the flag
         */
        if (object->readBuffer.count < (object->readBuffer.length - object->ringBlkSize) && object->rtsPinBitBand != NULL) {
            *object->rtsPinBitBand = 0x0000;
        }
    }

    /* Post-peek unlock */
    HwiP_restore(key);

    /* Restrict the request to half the ring size otherwise
     * the loop functions below may loop beyond the ability of the
     * ping-pong buffer to hold */
    if (request >= (object->readBuffer.length - object->ringBlkSizeMin)) {
        request = object->readBuffer.length - object->ringBlkSizeMin;
    }

    if (request == 0) {
        /* If we have bytes in the input ping-pong already, then
         * we do not wait, we return everything we have so it can
         * be inspected immediately, because we've called with zero
         * requested..
         */
        if (!object->readBuffer.count) {
            object->rx_readMin = 1;
            /* Enable the timer because we've been asked to read a minimum
             * of zero bytes, thus the timer wont start automatically and
             * we will never return. So start it now incase it doesn't start
             * later on
             */
            Timer_start(object->timeoutTmr);

            /* Wait on the semaphore for more bytes before returning.
             * This will be either triggered from the above timer or
             * if enough bytes ( >= object->readMin ) have been read
             * in the background from a combination of DMA interrupt
             * and timeout. Which ever occurs first.
             */
            SemaphoreP_pend(object->readSem, SemaphoreP_WAIT_FOREVER);
        }
    } else if (request > object->readBuffer.count) {
        /* if we have requested MORE bytes than there are in the current
         * ping-pong buffer, then we MUST wait (or timeout) for more data
         * as we've been told to explicitly try for more bytes.. Its likely
         * the caller needs that many more bytes to determine the next or
         * current packet header. This saves on excess call / return when
         * the amount of more data required is known
         */
        object->rx_readMin = request;
        //Timer_start(object->timeoutTmr);
        /*
         * Loop until we have a ring buffer size >= requested, or we experienced a buffer timeout.
         * Note: the timer is setup by the Rx DMA Scatter Gatherer channel to be ~ 4x the Rx frame
         * length in clock cycles. Thus a continuous stream of bytes should be un-interrupted
         */
        while (object->readBuffer.count < request && !object->state.bufTimeout) {
            /* Start the timer if bytes have been requested and we have ANY bytes in the queue as this
             * is a AsyncRead call, and when requested we timeout from the reception of the first (or subsequent bytes)
             */
            if (object->readBuffer.count) {
                /* Start the timer, or again, as we've either expired a read sequence or this
                 * is our first iteration through and we have bytes in our buffer, so we are going
                 * to return the the caller with the buffer on a timeout, or directly after a byte
                 * stream is received
                 * Note: This timer count will be overwritten by the Rx DMA Scatter Gatherer continuously
                 * while receiving characters from the UART, meaning a stream of data will push out this
                 * timeout until a DMA buffer toggles ping-pong, so quite large potentially?
                 * ~ > (ring_buffer_size / 2). But really, if the timeout value has been configured as
                 * 10ms, then 1,150 bytes will be received in this time, which is likely greater than
                 * the size of the ring-buffer anyway... so we will get notification
                 */
                Timer_start(object->timeoutTmr);
            }

            /* Wait for the DMA or timer callback to strike */
            SemaphoreP_pend(object->readSem, SemaphoreP_WAIT_FOREVER);
        }
    } else {
        /* The number of bytes requested is equal to the size of the ping-pong buffer
         * or less than the size required, so we can just fall through here and return
         * what we have in the buffer..
         * Note: we already return the MAXIMUM amount in the buffer.. In a continuous buffer
         * that is.. Non-continuous buffers are handled over multiple calls to the AsyncRead
         * function.
         */
        object->rx_readMin = 0;
    }

    /* Get a continuous buffer from the ping-pong, this does not decrease the buffer count until
     * the read is consumed by a further calls to AsyncRead. The buffer returned is a direct
     * pointer into the read ringbuffer as is considered volatile beyond the scope of the return
     * call. Only another read operation, with the parameter "release" size as the last read-size
     * will advance the ring buffer forward.
     * Note: Not releasing the buffer will allow/cause the ping-pong to overwrite the ring buffer
     * contents if not released.
     */
    *size = object->readAsyncPrev = RingBuf_peek_contiguous(&object->readBuffer, (uint8_t**)buffer);

    /*
     * Return back the total size of the ring-buffer so the caller knows how much data is pending to be
     * parsed. This size differs from the *size and readPrevious because because the returned buffer is
     * a continuous block of memory of *size. This saved on memory copy actions between driverlib and
     * caller, and any deeper layers behind the caller, but does allow the possibility of a buffer overflow
     * corrupting the buffer returned above.
     * Note: Take caution and consume the returned buffer immediately! Be sure to call ReadAsync with the
     * release parameter equal the the size of buffer returned previously, otherwise the ring-buffer will
     * fill up until it overflows, before setting the RTS pin.
     * Note: The DMA hardware interrupt routine may set the RTS pin when the ring-buffer is filling up.
     */
    SEGGER_SYSVIEW_RecordU32x3(SYSVIEW_EID_uartReadAsyncExit, (uintptr_t)handle, object->readAsyncPrev, object->readBuffer.count );
    return object->readBuffer.count;
}

/*
 *  ======== UARTMSP432DMA_read ========
 */
int_fast32_t UARTMSP432DMA_read(UART_Handle handle, void *buffer, size_t size)
{
    uintptr_t key;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
    uint32_t readCount = 0, bfr_size;
    uint8_t *readBuffer = (uint8_t*)buffer;
    uint8_t *ringBuffer;

    SEGGER_SYSVIEW_RecordU32x2(SYSVIEW_EID_uartReadEnter, (uintptr_t)handle, size);

    /*
    * It is possible for the object->timeoutClk and the callback function to
    * have posted the object->readSem Semaphore from the previous UART_read
    * call (if the code below didn't get to stop the clock object in time).
    * To clear this, we simply do a NO_WAIT pend on (binary) object->readSem
    * so that it resets the Semaphore count.
    */
    SemaphoreP_pend(object->readSem, SemaphoreP_NO_WAIT);
    object->state.bufTimeout = false;

    /* Pre-peek lock */
    key = HwiP_disable();

    /* Check if we have a lingering readPrevious to take care of, as we're performing
     * a synchronous read we need to disband with the Async methods and clear down
     * any flags hanging over
     */
    if (object->readAsyncPrev)
    {
        /* Further limit the release amount to the actual size of the current ring buffer
        * as the limits are not enforced within the nested functions
        */
        if (object->readAsyncPrev <= object->readBuffer.count) {
            RingBuf_get_dma(&object->readBuffer, object->readAsyncPrev);
        } else {
            RingBuf_get_dma(&object->readBuffer, object->readBuffer.count);
        }
        object->readAsyncPrev = 0;

        /* No need to check for a RTS release condition here because the while
         * loop below will capture that check if it reads a single character,
         * which it will if there is enough data to actually cause a RTS assertion
         */
    }

    /* Perform a read ping-pong peek so the true size of the receive buffer
     * is known before the semaphore is posted, unblocking the reader
     */
    UARTMSP432DMA_ReadPingPongPeek(handle);

    /* Post-peek unlock */
    HwiP_restore(key);

    /* Handle special "Flush" read buffer. Null buffer and zero size.
     * Otherwise a NULL buffer is valid, and will act like a read but
     * throw away the data
     */
    if (buffer == NULL && size == 0 && object->readBuffer.count)
    {
        size = object->readBuffer.count;
        RingBuf_get_dma(&object->readBuffer, object->readBuffer.count);

        /* Shoot back the size so the client knows how many bytes were just purged */
        return size;
    }

    /* if we have requested MORE bytes than there are in the current
     * ping-pong buffer, then we MUST wait (or timeout) for more data
     * as we've been told to explicitly try for more bytes.. Its likely
     * the caller needs that many more bytes to determine the next or
     * current packet header. This saves on excess call / return when
     * the amount of more data required is known
     */
    object->rx_readMin = 1;

    /* The read-wait-timeout loop will execute so long as there are bytes
     * in the receive queue, we're not timed-out and we have more bytes
     * to read. So even if we're timed out, we'll still loop around while
     * there are more bytes to shift into the callers buffer.. because each
     * read cycle could get a continuous buffer which wraps the ring buffer
     * leaving more bytes to be returned on the very next call.
     */
    while ( size && (object->readBuffer.count || !object->state.bufTimeout) )
    {
        /* Read out as many bytes as possible on entry
         * and then wait on the semaphore if more data
         * is required to be read. Reading and performing a
         * memcpy as data arrives makes processing more efficient
         * and prevents the read buffer from overflowing
         */
        if (object->readBuffer.count) {
            /* Get a continuous buffer from the ring and cap it at the remaining size */
            if ( (bfr_size = RingBuf_peek_contiguous(&object->readBuffer, &ringBuffer)) > size ) {
                bfr_size = size;
            }

            /* Handle effective read to NULL. Don't actually copy unless we have a buffer, otherwise
             * act like we do... just copy nothing
             */
            if (readBuffer != NULL) {
                /* Copy across the read buffer into the callers supplied buffer */
                memcpy(readBuffer, ringBuffer, bfr_size);
                readBuffer += bfr_size;
            }
            /* Skip the number of bytes just copied to the callers buffer. Call the DMA
             * version of the ring buffer skip, because we are not in control of where
             * the DMA will write to next, so we cannot reset the head and tail back to
             * zero because the Ping-Ping receive buffer pointers have already been defined
             * and may execute while this is executing. One cannot pull the rug
             * from underneath the DMA controller, otherwise it will byte you... ha...
             */
            RingBuf_get_dma(&object->readBuffer, bfr_size);

            /* Copy out as many bytes as we can into our buffer then increment
             * the pointer and decrement the size */
            readCount += bfr_size;
            size -= bfr_size;

            /* Asses the size of the read buffer, and if its dropped below the high water mark
             * then check if we need to trigger a DMA cycle and unmask the DMA channel
             */
            if (object->readBuffer.count < (object->readBuffer.length - object->ringBlkSize))
            {
                /* Check to see if we also need to lower the RTS flag, always after we manually trigger the
                 * DMA channel in case the RTS clearing causes more bytes to come in, in which case we could
                 * receive a byte in between calling software request and returning from lowering the flag
                 */
                if (object->rtsPinBitBand != NULL) {
                    *object->rtsPinBitBand = 0x0000;
                }

                /* Check if we can also un-mask the receive channel in case we were either disabled or
                 * previously masked due to overflow condition met
                 */
                if (DMA_Control->REQMASKSET & (1 << (hwAttrs->rxDMAChannelIndex & 0x0F)))
                {
                    /* Pre-flow-on lock */
                    key = HwiP_disable();

                    DMA_Control->REQMASKCLR = 1 << (hwAttrs->rxDMAChannelIndex & 0x0F);

                    /* We have just unmasked the DMA channel, it could now have a pending transfer
                     * which has not been auctioned because the channel was masked before the interrupt
                     * Occurred. Manually trigger the DMA channel to spring life back into the receive
                     * DMA transaction machine
                     */
                    if (MAP_UART_getInterruptStatus(hwAttrs->baseAddr, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
                    {
                        DMA_Control->SWREQ = (1 << (hwAttrs->rxDMAChannelIndex & 0x0F));
                    }

                    /* Post-flow-on unlock */
                    HwiP_restore(key);
                }
            }
        } else {
            /* Start the timer, or again, as we've either expired a read sequence or this
             * is our first iteration through and we have bytes in our buffer, so we are going
             * to return the the caller with the buffer on a timeout, or directly after a byte
             * stream is received
             * Note: This timer count will be overwritten by the Rx DMA Scatter Gatherer continuously
             * while receiving characters from the UART, meaning a stream of data will push out this
             * timeout until a DMA buffer toggles ping-pong, so quite large potentially?
             * ~ > (ring_buffer_size / 2). But really, if the timeout value has been configured as
             * 10ms, then 1,150 bytes will be received in this time, which is likely greater than
             * the size of the ring-buffer anyway... so we will get notification
             */
            Timer_start(object->timeoutTmr);
            /* Wait for the interrupt or DMA functions to post to the
             * semaphore, meaning data available to read, or a timeout occurred
             */
            SemaphoreP_pend(object->readSem, SemaphoreP_WAIT_FOREVER);
        }
    }

    SEGGER_SYSVIEW_RecordU32x3(SYSVIEW_EID_uartReadExit, (uintptr_t)handle, readCount, object->readBuffer.count);

    return readCount;
}

/*
 *  ======== UARTMSP432DMA_readCancel ========
 *  If there is no valid ReadBuf then we have
 *  no active listener... so just return quickly
 */
long UARTMSP432DMA_readCancel(UART_Handle handle)
{
    return UARTMSP432DMA_timeoutIntRx(NULL, (uintptr_t)handle);
}

/*
 *  ======== UARTMSP432DMA_transmitDmaFill ========
 *  This function configures the DMA transactions in straight transfer mode. An attempt at using ping-pong mode
 *  has been made and it needs further debugging and development to make work correctly. The flags tx_toggle and tx_depth
 *  have been left for futher development. For whatever reason I could not get the transactions to continue beyond the
 *  first one, ever. Even if I setup a new transaction after the first one, it would just hang and never complete. I would
 *  also fail to get completion interrupts or fail to keep sending when the alt and primary were different sizes
 *
 *  High baud rate writes are choppy without ping-pong as there are many gaps during re-initialising the output buffer(s)
 */
static void UARTMSP432DMA_transmitDmaFill(UARTMSP432DMA_HWAttrsV1 const *hwAttrs, UARTMSP432DMA_Object *object)
{
   uint8_t enableNeeded = 0;
#if defined(UART_TX_PINGPONG)
#define MAX_UART_TX_DEPTH 2
#else
#define MAX_UART_TX_DEPTH 1
#endif

   if (object->tx_depth == 0)
    {
      //MAP_DMA_disableChannel(object->tx_transfer.dmaChannel & 0x0F);
       /* Clear the register before setting attributes */
      MAP_DMA_disableChannelAttribute(object->tx_transfer.dmaChannel,
          UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
          UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);
      enableNeeded = 1;
      object->tx_toggle = 0;
   }

#if !defined(UART_TX_PINGPONG)
   if (object->tx_depth == 0)
#endif
   {
       /* Fill the DMA Buffer, this is either skip, one or two passes */
       while ( object->tx_depth < (MAX_UART_TX_DEPTH) && object->writeBuffer.reserved < object->writeBuffer.count) {
           /* Depth moves forward when more transactions are used while
            * toggle stays put until a free operation occurs
         */
           eUDMAPingPong tx_index = (eUDMAPingPong)((object->tx_toggle + object->tx_depth++ ) % (uint8_t)eUDMAPingPongMAX) ;

           object->tx_transfer.specs[tx_index].size =
               RingBuf_alloc_get_contiguous(&object->writeBuffer, object->ringBlkSize,
                                           (uint8_t **)&object->tx_transfer.specs[tx_index].src);

           /* Sanity check here, should always pass */
           if (object->tx_transfer.specs[tx_index].size != 0) {
               /* Calculate the Primary or Alternate channel */
               uint32_t dmaChannel = object->tx_transfer.dmaChannel | (tx_index << 3);
               MAP_DMA_setChannelControl(dmaChannel, object->tx_transfer.ctlOptions);

               /* If there is the potential for more buffers to be filled after this transaction then
                * enable ping-pong, otherwise we're a basic transaction type
                */
#if defined(UART_TX_PINGPONG)
               if (object->writeBuffer.reserved < object->writeBuffer.count ) {
                   MAP_DMA_setChannelTransfer(dmaChannel, UDMA_MODE_PINGPONG,
                      object->tx_transfer.specs[tx_index].src, object->tx_transfer.specs[tx_index].dst,
                      object->tx_transfer.specs[tx_index].size);
            } else {
                   MAP_DMA_setChannelTransfer(dmaChannel, UDMA_MODE_BASIC,
                      object->tx_transfer.specs[tx_index].src, object->tx_transfer.specs[tx_index].dst,
                      object->tx_transfer.specs[tx_index].size);
            }

               /* Check if the active buffer is set to UDMA_MODE_BASIC, and if so set a bit to make it PING PONG */
               if (object->tx_depth == 2 && !enableNeeded
               && (eUDMAPingPong)*object->tx_dmaAltSetBitBand == object->tx_toggle
               && (*object->tx_dmaCtrl[object->tx_toggle] & 0x07 ) != 0 ) {
                  *object->tx_dmaPPbit[object->tx_toggle] = 1;
               }
#else
               MAP_DMA_setChannelTransfer(dmaChannel, UDMA_MODE_BASIC,
                object->tx_transfer.specs[tx_index].src, object->tx_transfer.specs[tx_index].dst,
                object->tx_transfer.specs[tx_index].size);
#endif
            }
        }

       if (enableNeeded) {
           MAP_DMA_clearInterruptFlag(object->tx_transfer.dmaChannel & 0x0F);

           /* Assign and enable DMA channel */
           MAP_DMA_assignChannel(object->tx_transfer.dmaChannel);
           MAP_DMA_enableChannel(object->tx_transfer.dmaChannel & 0x0F);
       }
   }
}

static long UARTMSP432DMA_hwiIntTxDMA_done(uintptr_t arg)
{
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = ((UART_Handle) arg)->hwAttrs;

    MAP_DMA_clearInterruptFlag(hwAttrs->txDMAChannelIndex & 0x0F);

    return UARTMSP432DMA_txDMA_done(arg);
}

/*
 *  ======== UARTMSP432DMA_hwiIntTxDMA_done ========
 *  write = (toggle + depth++) % 2
 *  free = toggle++; depth--;
 */
static long UARTMSP432DMA_txDMA_done(uintptr_t arg)
{
    UARTMSP432DMA_Object *object = ((UART_Handle) arg)->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = ((UART_Handle) arg)->hwAttrs;
    uintptr_t   key;
    uint8_t     writeUnblock = 0;

        key = HwiP_disable();

    /* Sanity check we have stuff to free */
    if ( object->tx_depth && object->tx_transfer.specs[object->tx_toggle].size && (*object->tx_dmaCtrl[object->tx_toggle] & (uint32_t)0x00000007) == 0 ) {
        /* Release the reservation from the RingBuf as the data can now be marked as sent */
        RingBuf_free_get_contiguous(&object->writeBuffer, object->tx_transfer.specs[object->tx_toggle].size );

        /* A little EXPENSIVE logging */
        SEGGER_SYSVIEW_RecordU32x3(SYSVIEW_EID_uartWriteFree, (uintptr_t)arg, object->tx_toggle, object->tx_transfer.specs[object->tx_toggle].size);
        if (((*object->tx_dmaCtrl[object->tx_toggle] & UDMA_CHCTL_XFERSIZE_M) >> 4) != 0) {
            SEGGER_SYSVIEW_Error("Write DMA control register contained valid transfer size");
        }

        /* Zero the DMA registers */
        //*object->tx_dmaCtrl[object->tx_toggle] = 0;
        object->tx_transfer.specs[object->tx_toggle].size = 0;

        /* Advance the toggle */
        object->tx_toggle = ++object->tx_toggle % eUDMAPingPongMAX;
        object->tx_depth--;

        /* Now we have freed space in the send buffer, we can unblock any clients waiting to feed
         * more data to the buffer
         */
            writeUnblock = 1;
    } else {
        /* Ah Ohhhh, we dont have anything to free and we're
         * in a DMA Complete interrupt, thats interesting
         */
        SEGGER_SYSVIEW_Error("UARTMSP432DMA_hwiIntTxDMA_done Called without any active DMA sequences");
        }

    if (object->writeBuffer.count) {
        /* Fill all transmit buffers with valid transactions */
        UARTMSP432DMA_transmitDmaFill(hwAttrs, object);
        } else {
        /* Remove constraints set during transfer */
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
            }

    if (object->tx_depth == 0)
    {
        /* Disable the channel and reset the toggle field */
        MAP_DMA_disableChannel(object->tx_transfer.dmaChannel & 0x0F);
        object->tx_toggle = 0;
            }

        HwiP_restore(key);

    return ( writeUnblock && SemaphoreP_post(object->writeSem) );
}

/*
 *  ======== getDmaRemainingXfers ========
 */
inline uint32_t getDmaRemainingTXfers(UARTMSP432DMA_Object *object)
{
   /* Use the bit-band addresses to determine how many bytes remaining in the transmit queue
    * and check if we'rd in the alt or primary structure in case we're using ping-pong instead
    * of regular transfer
    */
   if (*object->tx_dmaAltSetBitBand)
       return *object->tx_dmaCtrl[eUDMAPingPongAlternate] & 0x07 ? ((*object->tx_dmaCtrl[eUDMAPingPongAlternate] & UDMA_CHCTL_XFERSIZE_M) >> 4) + 1 : 0;
   else
       return *object->tx_dmaCtrl[eUDMAPingPongPrimary] & 0x07 ? ((*object->tx_dmaCtrl[eUDMAPingPongPrimary] & UDMA_CHCTL_XFERSIZE_M) >> 4) + 1 : 0;
}

/* ======== InitDmaRxScatterGatherer =========
 * Setup the Rx Scatter Gather channel to infinitely loop and trigger the Ping-Ping RX buffer
 */
static void InitDmaRxScatterGatherer(UARTMSP432DMA_Object *object, UARTMSP432DMA_HWAttrsV1 const *hwAttrs)
{
    /* Task 1: Trigger ping-pong buffer */
    DMA_ControlTable triggerPingPong =
           DMA_TaskStructEntry(1, UDMA_SIZE_32,
              UDMA_SRC_INC_NONE, &object->rx_dmaSwReq,
              UDMA_DST_INC_NONE, &DMA_Control->SWREQ,
              UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER));
    /* Task 2: Re-arm the idle timeout timer */
    DMA_ControlTable rearmTimeout =
           DMA_TaskStructEntry(1, UDMA_SIZE_32,
           UDMA_SRC_INC_NONE, &object->rx_dmaIdleControl,
           UDMA_DST_INC_NONE, object->rx_timer32ControlAdr,
           UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER));
    /* Task 3: Setup idle timeout as 2x Rx rate */
    DMA_ControlTable idleTimeout =
           DMA_TaskStructEntry(1, UDMA_SIZE_32,
              UDMA_SRC_INC_NONE, &object->rx_dmaIdleTimeout,
              UDMA_DST_INC_NONE, object->rx_timer32LoadAdr,
              UDMA_ARB_4, (UDMA_MODE_MEM_SCATTER_GATHER));
    /* Task 4: Reset task control size to enable looping this task list */
    DMA_ControlTable resetTask =
           DMA_TaskStructEntry(1, UDMA_SIZE_32,
              UDMA_SRC_INC_NONE, &object->rx_dmaTaskControl,
              UDMA_DST_INC_NONE, object->rx_dmaTaskCtrl,
              UDMA_ARB_4, (UDMA_MODE_PER_SCATTER_GATHER));

    /* Copy the structure above into the last list */
    object->rx_dmaTaskList[eTask1TriggerPingPongDMA]    = triggerPingPong;
    object->rx_dmaTaskList[eTask2ReloadIdleTimeout]     = idleTimeout;
    object->rx_dmaTaskList[eTask3RearmIdleTimer]        = rearmTimeout;
    object->rx_dmaTaskList[eTask4ResetTaskCount]        = resetTask;

    /* Setup scatter gatherer channels here */
    MAP_DMA_setChannelScatterGather(hwAttrs->rxDMAChannelIndex, eTaskMax, (void*)object->rx_dmaTaskList, 1);
    /*
     * ========== This is the money maker :) ==========
     * Buffer the DMA control element now it has been initialised with first values
     * calculated in the ROM functions. This will be copied back in the LAST task
     * in the scatter gatherer list with the control type set to PERIPHERAL SCATTER GATHERER
     * mode. The last part is very important because that makes the task list pause AFTER
     * the task list has been executes, and thus waits for another RX event. An endless loop
     * of DMA tasks.
     */
    object->rx_dmaTaskControl = *object->rx_dmaTaskCtrl;
    /* enable the channel and go */
    MAP_DMA_assignChannel(hwAttrs->rxDMAChannelIndex);
    MAP_DMA_clearInterruptFlag(hwAttrs->rxDMAChannelIndex & 0x0F);
    MAP_DMA_enableChannel(hwAttrs->rxDMAChannelIndex);
}
/*
 *  ======== initHw ========
 *
 *  Configures UART peripheral
 */
static void initHw(UARTMSP432DMA_Object *object, UARTMSP432DMA_HWAttrsV1 const *hwAttrs, uint32_t inputClkFreq)
{
    int32_t baudrateIndex;
    eUSCI_UART_Config uartConfig;

    /*
     * This will never return -1, constarints prevent unsupported performance
     * levels
     */
    baudrateIndex = findBaudDividerIndex(hwAttrs->baudrateLUT, hwAttrs->numBaudrateEntries, object->baudRate,
                                         inputClkFreq);

    uartConfig.selectClockSource = hwAttrs->clockSource;
    uartConfig.clockPrescalar = hwAttrs->baudrateLUT[baudrateIndex].prescalar;
    uartConfig.firstModReg = hwAttrs->baudrateLUT[baudrateIndex].hwRegUCBRFx;
    uartConfig.secondModReg = hwAttrs->baudrateLUT[baudrateIndex].hwRegUCBRSx;
    uartConfig.parity = parityTypes[object->parityType];
    uartConfig.msborLsbFirst = hwAttrs->bitOrder;
    uartConfig.numberofStopBits = stopBits[object->stopBits];
    uartConfig.uartMode = EUSCI_A_UART_MODE;
    uartConfig.overSampling = hwAttrs->baudrateLUT[baudrateIndex].oversampling;

    MAP_UART_initModule(hwAttrs->baseAddr, &uartConfig);

    /* Enable UART and disable its interrupts. */
    MAP_UART_disableInterrupt(hwAttrs->baseAddr,
                              EUSCI_A_UART_TRANSMIT_INTERRUPT |
                              EUSCI_A_UART_RECEIVE_INTERRUPT |
                              EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT |
                              EUSCI_A_UART_STARTBIT_INTERRUPT );
    MAP_UART_clearInterruptFlag(hwAttrs->baseAddr,
                                EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG |
                                EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG |
                                EUSCI_A_UART_STARTBIT_INTERRUPT_FLAG |
                                EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG);
    MAP_UART_enableInterrupt(hwAttrs->baseAddr,
                                 EUSCI_A_UART_BREAKCHAR_INTERRUPT |
                                 EUSCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT );
    MAP_UART_enableModule(hwAttrs->baseAddr);
    MAP_UART_enableInterrupt(hwAttrs->baseAddr,
                                     EUSCI_A_UART_BREAKCHAR_INTERRUPT |
                                     EUSCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT); // |
                                     //EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(hwAttrs->intNum);

    /* Setup the RX DMA */
    UDMAMSP432_setupPingPongTransfer(&object->rx_transfer);
    /* Setup the TX DMA here, blank and in-active but plummed */
    //UDMAMSP432_setupPingPongTransfer(&object->tx_transfer);
    /* Specify the buffer channel as having high priority */
    DMA_Control->PRIOSET = (1 << (hwAttrs->bfrDMAChannelIndex & 0x0F));
    DMA_Control->PRIOSET = (1 << (hwAttrs->rxDMAChannelIndex & 0x0F));
    baudrateIndex = EUSCI_A_CMSIS(hwAttrs->baseAddr)->RXBUF;

    /* Call setup route for DMA scatter gatherer */
    InitDmaRxScatterGatherer(object, hwAttrs);

    /* Clear the interrupt flags first */
    MAP_DMA_clearInterruptFlag(hwAttrs->txDMAChannelIndex & 0x0F);
    MAP_DMA_clearInterruptFlag(hwAttrs->bfrDMAChannelIndex & 0x0F);
    MAP_DMA_enableInterrupt(hwAttrs->dmaIntNum);

    MAP_Interrupt_enableInterrupt(hwAttrs->dmaIntNum);

    DebugP_log3("UART:(%p) CPU freq: %d; UART baudRate to %d", hwAttrs->baseAddr,
            inputClkFreq, object->baudRate);
}

/*
 *  ======== perfChangeNotifyFxn ========
 *
 *  Called by Power module before and after performance level is changed.
 */
static int perfChangeNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    uint32_t clockFreq;
    PowerMSP432_Freqs powerFreqs;
    UARTMSP432DMA_Object *object = ((UART_Handle) clientArg)->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = ((UART_Handle) clientArg)->hwAttrs;

    if (eventType == PowerMSP432_START_CHANGE_PERF_LEVEL)
    {
        /* Disable peripheral before performance level change */
        MAP_UART_disableModule(hwAttrs->baseAddr);
    }
    else
    { /* eventType == PowerMSP432_DONE_CHANGE_PERF_LEVEL */
        /* Get new performance level clock frequencies */
        PowerMSP432_getFreqs((unsigned int) eventArg, &powerFreqs);
        clockFreq = (hwAttrs->clockSource == EUSCI_A_UART_CLOCKSOURCE_SMCLK) ? powerFreqs.SMCLK : powerFreqs.ACLK;

        /* Reconfigure UART peripheral */
        initHw(object, hwAttrs, clockFreq);
    }

    return (Power_NOTIFYDONE);
}

/*
 *  ======== UARTMSP432DMA_close ========
 */
void UARTMSP432DMA_close(UART_Handle handle)
{
    unsigned int i;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    MAP_UART_disableInterrupt(hwAttrs->baseAddr,
    EUSCI_A_UART_RECEIVE_INTERRUPT | EUSCI_A_UART_TRANSMIT_INTERRUPT |
    EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT |
    EUSCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT |
    EUSCI_A_UART_BREAKCHAR_INTERRUPT);
    MAP_UART_clearInterruptFlag(hwAttrs->baseAddr,
    EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG |
    EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG |
    EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    MAP_UART_disableModule(hwAttrs->baseAddr);

    if (object->hwiHandle)
    {
        HwiP_delete(object->hwiHandle);
    }
    if (object->writeSem)
    {
        SemaphoreP_delete(object->writeSem);
    }
    if (object->readSem)
    {
        SemaphoreP_delete(object->readSem);
    }
    if (object->timeoutTmr)
    {
        Timer_stop(object->timeoutTmr);
        Timer_close(object->timeoutTmr);
    }
    if (object->txdmaHandle)
    {
        UDMAMSP432_close(object->txdmaHandle, hwAttrs->txDMAChannelIndex, hwAttrs->dmaIntNum);
        object->txdmaHandle = NULL;
    }
    if (object->rxdmaHandle)
    {
       UDMAMSP432_close(object->rxdmaHandle, hwAttrs->rxDMAChannelIndex, hwAttrs->dmaIntNum);
       object->rxdmaHandle = NULL;
    }
    if (object->rxdmaBfrHandle)
    {
       UDMAMSP432_close(object->rxdmaBfrHandle, hwAttrs->bfrDMAChannelIndex, hwAttrs->dmaIntNum);
       object->rxdmaBfrHandle = NULL;
    }
    /* Remove power constraints */
    for (i = 0; object->perfConstraintMask; i++)
    {
        if (object->perfConstraintMask & 0x01)
        {
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERFLEVEL_0 + i);
        }
        object->perfConstraintMask >>= 1;
    }
    if (DMA_Control->REQMASKSET & (1 << (hwAttrs->rxDMAChannelIndex & 0x0F)) == 0)
    {
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
    } else {
        DMA_Control->REQMASKCLR = (1 << (hwAttrs->rxDMAChannelIndex & 0x0F));
    }
    Power_unregisterNotify(&object->perfChangeNotify);

    object->state.opened = false;

    DebugP_log1("UART:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== UARTMSP432DMA_control ========
 *  @pre    Function assumes that the handle is not NULL
 */

int_fast16_t UARTMSP432DMA_control(UART_Handle handle, uint_fast16_t cmd, void *arg)
{
    int bufferCount;
    unsigned char data;
    uintptr_t key;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    if (cmd <= UART_CMD_RXDISABLE) {
        /* Call the DMA Ping-Pong peek routine as we NEED to know
         * the actual size of the buffer now
         */

        /* Pre-peek lock */
        key = HwiP_disable();

        /* Perform a read ping-pong peek so the true size of the receive buffer
         * is known before the semaphore is posted, unblocking the reader
         */
        UARTMSP432DMA_ReadPingPongPeek(handle);

        bufferCount = object->readBuffer.count;

        /* Post-peek unlock */
        HwiP_restore(key);
    } else if (cmd <= UART_CMD_TXENABLE) {
        bufferCount = object->writeBuffer.count;

    }

    switch (cmd)
    {
    /* Common UART CMDs */
    case (UART_CMD_PEEK):
        *(int *) arg = RingBuf_peek(&object->readBuffer, &data) ? data : UART_ERROR;
        DebugP_log2("UART:(%p) UART_CMD_PEEK: %d", hwAttrs->baseAddr,
                *(uintptr_t*)arg);
        return (UART_STATUS_SUCCESS);

    case (UART_CMD_ISAVAILABLE):
        *(bool *) arg = (bufferCount != 0);
        DebugP_log2("UART:(%p) UART_CMD_ISAVAILABLE: %d", hwAttrs->baseAddr,
                *(uintptr_t*)arg);
        return (UART_STATUS_SUCCESS);

    case (UART_CMD_GETRXCOUNT):
        *(int *) arg = bufferCount;
        DebugP_log2("UART:(%p) UART_CMD_GETRXCOUNT: %d", hwAttrs->baseAddr,
                *(uintptr_t*)arg);
        return (UART_STATUS_SUCCESS);

    case (UART_CMD_RXENABLE):
        /* Only re-enable RX if the buffer is NOT FULL ! */
        if (object->readBuffer.count < object->readBuffer.length) {
            /* Check we have ANY space left.. if we do, the DMA channel CAN be un-masked
             * with caution to the caller.. re-enabling the RX buffer with an extremely
             * full buffer may result in an overflow... you've been warned
             */
            if (DMA_Control->REQMASKSET & (1 << (hwAttrs->rxDMAChannelIndex & 0x0F))) {
                /* Pre-peek lock */
                key = HwiP_disable();

                /* RX Enable status is define by the REQMASK flag in the DMA controller
                 * and not by any other means. Once the driver has been stared the DMA
                 * free runs, so really a boolean flag is as useless as.... on a nun..
                 */
                DMA_Control->REQMASKCLR = 1 << (hwAttrs->rxDMAChannelIndex & 0x0F);

                /* We have just unmasked the DMA channel, it could now have a pending transfer
                 * which has not been auctioned because the channel was masked before the interrupt
                 * Occurred. Manually trigger the DMA channel to spring life back into the receive
                 * DMA transaction machine
                 */
                if (MAP_UART_getInterruptStatus(hwAttrs->baseAddr, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
                {
                    DMA_Control->SWREQ = (1 << (hwAttrs->rxDMAChannelIndex & 0x0F));
                }

                /* Post-peek unlock */
                HwiP_restore(key);
            }
            /* If we're below a high water mark then clear the RTS signal to allow flow control to
             * do gods work
             */
            if (object->readBuffer.count < (object->readBuffer.length - object->ringBlkSize)) {
                /* clear the RTS if hardware flow control pins have been defined */
                if (object->rtsPinBitBand != NULL) {
                    *object->rtsPinBitBand = 0x0000;
                }
            }

            /*
             * Set power constraints to keep peripheral active receiving
             * RX interrupts and prevent a performance level change.
             */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
            Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
            Power_setConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

            DebugP_log1("UART:(%p) UART_CMD_RXENABLE: Enabled",
                    hwAttrs->baseAddr);DebugP_log1("UART:(%p) UART_control set read power constraint",
                    hwAttrs->baseAddr);
            return (UART_STATUS_SUCCESS);
        }
        DebugP_log1("UART:(%p) UART_CMD_RXENABLE: Already enabled",
                hwAttrs->baseAddr);
        return (UART_STATUS_ERROR);

    case (UART_CMD_RXDISABLE):
        /* Check the REQMASKSET register to determine if we're disabled or not. This
         * allows the read and readAsyn functions to re-enable RX on first read without
         * worrying about a pesky rx_enable flag.
         */
        if (DMA_Control->REQMASKSET & (1 << (hwAttrs->rxDMAChannelIndex & 0x0F)) == 0)
        {
            /* Pre-peek lock */
            key = HwiP_disable();

            /* Asser RTS if hardware flow control pins have been defined */
            if (object->rtsPinBitBand != NULL) {
                *object->rtsPinBitBand = 0x0001;
            }

            /* RX Enable status is define by the REQMASK flag in the DMA controller
             * and not by any other means. Once the driver has been stared the DMA
             * free runs, so really a boolean flag is as useless as.... on a nun..
             */
            DMA_Control->REQMASKSET = 1 << (hwAttrs->rxDMAChannelIndex & 0x0F);

            /* Post-peek unlock */
            HwiP_restore(key);

            /*
             * Remove constraints, this will allow the device to enter
             * LPM3 and higher as well as allow performance level changes
             * by the application.
             */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
            Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

            DebugP_log1("UART:(%p) UART_CMD_RXDISABLE: Disabled",
                    hwAttrs->baseAddr);DebugP_log1("UART:(%p) UART_control released read power "
                    "constraint", hwAttrs->baseAddr);
            return (UART_STATUS_SUCCESS);
        }
        DebugP_log1("UART:(%p) UART_CMD_RXDISABLE: Already disabled",
                hwAttrs->baseAddr);
        return (UART_STATUS_ERROR);

    case (UART_CMD_ISACTIVE):
        *(bool *) arg = (bufferCount != 0);
        DebugP_log2("UART:(%p) UART_CMD_ISACTIVE: %d", hwAttrs->baseAddr,
                *(uintptr_t*)arg);
        return (UART_STATUS_SUCCESS);
    case (UART_CMD_GETTXCOUNT):
        *(int *) arg = bufferCount;
        DebugP_log2("UART:(%p) UART_CMD_GETTXCOUNT: %d", hwAttrs->baseAddr,
                *(uintptr_t*)arg);
        return (UART_STATUS_SUCCESS);
    case (UART_CMD_TXENABLE):
        if (DMA_Control->REQMASKSET & (1 << (hwAttrs->txDMAChannelIndex & 0x0F)))
        {
            /* Pre-peek lock */
            key = HwiP_disable();

            /* Un-mask the channel */
            DMA_Control->REQMASKCLR = 1 << (hwAttrs->txDMAChannelIndex & 0x0F);

            /* Check if there are bytes remaining to be sent in the DMA control */
            if (object->writeBuffer.count)
            {
                /* Configure the Tx DMA again from scratch */
                UARTMSP432DMA_transmitDmaFill(hwAttrs, object);

                DebugP_log1("UART:(%p) UART_CMD_TXENABLE: Continue",
                        hwAttrs->baseAddr);
            }
            else
            {
                DebugP_log1("UART:(%p) UART_CMD_TXENABLE: Enabled",
                        hwAttrs->baseAddr);
            }
            /* Post-peek unlock */
            HwiP_restore(key);

            return (UART_STATUS_SUCCESS);
        }
        DebugP_log1("UART:(%p) UART_CMD_TXENABLE: Already enabled",
                hwAttrs->baseAddr);
        return (UART_STATUS_ERROR);
    case (UART_CMD_TXDISABLE):
        if (DMA_Control->REQMASKSET & (1 << (hwAttrs->txDMAChannelIndex & 0x0F)) == 0)
        {
            /* Mask the transmit ping-ping channel so it naturally stops processing new
             * requests.
             */
            DMA_Control->REQMASKSET = 1 << (hwAttrs->txDMAChannelIndex & 0x0F);

            /* Complete any outstanding transmit routines which will unblock
             * any active callers to transmit and allow them to return. Bytes in
             * the buffer already will still be sent, they are not removed from
             * the send queue, otherwise half messages would get out. SOftware must
             * handle what to do in this case.. but you likely already know because
             * you called TX Disable anyway.
             */
            UARTMSP432DMA_transmitCancel(handle);

            /* At this point the DMA transmit channel will be closed */

            DebugP_log1("UART:(%p) UART_CMD_TXDISABLE: Disabled",
                    hwAttrs->baseAddr);
            return (UART_STATUS_SUCCESS);
        }
        DebugP_log1("UART:(%p) UART_CMD_TXDISABLE: Already disabled",
                hwAttrs->baseAddr);
        return (UART_STATUS_ERROR);

    default:
        DebugP_log2("UART:(%p) UART CMD undefined: %d",
                hwAttrs->baseAddr, cmd);
        return (UART_STATUS_UNDEFINEDCMD);
    }
}

/*
 *  ======== UARTMSP432DMA_hwiIntFxn ========
 *  Hwi function that processes UART interrupts.
 *
 *  @param(arg)         The UART_Handle for this Hwi.
 */

static long UARTMSP432DMA_hwiIntFxn(uintptr_t arg)
{
    uint32_t status;
    uint32_t errorFlags;
    UARTMSP432DMA_Object *object = ((UART_Handle) arg)->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = ((UART_Handle) arg)->hwAttrs;
    long xSwitch = 0;

    status = MAP_UART_getEnabledInterruptStatus(hwAttrs->baseAddr);

    errorFlags = MAP_UART_queryStatusFlags(hwAttrs->baseAddr,
    EUSCI_A_UART_FRAMING_ERROR | EUSCI_A_UART_OVERRUN_ERROR |
    EUSCI_A_UART_PARITY_ERROR | EUSCI_A_UART_BREAK_DETECT |
    EUSCI_A_UART_RECEIVE_ERROR);

    /* Call the inline method receive to kick off any async read complete
     * rules
     */
    if (status & (EUSCI_A_UART_STARTBIT_INTERRUPT))
    {
        xSwitch |= UARTMSP432DMA_hwiIntRx(arg);
        MAP_UART_clearInterruptFlag(hwAttrs->baseAddr, EUSCI_A_UART_STARTBIT_INTERRUPT);
    }

    /* Catch-all for completion and transmit interrupts being thrown */
    if (status & (EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG  ) )
    {
        MAP_UART_clearInterruptFlag(hwAttrs->baseAddr, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG);
        UARTMSP432DMA_txDMA_done(arg);
    }

    if (errorFlags)
    {
        xSwitch |= hwAttrs->errorFxn((UART_Handle) arg, errorFlags);

        /* Reading the rxData regs clear the error flags */
        /* Call this second so we dont trump over a previous read */
        status = EUSCI_A_CMSIS(hwAttrs->baseAddr)->RXBUF;
        (void)status;
    }

    return xSwitch;
}

/*
 *  ======== UARTMSP432DMA_init ========
 */
void
UARTMSP432DMA_init(UART_Handle handle)
{
    UDMAMSP432_init();
}

/*
 *  ======== UARTMSP432DMA_open ========
 */
UART_Handle UARTMSP432DMA_open(UART_Handle handle, UART_Params *params)
{
    unsigned int i;
    uintptr_t key;
    uint32_t clockFreq;
    uint8_t numPerfLevels;
    int32_t baudrateIndex;
    union
    {
        Timer_Params tmr_params;
        HwiP_Params hwiParams;
        SemaphoreP_Params semParams;
    } portsParams;
    PowerMSP432_Freqs powerFreqs;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
    uint8_t pin;
    uint8_t port;
    uint8_t value;

    DebugP_assert((hwAttrs->clockSource == EUSCI_A_UART_CLOCKSOURCE_ACLK) ||
            (hwAttrs->clockSource == EUSCI_A_UART_CLOCKSOURCE_SMCLK));
    /* The DMA channel must be INT0 as all routines refere to this directly
     * because we need at minimum three channels, and this would be excessive
     * to write support for independent and generic INT0 routines.
     */
    DebugP_assert((hwAttrs->dmaIntNum == DMA_INT0));

    /* The ring buf block size MUST be greater than 32, otherwise bad things will happen,
     * along with equal or greater than 3 blocks allocated to the ping-pong for buffer
     * overrun support.
     */
    DebugP_assert((hwAttrs->ringBufBlockCnt >= 3) && ((hwAttrs->ringBufSize / hwAttrs->ringBufBlockCnt >= 32)));
    key = HwiP_disable();

    if (object->state.opened)
    {
        HwiP_restore(key);
        DebugP_log1("UART:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }
    object->state.opened = true;

    HwiP_restore(key);

    port = PinConfigPort(hwAttrs->rxPin);
    value = PinConfigValue(hwAttrs->rxPin);

    if (value != 0)
    {
        pin = (hwAttrs->rxPin) & 0x7;
        mapPin(port, pin, value);
    }

    pin = PinConfigPin(hwAttrs->rxPin);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port, pin,
    GPIO_PRIMARY_MODULE_FUNCTION);

    port = PinConfigPort(hwAttrs->txPin);
    value = PinConfigValue(hwAttrs->txPin);

    if (value != 0)
    {
        pin = (hwAttrs->txPin) & 0x7;
        mapPin(port, pin, value);
    }

    pin = PinConfigPin(hwAttrs->txPin);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port, pin,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /*
     * Add power management support - Disable performance transitions while
     * opening the driver is open.  This constraint remains active until a
     * UART_control() disables receive interrupts.  Afterwards performance
     * levels can be changed by the application.  A UART_control() call can
     * enable RX interrupts again and set the pertinent constraints.
     */
    Power_setConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    /*
     * Verify that the driver can be opened at current performance level and
     * set constraints for other performance levels.
     */
    numPerfLevels = PowerMSP432_getNumPerfLevels();
    PowerMSP432_getFreqs(Power_getPerformanceLevel(), &powerFreqs);
    if (hwAttrs->clockSource == EUSCI_A_UART_CLOCKSOURCE_ACLK)
    {
        /*
         * Verify if driver can be opened with ACLK; ACLK does not change
         * in any performance level.
         */
        baudrateIndex = findBaudDividerIndex(hwAttrs->baudrateLUT, hwAttrs->numBaudrateEntries, params->baudRate,
                                             powerFreqs.ACLK);
        if (baudrateIndex == -1)
        {
            DebugP_log3("UART:(%p) unable to find a valid buadrate %d "
                    "configuration at clock input clock freq %d", hwAttrs->baseAddr,
                    params->baudRate, powerFreqs.ACLK);
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
            object->state.opened = false;
            return (NULL);
        }
        clockFreq = powerFreqs.ACLK;
    }
    else
    { /* hwAttrs->clockSource == EUSCI_A_UART_CLOCKSOURCE_SMCLK */
        baudrateIndex = findBaudDividerIndex(hwAttrs->baudrateLUT, hwAttrs->numBaudrateEntries, params->baudRate,
                                             powerFreqs.SMCLK);
        if (baudrateIndex == -1)
        {
            DebugP_log3("UART:(%p) unable to find a valid buadrate %d "
                    "configuration at clock input clock freq %d", hwAttrs->baseAddr,
                    params->baudRate, powerFreqs.SMCLK);
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
            object->state.opened = false;
            return (NULL);
        }
        clockFreq = powerFreqs.SMCLK;

        /*
         * SMCLK changes with performance levels.  Set constraints for
         * unsupported performance levels.
         */
        for (i = 0; i < numPerfLevels; i++)
        {
            PowerMSP432_getFreqs(i, &powerFreqs);
            baudrateIndex = findBaudDividerIndex(hwAttrs->baudrateLUT, hwAttrs->numBaudrateEntries, params->baudRate,
                                                 powerFreqs.SMCLK);
            if (baudrateIndex == -1)
            {
                /* Set constraint and keep track of it in perfConstraintMask */
                object->perfConstraintMask |= (1 << i);
                Power_setConstraint(PowerMSP432_DISALLOW_PERFLEVEL_0 + i);
            }
        }
    }

    /*
     * For MSP432P401x devices the DEEPSLEEP_0 constraint is set to allow the
     * UART peripheral to continue to receive data.  This constraint is not
     * used with later MSP432 devices, because on later devices the CPU can
     * enter deep sleep without stopping the UART peripheral.
     */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif

    /* Register function to reconfigure peripheral on perf level changes */
    Power_registerNotify(&object->perfChangeNotify,
    PowerMSP432_START_CHANGE_PERF_LEVEL |
    PowerMSP432_DONE_CHANGE_PERF_LEVEL,
                         perfChangeNotifyFxn, (uintptr_t) handle);

    /* Create the Hwi for this UART peripheral */
    HwiP_Params_init(&(portsParams.hwiParams));
    portsParams.hwiParams.arg = (uintptr_t) handle;
    portsParams.hwiParams.priority = hwAttrs->intPriority;
    object->hwiHandle = HwiP_create(hwAttrs->intNum, UARTMSP432DMA_hwiIntFxn, &(portsParams.hwiParams));
    if (object->hwiHandle == NULL)
    {
        DebugP_log1("UART:(%p) HwiP_create() failed", hwAttrs->baseAddr);
        UARTMSP432DMA_close(handle);
        return (NULL);
    }

    SemaphoreP_Params_init(&(portsParams.semParams));
    portsParams.semParams.mode = SemaphoreP_Mode_BINARY;

    /* If write mode is blocking create a semaphore and set callback. */
    object->writeSem = SemaphoreP_create(0, &(portsParams.semParams));
    if (object->writeSem == NULL)
    {
        DebugP_log1("UART:(%p) SemaphoreP_create() failed.",
                hwAttrs->baseAddr);
        UARTMSP432DMA_close(handle);
        return (NULL);
    }

    /* If read mode is blocking create a semaphore and set callback. */
    object->readSem = SemaphoreP_create(0, &(portsParams.semParams));
    if (object->readSem == NULL)
    {
        DebugP_log1("UART:(%p) SemaphoreP_create() failed.",
                hwAttrs->baseAddr);
        UARTMSP432DMA_close(handle);
        return (NULL);
    }

    /* Initialize the hardware timer */
    if (hwAttrs->timeoutTmrIdx != -1)
    {
        TimerMSP432_HWAttrs const *tmrHwattrs;
        Timer_Params_init(&portsParams.tmr_params);

        portsParams.tmr_params.periodUnits = Timer_PERIOD_US;
        portsParams.tmr_params.timerMode = Timer_ONESHOT_CALLBACK;
        portsParams.tmr_params.period = params->readTimeout;
        portsParams.tmr_params.timerCallback = UARTMSP432DMA_timeoutIntRx;
        portsParams.tmr_params.timerCallbackArg = (uintptr_t) handle;

        /* Store the handle for the rx idle timer
         * .. This only supports Timer32 at the moment, due to scatter gather tasks
         * which do not call library methods
         */
        object->timeoutTmr = Timer_open(hwAttrs->timeoutTmrIdx, &portsParams.tmr_params);
        if (object->timeoutTmr == NULL)
        {
            DebugP_log1("UART:(%p) Timer_open() failed.", hwAttrs->baseAddr);
            UARTMSP432DMA_close(handle);
            return (NULL);
        }
        tmrHwattrs = (TimerMSP432_HWAttrs*)object->timeoutTmr->hwAttrs;
        object->rx_timer32LoadAdr = &TIMER32_CMSIS(tmrHwattrs->timerBaseAddress)->LOAD;
        object->rx_timer32ControlAdr = &TIMER32_CMSIS(tmrHwattrs->timerBaseAddress)->CONTROL;
    }
    else
    {
        DebugP_log1("UART:(%p) Timer_open(ASYNC.MUST.HAVE.TMR) failed.", hwAttrs->baseAddr);
        UARTMSP432DMA_close(handle);
        return (NULL);
    }

    /* Claim TX DMA resources */
    object->txdmaHandle = UDMAMSP432_open(hwAttrs->txDMAChannelIndex, hwAttrs->dmaIntNum, hwAttrs->intPriority,
                                          UARTMSP432DMA_hwiIntTxDMA_done,
                                          (uintptr_t) handle);
    if (object->txdmaHandle == NULL)
    {
        DebugP_log1("UART:(%p) UDMAMSP432_open() failed.", hwAttrs->baseAddr);
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
        UARTMSP432DMA_close(handle);

        return (NULL);
    }

    /* Claim the RX buffer ping pong dma channel */
    object->rxdmaBfrHandle = UDMAMSP432_open(hwAttrs->bfrDMAChannelIndex, hwAttrs->dmaIntNum, hwAttrs->intPriority,
                                             UARTMSP432DMA_hwiIntRxDMA_done,
                                             (uintptr_t) handle);
    if (object->rxdmaBfrHandle == NULL)
    {
        DebugP_log1("UART:(%p) UDMAMSP432_open() failed.", hwAttrs->baseAddr);
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
        UARTMSP432DMA_close(handle);

        return (NULL);
    }

    /* Claim RX DMA resources */
    object->rxdmaHandle = UDMAMSP432_open(hwAttrs->rxDMAChannelIndex, hwAttrs->dmaIntNum, hwAttrs->intPriority,
                                          UARTMSP432DMA_hwiIntRx,
                                          (uintptr_t) handle);
    if (object->rxdmaHandle == NULL)
    {
        DebugP_log1("UART:(%p) UDMAMSP432_open() failed.", hwAttrs->baseAddr);
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
        UARTMSP432DMA_close(handle);

        return (NULL);
    }

    /*
     * Initialize UART read buffer - will store received bytes until
     * UART_read is invoked.
     */
    object->ringBlkSize = hwAttrs->ringBufSize / hwAttrs->ringBufBlockCnt;
    object->ringBlkSizeMin = hwAttrs->ringBufBlockSizeMin;
    /* Multiply the ring buffer block count by the integer division of the ringBufSize so
     * that the size of the ring buffer is of equal proportions, otherwise small buffers
     * will cause issues during ping-pong transitions, at odd times
     */
    RingBuf_construct(&object->readBuffer, hwAttrs->ringBufPtr,
                      (object->ringBlkSize * hwAttrs->ringBufBlockCnt ));

    /*
     * Initialize UART write buffer - will store sending bytes during DMA transfer operations
     * and cause a block if the buffer is actually full
     */
    RingBuf_construct(&object->writeBuffer, hwAttrs->writeBufPtr, hwAttrs->writeBufSize);

    /*
     * Store UART parameters & initialize peripheral.  These are used to
     * re/initialize the peripheral when opened or changing performance level.
     */
    object->readTimeout = params->readTimeout;
    object->writeTimeout = params->writeTimeout;
    object->baudRate = params->baudRate;
    object->stopBits = params->stopBits;
    object->parityType = params->parityType;
    object->written = 0;
    object->rx_readMin = 0;
    object->readAsyncPrev = 0;
    object->tx_cancelInProgress = false;
    object->tx_toggle = 0;
    object->tx_depth = 0;

    /* RTS Pin used for flow control of the RX buffer. Set once the highwater mark is met, meaning less than or equal
     * to one rx block size of buffer free. This allows for run-out during flow control transition and prevents
     * your receive queue being poisoned
     */
    object->rtsPinBitBand = hwAttrs->rtsPinBitBand;

    /* Variables used in the Rx scatter gather task list */
    object->rx_dmaIdleControl = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE | TIMER32_CONTROL_MODE ;
    object->rx_dmaSwReq   = 1 << (hwAttrs->bfrDMAChannelIndex & 0x0F);
    object->rx_dmaIdleTimeout = (clockFreq / ( params->baudRate / 10 ) ) * 8;

    /* the ALT control bit is in peripheral address space */
    object->rx_dmaAltSetBitBand = (volatile uint32_t*)BITBAND_PERI_ADR(DMA_Control->ALTSET, (hwAttrs->bfrDMAChannelIndex & 0x0F));
    object->rx_dmaAltClrBitBand = (volatile uint32_t*)BITBAND_PERI_ADR(DMA_Control->ALTCLR, (hwAttrs->bfrDMAChannelIndex & 0x0F));
    object->rx_dmaCtrl[eUDMAPingPongPrimary]   = &((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->bfrDMAChannelIndex & 0x0F)].control;
    object->rx_dmaCtrl[eUDMAPingPongAlternate] = &((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->bfrDMAChannelIndex & 0x0F) | UDMA_ALT_SELECT].control;
    object->rx_dmaTaskCtrl = &((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->rxDMAChannelIndex & 0x0F)].control;

    /* tx DMA peripheral address access */
    object->tx_dmaAltSetBitBand = (volatile uint32_t*)BITBAND_PERI_ADR(DMA_Control->ALTSET, (hwAttrs->txDMAChannelIndex & 0x0F));
    object->tx_dmaAltClrBitBand = (volatile uint32_t*)BITBAND_PERI_ADR(DMA_Control->ALTCLR, (hwAttrs->txDMAChannelIndex & 0x0F));
    object->tx_dmaCtrl[eUDMAPingPongPrimary]   = &((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->txDMAChannelIndex & 0x0F)].control;
    object->tx_dmaCtrl[eUDMAPingPongAlternate] = &((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->txDMAChannelIndex & 0x0F) | UDMA_ALT_SELECT].control;
    object->tx_dmaPPbit[eUDMAPingPongPrimary]   = (volatile uint32_t*)BITBAND_SRAM_ADR(((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->txDMAChannelIndex & 0x0F)].control, 1);
    object->tx_dmaPPbit[eUDMAPingPongAlternate] = (volatile uint32_t*)BITBAND_SRAM_ADR(((DMA_ControlTable*)DMA_Control->CTLBASE)[(hwAttrs->txDMAChannelIndex & 0x0F) | UDMA_ALT_SELECT].control, 1);

    /* Setup these constants so that interrupts call into them constantly... */
    object->tx_transfer.dmaChannel = hwAttrs->txDMAChannelIndex;
    object->tx_transfer.ctlOptions = UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1;
    object->tx_transfer.specs[eUDMAPingPongPrimary].dst = (void *) MAP_UART_getTransmitBufferAddressForDMA(hwAttrs->baseAddr);
    object->tx_transfer.specs[eUDMAPingPongAlternate].dst = (void *) MAP_UART_getTransmitBufferAddressForDMA(hwAttrs->baseAddr);
    object->tx_transfer.transferMode = UDMA_MODE_PINGPONG;
    /* Start with Nulls and Zero length for safety */
    object->tx_transfer.specs[eUDMAPingPongPrimary].size = 0;
    object->tx_transfer.specs[eUDMAPingPongAlternate].size = 0;
    object->tx_transfer.specs[eUDMAPingPongPrimary].src = NULL;
    object->tx_transfer.specs[eUDMAPingPongAlternate].src = NULL;

    /* Setup the ping-pong source and sizes */
    object->rx_transfer.dmaChannel = hwAttrs->bfrDMAChannelIndex;
    object->rx_transfer.ctlOptions = UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1;
    object->rx_transfer.specs[eUDMAPingPongPrimary].src = (void *) MAP_UART_getReceiveBufferAddressForDMA(hwAttrs->baseAddr);
    object->rx_transfer.specs[eUDMAPingPongAlternate].src = (void *) MAP_UART_getReceiveBufferAddressForDMA(hwAttrs->baseAddr);
    object->rx_transfer.transferMode = UDMA_MODE_PINGPONG;
    /* Initial pointers for the ping ping RX buffer */
    object->rx_transfer.specs[eUDMAPingPongPrimary].size =
            RingBuf_alloc_put_contiguous(&object->readBuffer, object->ringBlkSize,
                                         (unsigned char **)&object->rx_transfer.specs[eUDMAPingPongPrimary].dst);
    object->rx_transfer.specs[eUDMAPingPongAlternate].size =
            RingBuf_alloc_put_contiguous(&object->readBuffer, object->ringBlkSize,
                                         (unsigned char **)&object->rx_transfer.specs[eUDMAPingPongAlternate].dst);

    initHw(object, hwAttrs, clockFreq);

    DebugP_log1("UART:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== SPIMSP432DMA_transferCancel ========
 */
void UARTMSP432DMA_transmitCancel(UART_Handle handle)
{
    uintptr_t key;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    /*
     * There are 2 use cases in which to call transferCancel():
     *   1.  The driver is in CALLBACK or ASYNC mode.
     *   2.  The driver is in BLOCKING mode & there has been a transfer timeout.
     */

    key = HwiP_disable();

    if (object->tx_cancelInProgress || object->writeBuffer.count == 0)
    {
        HwiP_restore(key);

        return;
    }
    object->tx_cancelInProgress = true;

    /* Clear DMA configuration */
    MAP_DMA_disableChannel(hwAttrs->txDMAChannelIndex & 0x0F);
    object->tx_transfer.specs[eUDMAPingPongPrimary].size = 0;
    object->tx_transfer.specs[eUDMAPingPongAlternate].size = 0;
    object->tx_dmaCtrl[eUDMAPingPongPrimary] = 0;
    object->tx_dmaCtrl[eUDMAPingPongAlternate] = 0;

    /* Clear the entire send queue */
    RingBuf_free_get_contiguous(&object->writeBuffer, hwAttrs->writeBufSize);

    HwiP_restore(key);

    /* Remove constraints set during transfer */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
    Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
    Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    /* Indicate we are done with this transfer */
    object->tx_cancelInProgress = false;
}

/*
 *  ======== UARTMSP432DMA_write ========
 */
int_fast32_t UARTMSP432DMA_write(UART_Handle handle, const void *buffer, size_t size)
{
    uintptr_t key;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
    unsigned char *writePtr = (unsigned char*) buffer;
    unsigned int lwriteCount = 0;
    unsigned int count;

    if (!size)
    {
        return (0);
    }

    SEGGER_SYSVIEW_RecordU32x2(SYSVIEW_EID_uartWriteEnter, (uintptr_t)handle, size);

    /*
       * It is possible for the object->timeoutClk and the callback function to
       * have posted the object->readSem Semaphore from the previous UART_read
       * call (if the code below didn't get to stop the clock object in time).
       * To clear this, we simply do a NO_WAIT pend on (binary) object->readSem
       * so that it resets the Semaphore count.
       */
    SemaphoreP_pend(object->writeSem, SemaphoreP_NO_WAIT);

    /* Put the data into the send buffer */
    /* WARNING.. THIS OPERATION COULD BE A COPY OF UP TO BUFFER SIZE...IE 1024 bytes
     * which can take
     */
    /* Prep the DMA queue */
    key = HwiP_disable();
    /* Attempt to add the buffer to the send queue where possible */
    count = RingBuf_put_buffer(&object->writeBuffer, writePtr, size);

    UARTMSP432DMA_transmitDmaFill(hwAttrs, object);
    size -= count;
    writePtr += count;
    lwriteCount = count;

    HwiP_restore(key);

    /*
     * Set power constraint to keep peripheral active during transfer and
     * to prevent a performance level change
     */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
    Power_setConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    while (size && (SemaphoreP_OK != SemaphoreP_pend(object->writeSem, object->writeTimeout)) ) {
        /* Always lock this out here, otherwise DMA could interrupt and mess with
         * our ring buffer without our knowledge
         */
        key = HwiP_disable();

        /* Put the data into the send buffer */
        count = RingBuf_put_buffer(&object->writeBuffer, writePtr, size);

        /* Configure the Xmit DMA to ping-pong away */
        UARTMSP432DMA_transmitDmaFill(hwAttrs, object);

        /* Commentary: Technically if your Tx ping-pong buffers are small, you can return
         * quicker to the transmitter with available buffer space, instead of waiting giant
         * buffer sizes. Depends on your application.. I havn't implemented a send buffer
         * block size other than using the same size of the RX ring blocks.
         */
        size -= count;
        writePtr += count;
        lwriteCount += count;

        HwiP_restore(key);
    }

    if (size) {
        /* Semaphore timed out, make the write empty and log the write. */
        UARTMSP432DMA_transmitCancel(handle);

        if (object->written != 0) {
            DebugP_log2("UART:(%p) Write timed out, %d bytes written",
                    hwAttrs->baseAddr, object->written);
        }
    }

    /* Return the number of bytes written by the interrupt processes */
    object->written = 0;

    SEGGER_SYSVIEW_RecordU32x3(SYSVIEW_EID_uartWriteExit, (uintptr_t)handle, lwriteCount, object->writeBuffer.count );

    return (lwriteCount);
}

/*
 *  ======== UARTMSP432_readPolling ========
 */
int_fast32_t UARTMSP432DMA_readPolling (UART_Handle handle, void *buf, size_t size)
{
    int_fast32_t read;
    size_t  bufferSize = size;
    /*
     * if you're using this driver code you're not calling an actual polling method...
     *
     * ever...
     *
     * Just saying...
     *
     * /rantover
     *
     * Call the read in a loop until all bytes are received OR we get a read error
     */
    while ( (size > 0) && (read = UARTMSP432DMA_read(handle, buf, size)) >= 0) {
        buf = (void*)((uintptr_t)buf + (uintptr_t)read);
        size -= read;
    }

    /*
     * Either return the error code or return the amount
     * of bytes read into the buffer..
     */
   return read < 0 ? read : (bufferSize - size);
}

/*
 *  ======== UARTMSP432DMA_writePolling ========
 */
int_fast32_t UARTMSP432DMA_writePolling(UART_Handle handle, const void *buf, size_t size)
{
    size_t added;
    UARTMSP432DMA_Object *object = handle->object;
    UARTMSP432DMA_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    /* Use the DMA write call. except wait for every last byte to be written
     * to the output.. Thats the only difference here. Its more of a Write with
     * confirmation of completion
     */
    if ((added = UARTMSP432DMA_write(handle, buf, size)) == size)
    {
        while (getDmaRemainingTXfers(object) != 0 && SemaphoreP_pend(object->writeSem, SemaphoreP_WAIT_FOREVER) != SemaphoreP_TIMEOUT) ;

        DebugP_log2("UART:(%p) Write success, %d bytes written",
                        hwAttrs->baseAddr, added);

        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

        return size;
    } else {
        /* This shuts up the compiler warning */
        (void)added;
        (void)hwAttrs;
        DebugP_log2("UART:(%p) Write failed adding to transmit queue early finished, %d bytes written",
                hwAttrs->baseAddr, added);

        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

        return UART_STATUS_ERROR;
    }
}

/*
 *  ======== mapPin ========
 */
static void mapPin(uint8_t port, uint8_t pin, uint8_t value)
{
    volatile uint8_t pmap;

    pmap = port * 0x8;  // 2 -> 0x10, 3 -> 0x18, 7 -> 0x38

    /*  Code from pmap.c: */
    /* Get write-access to port mapping registers */
    PMAP->KEYID = PMAP_KEYID_VAL;

    /* Enable reconfiguration during runtime */
    PMAP->CTL = (PMAP->CTL & ~PMAP_CTL_PRECFG) | PMAP_ENABLE_RECONFIGURATION;
    HWREG8((uint32_t)PMAP_BASE + pin + pmap) = value;

    /* Disable write-access to port mapping registers: */
    PMAP->KEYID = 0;
}
