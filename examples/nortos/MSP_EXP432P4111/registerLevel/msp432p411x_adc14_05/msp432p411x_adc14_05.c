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
//  MSP432P411 Demo - ADC14, Using an External Reference
//
//  Description: This example shows how to use an external positive reference for
//  the ADC14.The external reference is applied to the VeREF+ pin. VeREF- is used
//  for the negative reference (use an external jumper to GND). A single
//  conversion is performed on channel A0. The conversion results are stored in
//  ADC14->MEM[0]. Test by applying a voltage to channel A0, then setting and
//  running to a break point at the "_NOP()" instruction. To view the conversion
//  results, open an SFR window in debugger and view the contents of
//  ADC14->MEM[0] or from the variable ADCvar.
//  NOTE: VeREF+ range: 1.4V (min) to AVCC (max)
//        VeREF- range: 0V (min) to 1.2V (max)
//        Differential ref voltage range: 1.4V(min) to AVCC(max)
//          (see datasheet for device specific information)
//
//                MSP432P411
//             -------------------
//         /|\|                   |
//          | |                   |
//          --|RST                |
//            |                   |
//     Vin -->|P5.5/A0            |
//            |                   |
//    REF+ -->|P5.6/VREF+/VeREF+  |
//            |                   |
//    REF-|-->|P5.7/VREF-/VeREF-  |
//        |   |                   |
//        |---|AVss               |
//            |                   |
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | June 2014 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

volatile uint16_t ADCvar;

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                 WDT_A_CTL_HOLD;

    // Configure GPIO
    P5->SEL1 |= BIT5 | BIT6 | BIT7;         // Enable A/D channel A0
    P5->SEL0 |= BIT5 | BIT6 | BIT7;         // and VeREF+ and VeREF-

    // Configure ADC14
    // Turn on ADC14, set sampling time
    ADC14->CTL0 = ADC14_CTL0_ON | ADC14_CTL0_SHP| ADC14_CTL0_SHT0_2;
    // Use sampling timer, 14-bit conversion results
    ADC14->CTL1 = ADC14_CTL1_RES_3;
    // Vr+ = VeREF+ (ext) and Vr-=VeREF-, A0
    ADC14->MCTL[0] = ADC14_MCTLN_VRSEL_14 | ADC14_MCTLN_INCH_0;
    // Enable conversions
    ADC14->CTL0 |= ADC14_CTL0_ENC;

    while (1)
    {
        ADC14->CTL0 |= ADC14_CTL0_SC;       // Start conversion-software trigger
        while (!(ADC14->IFGR0 & BIT0));
        ADCvar = ADC14->MEM[0];             // Read conversion result
        __no_operation();                   // SET BREAKPOINT HERE
    }
}
