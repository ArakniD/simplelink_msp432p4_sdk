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
 * MSP432 Flash Controller A - Mass Erase
 *
 * Description: This example shows the functionality of the mass erase API in
 * DriverLib. At the start of the example, memory addresses 0x1FE000 - 0x1FFFFF
 * are unprotected. As the flash sector size of MSP432P4111 is 4K, this
 * corresponds to the last two sectors of the device. Both sectors are then
 * programmed with a checkerboard pattern of 0xA5. After programmer, the last
 * sector is protected and a mass erase is performed. The user can then inspect
 * the memory in the debug view to observe the behavior of a mass erase. The 
 * mass erase causes the second to last sector to be erased (set to 0xFFs) while
 * keeping the protected sector intact.
 *
 *                MSP432P4111
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Statics */
uint8_t patternArray[8192];

int main(void)
{
    /* Since this program has a huge buffer that simulates the calibration data,
     * halting the watch dog is done in the reset ISR to avoid a watchdog 
     * timeout during the zero 
     */

    /* Setting our MCLK to 48MHz for faster programming */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_A_setWaitState(FLASH_A_BANK0, 6);
    MAP_FlashCtl_A_setWaitState(FLASH_A_BANK1, 6);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    /* Initializing our buffer to a pattern of 0xA5 */
    memset(patternArray, 0xA5, 8192);
    
    //![Simple Flash_A Config]
    /* Unprotecting the last to sectors (0x1FE000 - 0x1FFFFF) */
    MAP_FlashCtl_A_unprotectMemory(0x1FE000, 0x1FFFFF);

    /* Trying a mass erase. Since we unprotected the last two sectors, 
     * this should erase these sectors. If it fails, we trap inside an 
     * infinite loop.
     */
    if(!MAP_FlashCtl_A_performMassErase())
        while(1);
    //![Simple Flash_A Config]
    
    /* Trying to program the filler data. If it fails, trap inside an infinite
       loop */
    if(!MAP_FlashCtl_A_programMemory (patternArray, (void*) 0x1FE000, 8192))
        while(1);

    /* Setting the last sector as protected */
    MAP_FlashCtl_A_protectMemory(0x1FFFFF, 0x1FFFFF);

    /* Performing the mass erase again. Now, since we protected the last sector,
     *  only 0x1FE000 - 0x1FEFFF should be erased. Set a breakpoint after this 
     *  call to observe the memory in the debugger.
     */
    if(!MAP_FlashCtl_A_performMassErase())
        while(1);

    /* Deep Sleeping when not in use */
    while (1)
    {
        MAP_PCM_gotoLPM3();
    }
}
