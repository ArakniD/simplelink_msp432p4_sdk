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
/******************************************************************************
 * MSP432 LCD_F Animated Text
 *
 * Description: In the code example the LCD_F module is configured to output
 * a basic animation using the "animated memory" of the LCD_F module. This
 * code example uses the built-in LCD on the MSP432P4111 LaunchPad. Specifically
 * the battery segments of the LCD are animated in sequence to give the
 * appearance of charging.
 *
 *             -----------------
 *            |         (L3)COM3|----------------|
 *            |         (L6)COM2|---------------||
 *            |        (L27)COM1|--------------|||
 *            |        (L26)COM0|-------------||||
 *            |                 |    -------------
 *            |        SEG LINES|---| 1 2 3 4 5 6 |
 *            |                 |    -------------
 *            |                 |       TI LCD
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Configuration Structure for LCD */
LCD_F_Config lcdConf =
{
    .clockSource = LCD_F_CLOCKSOURCE_ACLK,
    .clockDivider = LCD_F_CLOCKDIVIDER_32,
    .clockPrescaler = LCD_F_CLOCKPRESCALER_1,
    .muxRate = LCD_F_4_MUX,
    .waveforms = LCD_F_STANDARD_WAVEFORMS,
    .segments = LCD_F_SEGMENTS_ENABLED
};

int main(void)
{
    /* Stop Watchdog  */
    WDT_A_holdTimer();

    /* Initializing all of the function selection bits in the pins */
    P3->SEL1 |= 0xF2;
    P6->SEL1 |= 0x0C;
    P7->SEL1 |= 0xF0;
    P8->SEL1 |= 0xFC;
    P9->SEL1 |= 0xFF;
    P10->SEL1 |= 0x3F;

    /* Setting ACLK to the reference oscillator */
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initializing the LCD_F module */
    LCD_F_initModule(&lcdConf);

    /* Clearing out all memory */
    LCD_F_clearAllMemory();

    /* Initializing all of our pins and setting the relevant COM lines */
    LCD_F_setPinsAsLCDFunction(LCD_F_SEGMENT_LINE_0, LCD_F_SEGMENT_LINE_3);
    LCD_F_setPinAsLCDFunction(LCD_F_SEGMENT_LINE_6);
    LCD_F_setPinsAsLCDFunction(LCD_F_SEGMENT_LINE_16, LCD_F_SEGMENT_LINE_19);
    LCD_F_setPinsAsLCDFunction(LCD_F_SEGMENT_LINE_26, LCD_F_SEGMENT_LINE_47);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_26, LCD_F_MEMORY_COM0);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_27, LCD_F_MEMORY_COM1);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_6, LCD_F_MEMORY_COM2);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_3, LCD_F_MEMORY_COM3);

    /* Set battery animation frames */
    LCD_F->ANM[0] = 0x11;         /* Frame 1 */
    LCD_F->ANM[1] = 0x11;         /* Frame 2 */
    LCD_F->ANM[2] = 0x31;         /* Frame 3 */
    LCD_F->ANM[3] = 0x33;         /* Frame 4 */
    LCD_F->ANM[4] = 0x73;         /* Frame 5 */
    LCD_F->ANM[5] = 0x77;         /* Frame 6 */
    LCD_F->ANM[6] = 0xF7;         /* Frame 7 */
    LCD_F->ANM[7] = 0xFF;         /* Frame 8 */

    /* Configuring and enabling animation */
    LCD_F_setAnimationControl(LCD_F_ANIMATION_FREQ_CLOCK_PRESCALAR_512,
            LCD_F_ANIMATION_FREQ_CLOCK_DIVIDER_8,
            LCD_F_ANIMATION_FRAMES_T0_TO_T7);
    LCD_F_enableAnimation();

    /* Turing the LCD_F module on */
    LCD_F_turnOn();

    /* Going into LPM0 */
    PCM_gotoLPM0();
}
