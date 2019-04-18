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
//  MSP432P401 Demo- ADC14, Sample A2-A3 Differential,Set P1.0 if i/p> 1V
//
//   Description: A single sample is made on differential channel A2-A3
//   with reference to AVcc.
//   A resistor ladder is used to provide two different voltage inputs one at
//   A2 and the other at A3. Since equal resistors are used, the input voltage
//   at A2 and A3 are 2/3Vin and 1/3Vin respectively.
//   Vin is varied from 1V to 3V. When Vin = 3V, A2 = 2V and A3 = 1V providing
//   a differential voltage of 1V across the ADC input.
//   If A2-A3 >= 1V, P1.0 set, else reset. The full, correct handling of
//   and ADC14 interrupt is shown as well.
//
//
//                msp432p401
//  Vin= 1V-3V -----------------
//     |      |                 |
//    10K     |                 |
//     |      |                 |
//     |      |                 |
//     -------| A2=2/3Vin       |
//     |      |                 |
//     10K    |                 |
//     |      |                 |
//     -------| A3=1/3Vin       |
//     |      |                 |
//    10K     |             P1.0|-->LED
//     |      |                 |
//    GND
//
//   Dung Dang
//   Texas Instruments Inc.
//   October 2015 (updated) | November 2013 (created)
//   Built with Code Composer Studio V6.0
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include "stdint.h"

int main(void) {
    volatile unsigned int i;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;                 // Stop WDT

    // GPIO Setup
    P1->OUT &= ~BIT0;                           // Clear LED to start
    P1->DIR |= BIT0;                            // Set P1.0/LED to output
    P5->SEL1 |= BIT3 | BIT2;                    // Configure P5.3/2 (A2/3) for ADC
    P5->SEL0 |= BIT3 | BIT2;

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);         // Enable ADC interrupt in NVIC module

    // Configure ADC14
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;          // Sampling time, S&H=16, ADC14 on
    ADC14->CTL1 = ADC14_CTL1_RES_2;                   // Use sampling timer, 12-bit conversion results
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_2 | ADC14_MCTLN_DIF;     // A2 ADC input in dif. mode select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;                    // Enable ADC conv complete interrupt

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Ensures SLEEPONEXIT occurs immediately
    __DSB();

    while (1)
    {
        for (i = 20000; i > 0; i--);           // Delay
      ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;         // Start sampling/conversion

      __sleep();

      __no_operation();                        // For debugger
    }
}

// ADC14 interrupt service routine

void ADC14_IRQHandler(void) {
    if (ADC14->MEM[0] >= 0x07FF)                   // ADC12MEM0 = A1 > 0.5AVcc?
      P1->OUT |= BIT0;                           // P1.0 = 1
    else
      P1->OUT &= ~BIT0;                          // P1.0 = 0
}

