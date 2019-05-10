/*
 * dma0_thread.c
 *
 *  Created on: 17 Apr. 2019
 *      Author: lucas.oldfield
 */

/* Includes */
#include <callbacks.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <FreeRTOS.h>
#include <SEGGER_SYSVIEW.h>
#include <ITM.h>

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART.h>

/* Board Header files */
#include "Board.h"

#include <ti/devices/msp432p4xx/driverlib/crc32.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

#include "led_thread.h"
#include "dma_thread.h"

/* Global Variables */
pthread_t dma_master_thread;
pthread_t dma_loopback_thread;
sem_t semDMAMaster;
sem_t semDMALoopback;
UART_Handle uartMasterHandle;
UART_Handle uartLoopbackHandle;
uint32_t dmaUartBaudrate = 1000000;
uint32_t dmaUartTimeout = 10000;
mqd_t  mqDMALoopback;
mqd_t  mqDMAMaster;

long gpioCtsMaster (uint_least8_t x);
long gpioCtsLoopback (uint_least8_t x);
void led_send_rgb_hz(int32_t red, int32_t green, int32_t blue, int32_t bpm);
void terminal_respond(UART_Handle terminal, char * format, ...);

static char response_buffer[100] = { 0 };

void terminal_respond(UART_Handle terminal, char * format, ...)
{
    va_list args;
    va_start (args, format);
    vsnprintf (response_buffer, 100, format, args);
    va_end (args);
    UART_write(terminal, response_buffer, strlen(response_buffer));
}

/*
 * Specify -1 to leave a colour or feature unchanged otherwise between 0 and 255 for colour
 * and 0 and 60 for bpm
 */
void led_send_rgb_hz(int32_t red, int32_t green, int32_t blue, int32_t bpm)
{
    static LedMsg msg;
    int rc;
    /* R G B BPM */
    static int32_t receiveArray[4] = {-1, -1, -1, -1};

    if (red >= 0 && red <= 255)
        receiveArray[0] = (int32_t) (pwmPeriod * (red / 255.0f));

    if (green >= 0 && green <= 255)
        receiveArray[1] = (int32_t) (pwmPeriod * (green / 255.0f));

    if (blue >= 0 && blue <= 255)
        receiveArray[2] = (int32_t) (pwmPeriod * (blue / 255.0f));

    if (bpm >= 0 && bpm <= 60)
        receiveArray[3] = (int32_t) ((60.0f / (bpm*60)) * 1000000 / 2);

    /* Send message to update LED color and blinkrate */
    msg.cmd = LedCmd_PC_UPDATE;
    msg.buffer = (void *)(&receiveArray);
    rc = mq_send(mqLED, (char *)&msg, sizeof(msg), 0);
    if (rc == -1) {
        // Failed to send to message queue
    }
    rc = sem_post(&semLED);
    if (rc == -1) {
        while (1);
    }
}


long dma_uart_error(UART_Handle handle,  uint32_t error)
{
    SEGGER_SYSVIEW_Error("UART error received");
    GPIO_write(MSP_EXP432P4111_GPIO_LED1, 1);

    return 0;
}

__attribute__((ramfunc))
long gpioCtsMaster (uint_least8_t x)
{
    // CTS has gone High, meaning break communications plz
    if (GPIO_read(MSP_EXP432P4111_DMA_UART0_CTS) == 0)
    {
        SEGGER_SYSVIEW_Print("CTS_UART0: Low");
        GPIO_write(MSP_EXP432P4111_GPIO_LED1, 0);
    }
    else
    {
        SEGGER_SYSVIEW_Print("CTS_UART0: High");
        GPIO_write(MSP_EXP432P4111_GPIO_LED1, 1);
    }

    return 0;
}

UART_Handle dmaUartOpen(MSP_EXP432P4111_UARTName eUart, MSP_EXP432P4111_GPIOName eCts,
                        GPIO_CallbackFxn callback, UART_Handle existing)
{
    UART_Params uartParams;

    /* Call driver init functions */
    UART_init();
    GPIO_init();
    Timer_init();

    if (existing != NULL)
    {
        /* Always cancel down read/write operations */
        UART_readCancel(existing);
        UART_writeCancel(existing);
        UART_close(existing);
    }
    else
    {
        /* This only activates the first time */
        /* Establish the CTS GPIO */
        GPIO_setConfig   (eCts, GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_BOTH_EDGES);
        GPIO_setCallback (eCts, callback);
        GPIO_enableInt   (eCts);
    }

    // Create a UART with data processing off.
    UART_Params_init (&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.readTimeout = dmaUartTimeout;
    uartParams.baudRate = dmaUartBaudrate;

    return UART_open (eUart, &uartParams);
}

static uint8_t sendBfr[8192];
static uint16_t testNumber = 0;

typedef struct uart_sendHeader {
    uint16_t    headerBits; // Always 0xFF55
    uint16_t    sendSize;   // Size of bytes after header
    uint16_t    testNumber; // Test run-number
    uint16_t    headerCRC;  // CRC32 of the sendSize and testNumber only
} sendHeader_t;

/*
 *  ======== lcdThread ========
 *  Handles displaying various texts on the segmented LCD
 */
void *dmaMasterThread(void *arg0)
{
    UART_Handle terminalHandle;
    volatile uint8_t *buffer;
    size_t readSize;
    long   writeCommited = 0;
    long   readRingSize;
    long   readReturnValue;
    long   SendStreamCountDown = 0;
    size_t SendStreamIndexUp = 0;
    long   ReceiveStreamCountDown = 0;
    int previous = 0;
    DmaMsg msg;
    int rc;
    int retc;
    int writeCount = 0;
    int readCount = 0;
    uint8_t ReceivePaused = 0;
    sendHeader_t sendHeader = {0};
    sendHeader_t readHeader;

    /* Initialize the sendBfr to increment by one */
    for (readSize = 0; readSize < sizeof(sendBfr) ; readSize++)
    {
        // Clip byte values from 0 to 0xFE, 0xFF is special please
        sendBfr[readSize] = (uint8_t)(readSize % 0xFF);
    }

    readSize = 0;

    uartMasterHandle = dmaUartOpen(MSP_EXP432P4111_DMA0, MSP_EXP432P4111_DMA_UART0_CTS,
                                   gpioCtsMaster, uartMasterHandle );

    if (uartMasterHandle == NULL)
    {
        while (1);
    }

    while (1)
    {
        /* If there are bytes to be streamed out then do it here and decrement once complete */
        if (SendStreamCountDown)
        {
            /* Write out in block sizes of 256, and wrap the writing so that we get a nice 0-255 write cycle going
             * through the UART. This will allow the recieve function to check if there is a sync or buffer < left or > right
             * shift by one error on wrap around of ring buffers or alike
             */

            writeCommited = UART_write(uartMasterHandle, &sendBfr[SendStreamIndexUp],
                                       SendStreamCountDown >= (sizeof(sendBfr) - SendStreamIndexUp) ?
                               (sizeof(sendBfr) - SendStreamIndexUp) :
                               SendStreamCountDown );
            writeCount += writeCommited;
            ITM_put_32((ITM_port_t)12, writeCount | (testNumber << 16));
            terminal_respond( terminalHandle, "{\"State\": \"Writing\", \"TestNumber\": %d, \"WriteCount\": %d, \"ReadCount\": %d, \"PacketSize\": %d}\n",
                                                                           testNumber, writeCount, readCount, sendHeader.sendSize + sizeof(sendHeader) );
            /* Check if we wrote bytes into the output queue or we received an error, meaning -1 response */
            if (writeCommited >= 0 && writeCommited <= SendStreamCountDown) {
                SendStreamCountDown -= writeCommited;

                if (SendStreamCountDown == 0) {
                    /*
                     * Reset the up index for the next pass
                     */
                    SendStreamIndexUp = 0;

                } else {
                    /* Wrap the up stream index around the one byte marker so we send from the correct pre-allocated
                     * number block
                     */
                    SendStreamIndexUp = (SendStreamIndexUp + writeCommited) % sizeof(sendBfr);
                }
            } else {
                /* We got a write error.. so exit out of this write sequence with an error */
                SendStreamCountDown = 0;
                ReceiveStreamCountDown = 0;
                SendStreamIndexUp = 0;
                /* Inspect writeCommited here as the error code and/or writeCount response was wrong */
                led_send_rgb_hz(255, 0, 64, 10);
                SEGGER_SYSVIEW_Error( "Log an error here to the LCD/Console" );
            }
        }
        else if ( ReceiveStreamCountDown )
        {
            if (sendHeader.headerBits == 0xFF55 && readHeader.headerBits != 0xFF55)
            {
                /* We are searching, reading the header.. we want exactly the header size before proceeding. Anything
                 * less or erroneous is an error state
                 */
                /* Handle the read down from the receive buffer here */
                readRingSize = UART_readAsync(uartMasterHandle, 0, previous, &buffer, &readSize);

                readCount += readCount;
                ITM_put_32((ITM_port_t)13, readCount | (testNumber << 16));

                terminal_respond( terminalHandle, "{\"State\": \"Reading\", \"TestNumber\": %d, \"WriteCount\": %d, \"ReadCount\": %d, \"PacketSize\": %d}\n",
                                                                               testNumber, writeCount, readCount, sendHeader.sendSize + sizeof(sendHeader) );

                /* Check if we're wrapping. then head based on continuous buffer sizes */
                if (readRingSize >= sizeof(sendHeader) )
                {
                    if (readSize > sizeof(sendHeader)) {
                        /* Read in one chunk */
                        memcpy(&readHeader, (const void*)buffer, sizeof(sendHeader));
                        /* Setup previous read size to be equal to the header size, thus only freeing the header off the
                         * ring buffer and leaving all other bytes for inspection
                         */
                        previous = sizeof(sendHeader);
                    } else  if (readSize == sizeof(sendHeader)) {
                        /* Read in one chunk */
                        memcpy(&readHeader, (const void*)buffer, sizeof(sendHeader));
                        /* Previous is header size, because readSize was header writeCount */
                        previous = readSize;
                    } else {
                        /* Read in two passes */
                        memcpy(&readHeader, (const void*)buffer, readSize);
                        /* This will pass immediately and clear the previous read buffer back into the ring
                         * buffer
                         */
                        if ( (readReturnValue = UART_read(uartMasterHandle, (void*)(((uintptr_t)&readHeader) + readSize), sizeof(readHeader) - readSize))
                        != ( sizeof(readHeader) - readSize))
                        {
                            /* Read error occurred, we should have been returned the exact size of the read because the size of
                             * whats in the buffer /was/ known, and thus should have been returned to us without fault. Inspect
                             * the returned value code for more information
                             */
                            led_send_rgb_hz(255, 0, 0, 10);
                            SEGGER_SYSVIEW_Error( "Log an error here to the LCD/Console as we got an unexpected result from the uart read call");
                            /* Clear down the variables controlling the reading operation */
                            ReceiveStreamCountDown = 0;
                            memset(&readHeader, 0, sizeof(readHeader));
                        }
                        /* Untrack the previous size as it should now be zero anyway */
                        previous = -1;
                    }
                    if (readHeader.headerBits == 0xFF55) {
                        CRC32_setSeed(0x1337, CRC32_MODE);
                        CRC32_set32BitData(*(uint32_t*)&readHeader.sendSize);
                        /* Set the headerCRC, but disallow 0xFF as a value, otherwise my scope
                         * wont combine the header and footers correctly for easy analysis
                         */
                        if ( readHeader.headerCRC == ( CRC32_getResult(CRC32_MODE) & (uint32_t)0x00007F7F) ) {
                            SEGGER_SYSVIEW_PrintfTarget( "Header received OK, test: %d expecting: %d payload: %d total", readHeader.testNumber, readHeader.sendSize, readHeader.sendSize + sizeof(readHeader) );
                        } else {
                            terminal_respond( terminalHandle, "{\"Result\":\"Bad Header CRC\", \"TestNumber\": %d}\n", testNumber );
                            SEGGER_SYSVIEW_Error( "Head signature correct but CRC bad, ignoring data" );
                            previous = -1;
                        }
                    } else {
                        terminal_respond( terminalHandle, "{\"Result\":\"Bad Header Signature\", \"TestNumber\": %d}\n", testNumber );
                        SEGGER_SYSVIEW_Error( "Head signature incorrect, ignoring data" );
                        previous = -1;
                    }
                } else if (readRingSize == 0) {
                    /* We've done a read and come back blank so bail out to the command semaphore */
                    ReceiveStreamCountDown = 0;
                } else {
                    /* We need more data from the port */
                    previous = -1;
                }
            } else {
                /* Call the readAsync function here with full receive size. Internal checks should truncate the request into
                 * manageable units to prevent a dead-lock from occurring. A read timeout is considered a fault because this
                 * test framework should be operating in free running mode.
                 */
                readRingSize = UART_readAsync(uartMasterHandle, ReceiveStreamCountDown, previous, &buffer, &readSize);

                readCount += readSize;
                ITM_put_32((ITM_port_t)13, readCount | (testNumber << 16) );
                terminal_respond( terminalHandle, "{\"State\": \"Reading\", \"TestNumber\": %d, \"WriteCount\": %d, \"ReadCount\": %d, \"PacketSize\": %d}\n",
                                                                                 testNumber, writeCount, readCount, sendHeader.sendSize + sizeof(sendHeader) );

                /* Check the return code/size for errors before processing the buffer */
                if (readRingSize > 0 && readRingSize >= readSize && readSize > 0) {
                    int checkCountDown = readSize;
                    uint32_t byteIndex;

                    for (byteIndex = SendStreamIndexUp; checkCountDown > 0; checkCountDown--)
                    {
                        if (*buffer++ != (byteIndex++ % 0xFF))
                        {
                            led_send_rgb_hz(255, 64, 0, 4);
                            /* Flush the read buffer please */
                            readSize += UART_read(uartMasterHandle, NULL, 0);
                            terminal_respond( terminalHandle, "{\"Result\":\"Payload sync lost\", \"TestNumber\": %d, \"Received\": %d}\n", testNumber, readSize );
                            SEGGER_SYSVIEW_Error( "Log an error here because the byte value does not match what should be in the stream" );
                            readSize = 0;
                            ReceiveStreamCountDown = 0;
                            previous = 0;
                            SendStreamIndexUp = 0;
                            memset(&readHeader, 0, sizeof(readHeader));

                            break;
                        }
                    }

                    /* Wrap the index for the comparator */
                    SendStreamIndexUp = (SendStreamIndexUp + readSize ) % sizeof(sendBfr);
                    /* We are now receiving the stream section of the buffer. Compare each byte that comes through and compare it
                     * against the known index value that it should be
                     */
                    ReceiveStreamCountDown -= readSize;
                    /* Always store the previous read size so the buffer can be cleared on the next read cycle
                     */
                    previous = readSize;

                    if (ReceiveStreamCountDown == 0) {
                        terminal_respond( terminalHandle, "{\"Result\":\"All received\", \"TestNumber\": %d, \"bytes\": %d}\n", testNumber, readHeader.sendSize + sizeof(readHeader) );
                        SEGGER_SYSVIEW_Print( "Byte stream received all OK" );
                    }
                } else {
                    /* Perform a read size of zero. This will flush the read buffers. A somewhat equivelent of cat > /dev/null
                     */
                    if (readRingSize > 0) {
                        led_send_rgb_hz(255, 0, 0, 10);
                        terminal_respond( terminalHandle, "{\"Result\":\"Failure; ReadSize > RingSize not possible\", \"TestNumber\": %d}\n", testNumber );
                        SEGGER_SYSVIEW_Error( "Log and error here to the LCD/Console as readSize was somehow larger than readRingSize" );

                        UART_read(uartMasterHandle, NULL, 0);
                    } else if (readRingSize == 0) {
                        led_send_rgb_hz(255, 0, 0, 10);
                        terminal_respond( terminalHandle, "{\"Result\":\"Failure; ReadRingSize is zero\", \"TestNumber\": %d}\n", testNumber );
                        SEGGER_SYSVIEW_Error( "Log and error here to the LCD/Console as either we got ZERO bytes back" );

                    } else {
                        led_send_rgb_hz(255, 64, 0, 4);
                        terminal_respond( terminalHandle, "{\"Result\":\"Failure; Unknown library error\", \"TestNumber\": %d}\n", testNumber );
                        SEGGER_SYSVIEW_Error( "Log and error here to the LCD/Console as either we got an error code from the library call" );
                    }

                    ReceiveStreamCountDown = 0;
                    previous = -1;
                    SendStreamIndexUp = 0;
                    memset(&readHeader, 0, sizeof(readHeader));
                }

            }
        } else {
            /* Wait for a change to occur */
            retc = sem_wait(&semDMAMaster);
            if (retc == 0)
            {
                retc = mq_receive(mqDMAMaster, (char *) &msg, sizeof(msg),
                                  NULL);
                if (retc != -1)
                {
                    terminalHandle = msg.terminalHandle;
                    switch (msg.cmd)
                    {
                    case dmaCmd_ChangePortSettings: // valueIntArray=<Baud>,<Timeout>
                        dmaUartBaudrate = ((uint32_t *)(msg.buffer))[0];
                        dmaUartTimeout  = ((uint32_t *)(msg.buffer))[1];

                        /* Change the master speed first, then the space */
                        uartMasterHandle = dmaUartOpen(MSP_EXP432P4111_DMA0, MSP_EXP432P4111_DMA_UART0_CTS,
                                                           gpioCtsMaster, uartMasterHandle );

                        /* Null out the receive pointers and counters as we're reset due to close/open */
                        previous = 0;
                        buffer = NULL;
                        readSize = 0;
                        memset(&sendHeader,0, sizeof(sendHeader));

                        /* Now change the loopback which will cause the loop to exit and wait on the semaphore */
                        uartLoopbackHandle = dmaUartOpen(MSP_EXP432P4111_DMA1, MSP_EXP432P4111_DMA_UART1_CTS,
                                                             gpioCtsLoopback, uartLoopbackHandle );

                        /* Send the same message to the loop-back thread.. Its mainly information at this point
                         * as the port speed has already changed */
                        rc = mq_send(mqDMALoopback, (char *) &msg, sizeof(msg), 0);
                        if (rc == -1)
                        {
                            // Failed to send to message queue
                        }
                        rc = sem_post(&semDMALoopback);
                        if (rc == -1)
                        {
                            while (1) ;
                        }

                        break;
                    case dmaCmd_PauseReceiving:     // valueInt=<Master or Loop>
                        ReceivePaused = 1;
                        UART_control(uartMasterHandle, UART_CMD_RXDISABLE, NULL);
                        break;

                    case dmaCmd_ResumeReceiving:    // valueInt=<Master or Loop>
                        ReceivePaused = 0;
                        UART_control(uartMasterHandle, UART_CMD_RXDISABLE, NULL);
                        break;

                    case dmaCmd_SendStream:        // valueInt=<number Of Bytes>
                        if (uartMasterHandle != NULL)
                        {
                            sendHeader.headerBits = 0xFF55;
                            sendHeader.sendSize = ((uint32_t *)(msg.buffer))[0];
                            sendHeader.testNumber = ++testNumber;

                            CRC32_setSeed(0x1337, CRC32_MODE);
                            CRC32_set32BitData(*(uint32_t*)&sendHeader.sendSize);
                            /* Set the headerCRC, but disallow 0xFF as a value, otherwise my scope
                             * wont combine the header and footers correctly for easy analysis
                             */
                            sendHeader.headerCRC = CRC32_getResult(CRC32_MODE) & (uint32_t)0x00007F7F;
                            memset(&readHeader, 0, sizeof(readHeader));
                            readCount = 0;

                            /* Send the header down the pipe for the slave to know some test information */
                            if ( (writeCount = UART_write(uartMasterHandle, (uint8_t*)&sendHeader, sizeof(sendHeader))) != sizeof(sendHeader)) {
                                terminal_respond( terminalHandle, "{\"Result\":\"Failure; Cannot send header\", \"TestNumber\": %d}\n", testNumber );
                                led_send_rgb_hz(255, 0, 64, 10);
                            } else {
                                led_send_rgb_hz(0, 0, 255, 1);
                                /* Setup the send size so we loop on the write and read operations */
                                SendStreamCountDown = sendHeader.sendSize;
                                ReceiveStreamCountDown = sendHeader.sendSize;
                                SendStreamIndexUp = 0;
                                terminal_respond( terminalHandle, "{\"State\": \"Header\", \"TestNumber\": %d, \"WriteCount\": %d, \"ReadCount\": %d, \"PacketSize\": %d}\n",
                                                                             testNumber, writeCount, readCount, sendHeader.sendSize + sizeof(sendHeader) );
                                ITM_put_32((ITM_port_t)12, writeCount| (testNumber << 16));
                                ITM_put_32((ITM_port_t)13, readCount | (testNumber << 16));
                            }


                        } else {
                            led_send_rgb_hz(255, 0, 255, 10);
                        }
                        break;
                    }
                }
            }
        }
    }
}

__attribute__((ramfunc))
long gpioCtsLoopback (uint_least8_t x)
{
    // CTS has gone High, meaning break communications plz
    if (GPIO_read(MSP_EXP432P4111_DMA_UART1_CTS) == 0)
    {
        SEGGER_SYSVIEW_Print("CTS_UART1: Low");
        GPIO_write(MSP_EXP432P4111_GPIO_LED1, 0);
    }
    else
    {
        SEGGER_SYSVIEW_Print("CTS_UART1: High");
        GPIO_write(MSP_EXP432P4111_GPIO_LED1, 1);
    }

    return 0;
}

/* Direct loopback of all data to the serial port */
void *dmaLoopbackThread(void *arg0)
{
    long rxRingSize = 0;
    uint8_t ReceivePaused = 0;
    volatile uint8_t *buffer = NULL;
    DmaMsg msg;
    int retc;
    size_t readSize = 0;
    size_t previous = 0;
    size_t readCount = 0;
    size_t writeCount = 0;
    int writeSize = 0;

    uartLoopbackHandle = dmaUartOpen(MSP_EXP432P4111_DMA1, MSP_EXP432P4111_DMA_UART1_CTS,
                                     gpioCtsLoopback, uartLoopbackHandle );

    if (uartLoopbackHandle == NULL)
    {
        while (1);
    }

    while (1)
    {
        while (!ReceivePaused
        && uartLoopbackHandle != NULL
        && (rxRingSize = UART_readAsync(uartLoopbackHandle, 0, previous, &buffer, &readSize) ) > 0)
        {
            readCount += readSize;
            ITM_put_32((ITM_port_t)11, readCount | (testNumber << 16));
            if (uartLoopbackHandle != NULL) {
                if ( ( writeSize = UART_write(uartLoopbackHandle, (const void *)buffer, readSize)) != readSize)
                {
                    if (writeSize < 0 ) {
                        led_send_rgb_hz(255, -1, 64, 10);
                        /* Write stream gave an error code. Not good */
                        previous = 0;
                        SEGGER_SYSVIEW_Error("Write size returned less than zero" );
                    } else {
                        /* Partial write success, only chop off how much we have written back */
                        led_send_rgb_hz(-1, -1, 64, 4);
                        previous = writeSize;
                        SEGGER_SYSVIEW_Warn("Partial write success, another loop should complete this" );
                    }
                } else {
                    /* All OK here
                     * Read-Write stream flowing OK
                     * */
                    previous = readSize;
                    led_send_rgb_hz(-1, 255, 255, 1);
                }
                writeCount += writeSize;
                ITM_put_32((ITM_port_t)10, writeCount | (testNumber << 16));
            } else {
                /* Serial port closing... out we go */
                previous = 0;
                rxRingSize = 0;
                buffer = NULL;
            }
        }
        /* Try wait if we're in full song */
        if (rxRingSize == 0 && uartLoopbackHandle != NULL)
        {
            retc = sem_trywait(&semDMALoopback);
        }
        /* otherwise wait for restart */
        else if (rxRingSize < 0 || uartLoopbackHandle == NULL)
        {
            /* A read error occurred, so check if we need to do anything. It could be our serial port speed is changing and
             * the read just got cancelled on us
             */
            /* Wait for a change to occur */
            retc = sem_wait(&semDMALoopback);
        } else {
            /* No auto fall through */
            retc = 1;
        }
        if (retc == 0)
        {
            retc = mq_receive(mqDMALoopback, (char *) &msg, sizeof(msg), NULL);
            if (retc != -1)
            {
                switch (msg.cmd)
                {
                case dmaCmd_ChangePortSettings:
                    previous = 0;
                    ReceivePaused = 0;
                    rxRingSize = -1;
                    if (uartLoopbackHandle != NULL) {
                        /* Flush receive queue */
                        UART_read(uartLoopbackHandle, NULL, 0);
                    }
                    readCount = 0;
                    writeCount = 0;
                    ITM_put_32((ITM_port_t)10, writeCount | (testNumber << 16));
                    ITM_put_32((ITM_port_t)11, readCount  | (testNumber << 16));
                    SEGGER_SYSVIEW_Print("Loop-back changing port speed");
                    break;

                case dmaCmd_PauseReceiving:     // valueInt=<Master or Loop>
                    ReceivePaused = 1;
                    rxRingSize = -1;
                    SEGGER_SYSVIEW_Print("Loop-back pausing receiving");
                    if (uartLoopbackHandle != NULL)
                        UART_control(uartLoopbackHandle, UART_CMD_RXDISABLE, NULL);
                    break;

                case dmaCmd_ResumeReceiving:    // valueInt=<Master or Loop>
                    ReceivePaused = 0;
                    SEGGER_SYSVIEW_Print("Loop-back resuming receiving");
                    if (uartLoopbackHandle != NULL)
                        UART_control(uartLoopbackHandle, UART_CMD_RXDISABLE, NULL);
                    break;
                }
            }
        }
    }
}
