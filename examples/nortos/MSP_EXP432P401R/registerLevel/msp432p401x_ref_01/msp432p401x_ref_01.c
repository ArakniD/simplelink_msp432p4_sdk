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
//   MSP432P401 Demo - Output reference module voltage to a port pin
//
//   Description: Configure and enable the reference module. Output the
//   reference voltage to a port pin. Cycle through the available voltage
//   levels, which can be observed on the oscilloscope/meter at port pin P5.6.
//
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |             P5.6|-->VREF
//            |             P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

#define DELAY_VALUE        400000

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    P1->DIR |= BIT0;                        // P1.0 set as output

    // Configure P5.6 to its analog function to output VREF
    P5->SEL0 |= BIT6;
    P5->SEL1 |= BIT6;

    REF_A->CTL0 |= REF_A_CTL0_ON;           // Turn on reference module
    REF_A->CTL0 |= REF_A_CTL0_OUT;          // Output reference voltage to a pin

    while (1)
    {
        // Output VREF = 1.2V
        REF_A->CTL0 &= ~(REF_A_CTL0_VSEL_3); // Clear existing VREF voltage level setting
        REF_A->CTL0 |= REF_A_CTL0_VSEL_0;    // Set VREF = 1.2V
        while (REF_A->CTL0 & REF_A_CTL0_GENBUSY);       // Wait until the reference generation is settled
        P1->OUT ^= BIT0;                     // Toggle P1.0 LED indicator

        // Arbitrary delay to observe the voltage on a multimeter
        for (i = DELAY_VALUE; i > 0; i--);

        // Output VREF = 1.45V
        REF_A->CTL0 &= ~(REF_A_CTL0_VSEL_3); // Clear existing VREF voltage level setting
        REF_A->CTL0 |= REF_A_CTL0_VSEL_1;    // Set VREF = 1.45V
        while (REF_A->CTL0 & REF_A_CTL0_GENBUSY);       // Wait until the reference generation is settled
        P1->OUT ^= BIT0;                     // Toggle P1.0 LED indicator

        // Arbitrary delay to observe the voltage on a multimeter
        for (i = DELAY_VALUE; i > 0; i--);

        // Output VREF = 2.5V
        REF_A->CTL0 |= REF_A_CTL0_VSEL_3;    // Set VREF = 2.5V
        while (REF_A->CTL0 & REF_A_CTL0_GENBUSY);       // Wait until the reference generation is settled
        P1->OUT ^= BIT0;                     // Toggle P1.0 LED indicator

        // Arbitrary delay to observe the voltage on a multimeter
        for (i = DELAY_VALUE; i > 0; i--);
    }
}

