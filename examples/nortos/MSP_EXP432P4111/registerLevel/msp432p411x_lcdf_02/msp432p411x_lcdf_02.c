/* --COPYRIGHT--,BSD_EX
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
 *
 *******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see http://www.ti.com/tool/mspdriverlib for an API functional
 * library & https://dev.ti.com/pinmux/ for a GUI approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP432P4111 Demo - LCD_F, Statically display "MSP432", blink the top row LCD
//                     symbols, and show battery charging animation.
//
//               Note: This example works on the MSP-EXP432P4111 LaunchPad
//                     LCD_F's animation feature is only supported on L0 and L1,
//                     which are connected to the battery segments on the LaunchPad
//
//  Description: Displays "MSP432" in sequence to the LCD display.
//                f(LCD) = 32768Hz/((15+1)*4) = 512Hz
//                f(BLINK) = 32768Hz/((7+1)*4096) = 1Hz
//                f(ANIMATION) = 32768Hz/((7+1)*512) = 8Hz
//                ACLK = default REFO ~32768Hz,
//                MCLK = SMCLK = default DCODIV ~3MHz.
//
//                MSP432P4111
//             -----------------
//         /|\|                 |
//          | |              XIN|--
//          --|RST              |  ~32768Hz
//            |             XOUT|--
//            |R03              |
//            |R13      (L3)COM3|----------------|
//            |R23      (L6)COM2|---------------||
//            |        (L27)COM1|--------------|||
//            |        (L26)COM0|-------------||||
//            |                 |    -------------
//            |            L0~L2|---| 1 2 3 4 5 6 |
//            |          L16-L19|    -------------
//            |          L28-L47|       TI LCD
//            |                 |
//
//  LCD pin - Port Pin Map
//  LCD pin    P4111_LineX
//    1         L16 (P9.1)
//    2         L17 (P9.0)
//    3         L18 (P8.7)
//    4         L19 (P8.6)
//    5         L32 (P9.3)
//    6         L33 (P9.2)
//    7         L34 (P10.5)
//    8         L35 (P10.4)
//    9         L40 (P9.7)
//    10        L41 (P9.6)
//    11        L42 (P9.5)
//    12        L43 (P9.4)
//    13        L36 (P10.3)
//    14        L37 (P10.2)
//    15        L38 (P10.1)
//    16        L39 (P10.0)
//    17        L28 (P7.7)
//    18        L29 (P7.6)
//    19        L30 (P7.5)
//    20        L31 (P7.4)
//    21        L3  (P3.4)
//    22        L6  (P3.1)
//    23        L27 (P6.2)
//    24        L26 (P6.3)
//    25        -
//    26        -
//    27        -
//    28        -
//    29        -
//    30        -
//    31        -
//    32        L2  (P3.5)
//    33        L0  (P3.7)
//    34        L1  (P3.6)
//    35        L44 (P8.5)
//    36        L45 (P8.4)
//    37        L46 (P8.3)
//    38        L47 (P8.2)
//
//   Eric Chen
//   Texas Instruments Inc.
//   August 2017 (updated) | August 2017 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"

#define char1 16    // Digit A1 - L16
#define char2 32    // Digit A2 - L32
#define char3 40    // Digit A3 - L40
#define char4 36    // Digit A4 - L36
#define char5 28    // Digit A5 - L28
#define char6 44    // Digit A6 - L44

// LCD memory map for numeric digits (Byte Access)
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

// LCD memory map for uppercase letters (Byte Access)
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

void showChar(char, int);

int main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;           // Stop watchdog timer

    // Select LCD function on port pins being used for LCD
    P3->SEL1 |= 0xF2;
    P6->SEL1 |= 0x0C;
    P7->SEL1 |= 0xF0;
    P8->SEL1 |= 0xFC;
    P9->SEL1 |= 0xFF;
    P10->SEL1 |= 0x3F;

    // Unlock CS module for register access
    CS->KEY = CS_KEY_VAL;
    CS->CTL1 = CS_CTL1_SELA__REFOCLK | CS_CTL1_SELS__DCOCLK | CS_CTL1_SELM__DCOCLK;

    // Configure LCD_F Port Control Register to enable each LCD pin that is in use
    LCD_F->PCTL0 |= 0xFC0F004F;   // L0-L3, L6, L16-L19, L26-L31
    LCD_F->PCTL1 |= 0x0000FFFF;   // L32-L47

    // Configure LCD_F Control Register. ACLK = LCD clock source, Divider = 16, Pre-scaler = 4, 4-mux mode
    LCD_F->CTL |= LCD_F_CTL_SSEL_0 | LCD_F_CTL_DIV_15 | LCD_F_CTL_PRE_2 | LCD_F_CTL_MX_3 | LCD_F_CTL_SON;

    // Configure Blinking and Memory Control Register. Clear all blinking and main memory, Blinking frequency = 1Hz.
    LCD_F->BMCTL |= LCD_F_BMCTL_CLRBM | LCD_F_BMCTL_CLRM | LCD_F_BMCTL_BLKDIV_7 | LCD_F_BMCTL_BLKPRE_3;
    LCD_F->BMCTL |= LCD_F_BMCTL_BLKMOD_1;    // Enable Blinking Mode of segments selected in both LCDMx & LCDBMx.

    // Configure COM pins
    LCD_F->CSSEL0 |= 0x0C000048;  // Select Pins L3, L6, L26, L27 as common lines
    LCD_F->M[26] = 0x01;          // COM0
    LCD_F->M[27] = 0x02;          // COM1
    LCD_F->M[6] = 0x04;           // COM2
    LCD_F->M[3] = 0x08;           // COM3

    // Set Main memory for characters A1 - A6 to display "MSP432"
    showChar('M', char1);
    showChar('S', char2);
    showChar('P', char3);
    showChar('4', char4);
    showChar('3', char5);
    showChar('2', char6);

    // Set both Main and Blinking Memory for symbols
    LCD_F->M[2] |= 0x0F;
    LCD_F->M[42] |= 0x04;
    LCD_F->M[46] |= 0x05;
    LCD_F->BM[2] |= 0x0F;
    LCD_F->BM[42] |= 0x04;
    LCD_F->BM[46] |= 0x05;

    // Configure Animation Control Register. Animation frequency = 8Hz, Use all 8 frames.
    LCD_F->ANMCTL |= LCD_F_ANMCTL_ANMDIV_7 | LCD_F_ANMCTL_ANMSTP_7;
    LCD_F->ANMCTL |= LCD_F_ANMCTL_ANMEN;    // Enable LCD Animation

    // Note - Animation is only supported on L0 and L1 LCD pins
    // L0 and L1 are connected to the battery segments on the MSP-EXP432P4111 LaunchPad

    // Set Battery animation frames
    // Control Bits | 7  | 6  | 5  |  4   | 3  | 2  | 1  | 0  |
    // Segments     | B6 | B4 | B2 | BATT | B5 | B3 | B1 | [] |
    LCD_F->ANM[0] = 0x11;    // Frame 1  0b00010001  [      ]-
    LCD_F->ANM[1] = 0x11;    // Frame 2  0b00010001  [      ]-
    LCD_F->ANM[2] = 0x31;    // Frame 3  0b00110001  [|     ]-
    LCD_F->ANM[3] = 0x33;    // Frame 4  0b00110011  [||    ]-
    LCD_F->ANM[4] = 0x73;    // Frame 5  0b01110011  [|||   ]-
    LCD_F->ANM[5] = 0x77;    // Frame 6  0b01110111  [||||  ]-
    LCD_F->ANM[6] = 0xF7;    // Frame 7  0b11110111  [||||| ]-
    LCD_F->ANM[7] = 0xFF;    // Frame 8  0b11111111  [||||||]-

    LCD_F->CTL |= LCD_F_CTL_ON;   // Turn on LCD

    __sleep();
    __no_operation();
}

/*
 * Display alphanumeric character on the specified position on the LCD
 * Sets the Main LCD Memory
 */
void showChar(char c, int position)
{
    int i;
    if (c == ' ')
    {
        for (i=0; i<4; i++)
        {
            LCD_F->M[position+i] = 0x00;
        }
    }
    else if (c >= '0' && c <= '9')
    {
        for (i=0; i<4; i++)
        {
            LCD_F->M[position+i] = digit[c-48][i];
        }
    }
    else if (c >= 'A' && c <= 'Z')
    {
        for (i=0; i<4; i++)
        {
            LCD_F->M[position+i] = alphabetBig[c-65][i];
        }
    }
    else
    {
        LCD_F->M[position] = 0x00;
    }
}

