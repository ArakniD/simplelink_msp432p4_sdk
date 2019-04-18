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
//  MSP432P411 Demo - Timer_A3, Toggle P1.0;P7.3,P2.4-5, Cont. Mode ISR,
//                    32kHz ACLK
//
//  Description: Use Timer0_A3 CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, TIMER_A0->CCR[0],
//  TIMER_A0->CCR[1] and TIMER_A0->CCR[2] output units are optionally selected
//  with port pins P7.3, P2.4 and P2.5 in toggle mode. As such, these pins will
//  toggle when respective TA0CCRx registers match the TA0R counter. Interrupts
//  are also enabled with all TA0CCRx units, software loads offset to next
//  interval only - as long as the interval offset is added to TA0CCRx, toggle
//  rate is generated in hardware. Timer0_A1 overflow ISR is used to toggle
//  P1.0 with software. Proper use of the TIMER_A0->IV interrupt vector
//  generator is demonstrated.
//
//  ACLK = TACLK = 32kHz, MCLK = SMCLK = default DCO ~3MHz
//
//  As coded and with TA0CLK = 32768Hz, toggle rates are:
//  P7.3 = TIMER_A0->CCR[0] = 32768/(2*4) = 4096Hz
//  P2.4 = TIMER_A0->CCR[1] = 32768/(2*16) = 1024Hz
//  P2.5 = TIMER_A0->CCR[2] = 32768/(2*100) = 163.84Hz
//  P1.0 = overflow = 32768/(2*65536) = 0.25Hz
//
//                MSP432P411
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//           |                 |
//           |       P7.3/TA0.0|--> TIMER_A0->CCR[0]
//           |       P2.4/TA0.1|--> TIMER_A0->CCR[1]
//           |       P2.5/TA0.2|--> TIMER_A0->CCR[2]
//           |             P1.0|--> Overflow/software
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | June 2014 (created)
//   Built with CCSv7.0, IAR EWARM V7.80.4, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    P7->DIR |= BIT3;                        // P7.3 option selection TA0.0
    P7->SEL0 |= BIT3;

    P2->DIR |= BIT4 | BIT5;                 // P2.4-5 option selection TA0.1-2
    P2->SEL0 |= BIT4 | BIT5;

    // Configure Timer_A
    // TIMER_A0->CCR[0] toggle, interrupt enabled
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_OUTMOD_4 |
            TIMER_A_CCTLN_CCIE;
    // TACCR1 toggle, interrupt enabled
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_4 |
            TIMER_A_CCTLN_CCIE;
    // TACCR2 toggle, interrupt enabled
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_4 |
            TIMER_A_CCTLN_CCIE;
    // ACLK, cont mode, interrupt enabled
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 |  // ACLK
            TIMER_A_CTL_MC_2 |              // continuous mode
            TIMER_A_CTL_IE;                 // Enable timer overflow interrupt

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] |= 1 << ((TA0_N_IRQn) & 31);
    NVIC->ISER[0] |= 1 << ((TA0_0_IRQn) & 31);

    __sleep();

    __no_operation();                       // For debugger
}

// Timer A0 interrupt service routine
void TA0_0_IRQHandler(void)
{
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCR[0] += 4;                  // Add Offset to TIMER_A0->CCR[0]
}

void TA0_N_IRQHandler(void)
{
    if(TIMER_A0->CCTL[1]&TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
        TIMER_A0->CCR[1] += 16;             // Add Offset to TIMER_A0->CCR[1]
    }
    if(TIMER_A0->CCTL[2]&TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIFG;
        TIMER_A0->CCR[2] += 100;            // Add Offset to TIMER_A0->CCR[2]
    }
    if(TIMER_A0->CTL&TIMER_A_CTL_IFG)
    {
        // Clear timer overflow flag
        TIMER_A0->CTL &= ~TIMER_A_CTL_IFG;
        P1->OUT ^= BIT0;
    }
}
