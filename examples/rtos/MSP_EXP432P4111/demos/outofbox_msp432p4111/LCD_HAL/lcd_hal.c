/*
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
 */

/*
 *  ======== lcd_hal.c ========
 *  This LCD HAL file is specific to the MSP-EXP432P4111 LaunchPad. The LCD
 *  pin connections were chosen to optimize software ease of use and to simplify
 *  board layout.
 */

/* Includes */
#include <string.h>
#include <unistd.h>

/* DriverLib Header Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>

/* Example/Board Header Files */
#include <ti/drivers/Board.h>

/* Local Header Files */
#include "lcd_hal.h"
#include "callbacks.h"

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
const char alphabetUppercase[26][4] =
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
    {0x1, 0xB, 0x0, 0x8},  /* "S" */
    {0x0, 0x8, 0x0, 0x5},  /* "T" */
    {0xC, 0x7, 0x0, 0x0},  /* "U" */
    {0xC, 0x0, 0x8, 0x2},  /* "V" */
    {0xC, 0x6, 0xA, 0x0},  /* "W" */
    {0x0, 0x0, 0xA, 0xA},  /* "X" */
    {0x0, 0x0, 0x0, 0xB},  /* "Y" */
    {0x0, 0x9, 0x8, 0x2}   /* "Z" */
};

const char lcdPositions[6] = {
    LCD_POSITION_1,
    LCD_POSITION_2,
    LCD_POSITION_3,
    LCD_POSITION_4,
    LCD_POSITION_5,
    LCD_POSITION_6
};

/* Configuration Structure for LCD */
const LCD_F_Config lcdConf =
{
    .clockSource = LCD_F_CLOCKSOURCE_ACLK,
    .clockDivider = LCD_F_CLOCKDIVIDER_32,
    .clockPrescaler = LCD_F_CLOCKPRESCALER_1,
    .muxRate = LCD_F_4_MUX,
    .waveforms = LCD_F_STANDARD_WAVEFORMS,
    .segments = LCD_F_SEGMENTS_ENABLED
};

/*
 * Initializes LCD
 */
void LCD_init()
{
    /* Configure GPIO pins to LCD peripheral function */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,
                                                GPIO_PIN1 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7,
                                                GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6,
                                                GPIO_PIN2 | GPIO_PIN3,
                                                GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7,
                                                GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7,
                                                GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P8,
                                                GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7,
                                                GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9,
                                                GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7,
                                                GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P10,
                                                GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6,
                                                GPIO_SECONDARY_MODULE_FUNCTION);

    /* Initializing the LCD_F module */
    LCD_F_initModule((LCD_F_Config*)&lcdConf);

    /* Clearing out all main, blink, and animation memory */
    LCD_F_clearAllMemory();
    LCD_F_clearAllBlinkingMemory();
    LCD_F_clearAllAnimationMemory();

    /* Initializing all LCD pins */
    LCD_F_setPinsAsLCDFunction(LCD_F_SEGMENT_LINE_0, LCD_F_SEGMENT_LINE_3);
    LCD_F_setPinAsLCDFunction(LCD_F_SEGMENT_LINE_6);
    LCD_F_setPinsAsLCDFunction(LCD_F_SEGMENT_LINE_16, LCD_F_SEGMENT_LINE_19);
    LCD_F_setPinsAsLCDFunction(LCD_F_SEGMENT_LINE_26, LCD_F_SEGMENT_LINE_47);

    /* Configure COM pins */
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_26, LCD_F_MEMORY_COM0);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_27, LCD_F_MEMORY_COM1);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_6, LCD_F_MEMORY_COM2);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_3, LCD_F_MEMORY_COM3);

    /* Turning the LCD_F module on */
    LCD_F_turnOn();
}

/*
 * Clears LCD main, blink, animation memory
 */
void LCD_clear()
{
    /* Turn off LCD before clearing all memory to avoid unpredictable behavior */
    LCD_F_turnOff();

    /* Clear all LCD main and blink memory */
    LCD_F_clearAllMemory();
    LCD_F_clearAllBlinkingMemory();
    LCD_F_clearAllAnimationMemory();

    /* Reconfigure COM pins */
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_26, LCD_F_MEMORY_COM0);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_27, LCD_F_MEMORY_COM1);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_6, LCD_F_MEMORY_COM2);
    LCD_F_setPinAsCOM(LCD_F_SEGMENT_LINE_3, LCD_F_MEMORY_COM3);

    /* Turn LCD back on after clearing memory and reconfiguring COM pins */
    LCD_F_turnOn();
}

/*
 * Displays a character at the specified position on the LCD
 */
void LCD_showChar(char character, int position)
{
    int i;
    if (character >= '0' && character <= '9')
    {
        /* character is 0-9 */
        for (i=0; i<4; i++)
        {
            if (i == 2)
            {
                /* Retain Antenna/TX/RX symbol memory bits */
                if (position == LCD_POSITION_3)
                    LCD_F->M[position+i] = digit[character-48][i] | (LCD_F->M[position+i] & 0x04);
                else if (position == LCD_POSITION_6)
                    LCD_F->M[position+i] = digit[character-48][i] | (LCD_F->M[position+i] & 0x05);
                else
                    LCD_F->M[position+i] = digit[character-48][i];
            }
            else
                LCD_F->M[position+i] = digit[character-48][i];
        }
    }
    else if (character >= 'A' && character <= 'Z')
    {
        /* character is upper-case A-Z */
        for (i=0; i<4; i++)
        {
            if (i == 2)
            {
                /* Retain Antenna/TX/RX symbol memory bits */
                if (position == LCD_POSITION_3)
                    LCD_F->M[position+i] = alphabetUppercase[character-65][i] | (LCD_F->M[position+i] & 0x04);
                else if (position == LCD_POSITION_6)
                    LCD_F->M[position+i] = alphabetUppercase[character-65][i] | (LCD_F->M[position+i] & 0x05);
                else
                    LCD_F->M[position+i] = alphabetUppercase[character-65][i];
            }
            else
                LCD_F->M[position+i] = alphabetUppercase[character-65][i];
        }
    }
    else
    {
        /* Clear all segments at the LCD position */
        for (i=0; i<4; i++)
        {
            if (i == 2)
            {
                /* Retain Antenna/TX/RX symbol memory bits */
                if (position == LCD_POSITION_3)
                    LCD_F->M[position+i] = LCD_F->M[position+i] & 0x04;
                else if (position == LCD_POSITION_6)
                    LCD_F->M[position+i] = LCD_F->M[position+i] & 0x05;
                else
                    LCD_F->M[position+i] = 0x00;
            }
            else
                LCD_F->M[position+i] = 0x00;
        }
    }
}

/*
 * Displays a string on the LCD. Left-justified
 */
void LCD_showString(char *string)
{
    int length = strlen(string);
    int i;

    /* */
    for (i=0; i<6; i++)
    {
        if (i < length)
            LCD_showChar(string[i], lcdPositions[i]);
        else
            LCD_showChar(' ', lcdPositions[i]);
    }
}

/*
 * Scrolls a string from right to left on the LCD (Marquee)
 */
void LCD_scrollString(char *string, int delay)
{
    int length = strlen(string);
    int i;
    int s = 5;
    char buffer[6] = "      ";
    for (i=0; i<length+7; i++)
    {
        if (scrolling == false)
            break;
        int j;
        for (j=0; j<6;j++)
            buffer[j] = ' ';
        for (j=0; j<length; j++)
        {
            if (((s+j) >= 0) && ((s+j) < 6))
                buffer[s+j] = string[j];
        }
        s--;
        for (j=0; j<6;j++)
            LCD_showChar(buffer[j], lcdPositions[j]);

        /* delay microseconds between scrolling frames */
        usleep(delay);
    }
}

/*
 * Shows temperature in degree Celsius on LCD
 */
void LCD_showTemperatureC(float temperature)
{
    LCD_showString("      ");

    char neg = 0x00;
    if (temperature < 0)
    {
        temperature *= -1;
        neg = 0x04;
    }

    // Handles displaying numbers ranging from -999.99 to 999.99
    if (temperature>=100)
        LCD_showChar((int)(temperature/100)%10 + '0', LCD_POSITION_1);
    if (temperature>=10)
        LCD_showChar((int)(temperature/10)%10 + '0', LCD_POSITION_2);
    if (temperature>=1)
        LCD_showChar((int)(temperature/1)%10 + '0', LCD_POSITION_3);
    else
        LCD_showChar('0', LCD_POSITION_3);
    if (temperature>=0.1)
        LCD_showChar((int)(temperature*10)%10 + '0', LCD_POSITION_4);
    if (temperature>=0.01)
        LCD_showChar((int)(temperature*100)%10 + '0', LCD_POSITION_5);

    /* Show negative sign */
    LCD_F->M[LCD_POSITION_1+2] |= neg;

    /* Show decimal point */
    LCD_F->M[LCD_POSITION_3+2] |= 0x01;

    /* Show degree symbol */
    LCD_F->M[LCD_POSITION_5+2] |= 0x04;

    /* Show Celsius unit */
    LCD_showChar('C', LCD_POSITION_6);
}

/*
 * Starts LCD animation
 */
void LCD_startAnimation(void)
{
    /* Configuring and enabling animation */
    LCD_F_setAnimationControl(LCD_F_ANIMATION_FREQ_CLOCK_PRESCALAR_512,
            LCD_F_ANIMATION_FREQ_CLOCK_DIVIDER_8,
            LCD_F_ANIMATION_FRAMES_T0_TO_T7);

    /* Enable LCD_F Animation feature */
    LCD_F_enableAnimation();
}

/*
 * Stops LCD animation
 */
void LCD_stopAnimation(void)
{
    LCD_F_disableAnimation();
}

/*
 * Updates the specified LCD main memory register using the byte mask
 * and the specified bitwise operation
 */
void LCD_updateMemory(int lcdMemory, char byte, char operation)
{
    switch (operation) {
        case SET:
            LCD_F->M[lcdMemory] |= byte;
            break;
        case CLEAR:
            LCD_F->M[lcdMemory] &= ~byte;
            break;
        case TOGGLE:
            LCD_F->M[lcdMemory] ^= byte;
            break;
        default:
            break;
    }
}

/*
 * Updates the specified LCD Animation memory register using the byte mask
 * and the specified bitwise operation
 */
void LCD_updateAnimationMemory(int lcdMemory, char byte, char operation)
{
    switch (operation) {
        case SET:
            LCD_F->ANM[lcdMemory] |= byte;
            break;
        case CLEAR:
            LCD_F->ANM[lcdMemory] &= ~byte;
            break;
        case TOGGLE:
            LCD_F->ANM[lcdMemory] ^= byte;
            break;
        default:
            break;
    }
}
