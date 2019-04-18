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
 * MSP432 Flash Controller A - Programming Calibration Data
 *
 * Description: This example shows the use of the the Flash Controller APIs
 * to erase and program simulated calibration data to a specific area in memory.
 * Data in this example is programmed to user area of memory. The "fake"
 * calibration data is stored in a RAM array and set using the memset function,
 * however in a real application this buffer would be filled out using a serial
 * interface such as I2C.
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

#define CALIBRATION_START 0x1FE000

/* Statics */
uint8_t simulatedCalibrationData[4096];

int main(void)
{
    /* Halting the WDT */
    WDT_A_holdTimer();
    
    /* Setting our MCLK to 48MHz for faster programming */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    FlashCtl_A_setWaitState(FLASH_A_BANK0, 6);
    FlashCtl_A_setWaitState(FLASH_A_BANK1, 6);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    /* Initializing our buffer to a pattern of 0xA5 */
    memset(simulatedCalibrationData, 0xA5, 4096);
    
    //![FlashCtl_A Program]
    /* Unprotecting 0x1FE000  */
    MAP_FlashCtl_A_unprotectMemory(CALIBRATION_START,CALIBRATION_START);

    /* Trying to erase the sector. Within this function, the API will
        automatically try to erase the maximum number of tries. If it fails,
         trap in an infinite loop */
    if(!MAP_FlashCtl_A_eraseSector(CALIBRATION_START))
    {
        while(1);
    }

    /* Trying to program the memory. Within this function, the API will 
        automatically try to program the maximum number of tries. If it fails,
        trap inside an infinite loop */
    if(!MAP_FlashCtl_A_programMemory(simulatedCalibrationData,
            (void*) CALIBRATION_START, 4096))
    {
        while(1);
    }

    /* Setting the sector back to protected  */
    MAP_FlashCtl_A_protectMemory(CALIBRATION_START,CALIBRATION_START);
    //![FlashCtl_A Program]

    /* Going to LPM3 when not in use */
    while (1)
    {
        MAP_PCM_gotoLPM3();
    }
}

