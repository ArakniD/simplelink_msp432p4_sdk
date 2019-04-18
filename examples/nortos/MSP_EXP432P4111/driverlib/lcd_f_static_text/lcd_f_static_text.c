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
 * MSP432 LCD_F Static Text Display
 *
 * Description: In the code example the LCD_F module is configured to output
 * a static text value of "123ABC". The LCD_F is configured in standard mode
 * and pin assignments are made that map the connected segment/COM pins to the
 * LCD_F registers. A basic map of the pin assignments that generate an
 * elementary alphabet are also provided and a convenience function that
 * displays a character at a position on the LCD is provided.
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

/* Definitions for LCD Driver */
#define char1 16    // Digit A1 - L16
#define char2 32    // Digit A2 - L32
#define char3 40    // Digit A3 - L40
#define char4 36    // Digit A4 - L36
#define char5 28    // Digit A5 - L28
#define char6 44    // Digit A6 - L44

/* LCD memory map for numeric digits (Byte Access) */
const char digit[10][4] =
{
    {0xC, 0xF, 0x8, 0x2},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x0, 0x6, 0x0, 0x2},  /* "1" */
    {0xB, 0xD, 0x0, 0x0},  /* "2" */
    {0x3, 0xF, 0x0, 0x0},  /* "3" */
    {0x7, 0x6, 0x0, 0x0},  /* "4" */
    {0x7, 0xB, 0x0, 0x0},  /* "5" */
    {0xF, 0xB, 0x0, 0x0},  /* "6" */
    {0x4, 0xE, 0x0, 0x0},  /* "7" */
    {0xF, 0xF, 0x0, 0x0},  /* "8" */
    {0x7, 0xF, 0x0, 0x0}   /* "9" */
};

/* LCD memory map for uppercase letters (Byte Access) */
const char alphabetBig[26][4] =
{
    {0xF, 0xE, 0x0, 0x0},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0x1, 0xF, 0x0, 0x5},  /* "B" */
    {0xC, 0x9, 0x0, 0x0},  /* "C" */
    {0x0, 0xF, 0x0, 0x5},  /* "D" */
    {0xF, 0x9, 0x0, 0x0},  /* "E" */
    {0xF, 0x8, 0x0, 0x0},  /* "F" */
    {0xD, 0xB, 0x0, 0x0},  /* "G" */
    {0xF, 0x6, 0x0, 0x0},  /* "H" */
    {0x0, 0x9, 0x0, 0x5},  /* "I" */
    {0x8, 0x7, 0x0, 0x0},  /* "J" */
    {0xE, 0x0, 0x2, 0x2},  /* "K" */
    {0xC, 0x1, 0x0, 0x0},  /* "L" */
    {0xC, 0x6, 0x0, 0xA},  /* "M" */
    {0xC, 0x6, 0x2, 0x8},  /* "N" */
    {0xC, 0xF, 0x0, 0x0},  /* "O" */
    {0xF, 0xC, 0x0, 0x0},  /* "P" */
    {0xC, 0xF, 0x2, 0x0},  /* "Q" */
    {0xF, 0xC, 0x2, 0x0},  /* "R" */
    {0x7, 0xB, 0x0, 0x0},  /* "S" */
    {0x0, 0x8, 0x0, 0x5},  /* "T" */
    {0xC, 0x7, 0x0, 0x0},  /* "U" */
    {0xC, 0x0, 0x8, 0x2},  /* "V" */
    {0xC, 0x6, 0xA, 0x0},  /* "W" */
    {0x0, 0x0, 0xA, 0xA},  /* "X" */
    {0x0, 0x0, 0x0, 0xB},  /* "Y" */
    {0x0, 0x9, 0x8, 0x2}   /* "Z" */
};

static void showChar(char c, int position);

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

    /* Turing the LCD_F module on */
    LCD_F_turnOn();

    /* 123ABC */
    showChar('1', char1);
    showChar('2', char2);
    showChar('3', char3);
    showChar('A', char4);
    showChar('B', char5);
    showChar('C', char6);

    while(1)
    {
        PCM_gotoLPM0();
    }
}

static void showChar(char c, int position)
{
    uint8_t ii;
    if (c == ' ')
    {
        for (ii=0; ii<4; ii++)
        {
            LCD_F->M[position+ii] |= 0x00;
        }
    }
    else if (c >= '0' && c <= '9')
    {
        for (ii=0; ii<4; ii++)
        {
            LCD_F->M[position+ii] |= digit[c-48][ii];
        }
    }
    else if (c >= 'A' && c <= 'Z')
    {
        for (ii=0; ii<4; ii++)
        {
            LCD_F->M[position+ii] |= alphabetBig[c-65][ii];
        }
    } else
    {
        LCD_F->M[position] = 0xFF;
    }
}
