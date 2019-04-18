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
//   MSP432P4111 Demo - WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
//
//  Description:  Toggle P1.0 using software timed by the WDT ISR. Toggle rate
//  is approximately 10ms = {(3MHz) / 32768} based on DCO = 3MHz
//  clock source used in this example for the WDT.
//  ACLK = n/a, SMCLK = 3MHz
//
//               MSP432P411x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | November 2013 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"

int main(void)
{
    /* Configure watch dog
     * - SMCLK as clock source
     * - Interval timer mode
     * - Clear WDT counter (initial value = 0)
     * - Watchdog interval = 32K = 2^15 ticks
     */
    WDT_A->CTL = WDT_A_CTL_PW |
            WDT_A_CTL_SSEL__SMCLK |
            WDT_A_CTL_TMSEL |
            WDT_A_CTL_CNTCL |
            WDT_A_CTL_IS_4;

    // Configure GPIO
    P1->DIR |= BIT0;                        // Set P1.0 to output direction

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up after ISR

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((WDT_A_IRQn) & 31);

    while (1)
    {
        /* Go to LPM0 mode */
        __sleep();
        __no_operation();                   // For debugger
    }
}


/* Watchdog Timer interrupt service routine */
void WDT_A_IRQHandler(void)
{
    P1->OUT ^= BIT0;                        // Toggle P1.0 LED
}


