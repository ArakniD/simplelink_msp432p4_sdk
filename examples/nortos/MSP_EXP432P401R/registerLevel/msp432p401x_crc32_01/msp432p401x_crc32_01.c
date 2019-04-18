/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//   MSP432P401 Demo - CRC32 - 16bit, Compare CRC output with software-based
//                     algorithm
//
//   Description: An array of 16 random 16-bit values are moved through the CRC
//   module, as well as a software-based CRC-CCIT-BR algorithm. Due to the fact
//   that the software-based algorithm handles 8-bit inputs only, the 16-bit
//   words are broken into 2 8-bit words before being run through (lower byte
//   first). The outputs of both methods are then compared to ensure that the
//   operation of the CRC module is consistent with the expected outcome. If
//   the values of each output are equal, set P1.0, else reset.
//
//   MCLK = SMCLK = default DCO~1MHz
//
//           MSP432P401x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|---> LED
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | Jun 2014 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

const uint16_t CRC16_Init = 0xFFFF;

const uint16_t CRC16_Input[] = {
  0x0fc0, 0x1096, 0x5042, 0x0010,           // 16 random 16-bit numbers
  0x7ff7, 0xf86a, 0xb58e, 0x7651,           // these numbers can be
  0x8b88, 0x0679, 0x0123, 0x9599,           // modified if desired
  0xc58c, 0xd1e2, 0xe144, 0xb691
};

uint16_t CRC16_Result;                      // Holds results obtained through the CRC16 module
uint16_t SW_Results;                        // Holds results obtained through SW

// Software Algorithm Function Declaration
uint16_t CCITT_Update(uint16_t, uint16_t);

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                 WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->OUT &= ~BIT0;                       // Clear LED to start
    P1->DIR |= BIT0;                        // P1.0 Output

    // First, use the CRC16 hardware module to calculate the CRC...
    CRC32->INIRES16 = CRC16_Init;           // Init CRC16 HW module

    for(i = 0; i < 16; i++)
    {
        // Input random values into CRC Hardware
        CRC32->DIRB16 = CRC16_Input[i];     // Input data in CRC
    }
    CRC16_Result = CRC32->INIRES16;         // Save results (per CRC-CCITT standard)

    // Now use a software routine to calculate the CRC...
    SW_Results = CRC16_Init;                // Init SW CRC
    for(i = 0; i < 16; i++)
    {
        // Then input upper byte
        SW_Results = CCITT_Update(SW_Results, (CRC16_Input[i] >> 8) & 0xFF);

        // First input lower byte
        SW_Results = CCITT_Update(SW_Results, CRC16_Input[i] & 0xFF);
    }

    // Compare data output results
    if(CRC16_Result==SW_Results)            // if data is equal
        P1->OUT |= BIT0;                    // set the LED
    else
        P1->OUT &= ~BIT0;                   // if not, clear LED

    while(1);                               // infinite loop
}

// Software algorithm - CCITT CRC16 code
uint16_t CCITT_Update(uint16_t init, uint16_t input)
{
    uint16_t CCITT = (uint8_t) (init >> 8) | (init << 8);
    CCITT ^= input;
    CCITT ^= (uint8_t) (CCITT & 0xFF) >> 4;
    CCITT ^= (CCITT << 8) << 4;
    CCITT ^= ((CCITT & 0xFF) << 4) << 1;
    return CCITT;
}

