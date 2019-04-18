/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 FlashCtl - Non-Blocking Sector Erase
 *
 * Description: In this DriverLib example a simple non-blocking sector erase is
 * shown. The maximum number of tries is parsed from the TLV and the erase is
 * initiated. An interrupt occurs when the erase completes and the main loop
 * handles the logic for verifying the memory and keeping track of the current
 * attempt number versus the maximum number of tries allowed. Once completed,
 * P1.0 is toggled continuous to signify completion.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.0  |---> P1.0 LED
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Top of bank one where we will initiate the release */
#define FLASH_ERASE_LOCATION 0x20000

/* Buffer that we will use to write the sector before we erase it */
uint8_t writeBuffer[4096];

int main(void)
{
    uint32_t mTries, currentTry, ii;
    uint_fast8_t tlvLength;
    SysCtl_FlashTLV_Info *flInfo;
    bool operationCompleted;

    /* Something */
    MAP_WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* This variable will keep track of our current try */
    currentTry = 0;
    operationCompleted = false;

    /* P1.0 Output */
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    /* Filling our buffer and writing it to memory using the blocking program
     * API
     */
    memset(writeBuffer, 0x00, 4096);
    MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR0);
    MAP_FlashCtl_programMemory(writeBuffer, (void*) FLASH_ERASE_LOCATION, 4096);

    /* Parsing the TLV and getting the maximum erase pulses. If the number of
     * retries cannot be found for some reason, set arbitrarily to 50 */
    MAP_SysCtl_getTLVInfo(TLV_TAG_FLASHCTL, 0, &tlvLength,
            (uint32_t**) &flInfo);

    if (tlvLength == 0 || flInfo->maxErasePulses == 0)
    {
        mTries = 50;
    } else
    {
        mTries = flInfo->maxErasePulses;
    }

    /* Enabling erase completion interrupts */
    MAP_FlashCtl_clearInterruptFlag(FLASH_ERASE_COMPLETE);
    MAP_FlashCtl_enableInterrupt(FLASH_ERASE_COMPLETE);
    MAP_Interrupt_enableInterrupt(INT_FLCTL);

    while (1)
    {
        /* Making sure we haven't tried to erase the max number of
         * iterations that are considered an error */
        if (currentTry == 0)
        {
            MAP_FlashCtl_initiateSectorErase(FLASH_ERASE_LOCATION);
            currentTry++;
        } else if (currentTry < mTries)
        {
            /* Verify that th ememory was erased, if not try again */
            if (!(MAP_FlashCtl_verifyMemory((void*) FLASH_ERASE_LOCATION, 4096,
            FLASH_1_PATTERN)))
            {
                MAP_FlashCtl_initiateSectorErase(FLASH_ERASE_LOCATION);
                currentTry++;
            } else
            {
                operationCompleted = true;
            }
        }
        /* If we have already reached the maximum number of retries, there
         * is an error and stall within an infinite loop
         */
        else
        {
            P1OUT |= BIT0;
            while (1)
                ;
        }

        if (operationCompleted)
        {
            /* Going into an infinite LED toggle loop to signify erase
             * has completed
             */
            while (1)
            {
                for (ii = 0; ii < 10000; ii++)
                    ;
                P1OUT ^= BIT0;
            }

        }

        PCM_gotoLPM0InterruptSafe();
    }
}

void FLCTL_IRQHandler(void)
{
    uint32_t status;

    status = MAP_FlashCtl_getEnabledInterruptStatus();
    MAP_FlashCtl_clearInterruptFlag(status);

    if (status & FLASH_ERASE_COMPLETE)
    {
        __no_operation();
    }
}
