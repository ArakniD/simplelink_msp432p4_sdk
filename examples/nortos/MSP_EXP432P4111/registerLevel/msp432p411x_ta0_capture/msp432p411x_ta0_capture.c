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
//***************************************************************************************
//  MSP432P4111 Demo - Timer0_A3 Capture of ACLK
//
//  Description; Capture a number of periods of the ACLK clock and store them in an array.
//  When the set number of periods is captured the program is trapped and the LED on
//  P1.0 is toggled. At this point halt the program execution read out the values using
//  the debugger.
//
//  Ensure to connect ACLK out (P4.2) to TA0.CCI2A (P2.5) with a jumper or wire externally
//
//  ACLK = LFXT = 32.76kHz, MCLK = SMCLK = default DCODIV = 3MHz.
//  Per capture the timer counts ~3MHz/32.768kHz = ~91. Note that the value captured in the array
//  is cumulative.
//  External LF crystal required for example, Switch ACLK source to REFO if crystal is not
//  available.
//
//                MSP432P411x
//             -----------------
//         /|\|                 |
//          | |             P2.5|<-- TA0.CCI2A
//          --|RST              |  |
//            |             P4.2|--> ACLK
//            |                 |
//            |             P1.0|-->LED
//
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017
//   Built with CCSv7.1, IAR EWARM V7.80.4, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

#define NUMBER_TIMER_CAPTURES        20

volatile uint16_t timerAcaptureValues[NUMBER_TIMER_CAPTURES] = {0};
uint16_t timerAcapturePointer = 0;

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;                        // Set P1.0 as output
    P1->OUT |= BIT0;                        // P1.0 high
    P2->SEL0 |= BIT5;                       // TA0.CCI2A input capture pin, second function
    P2->DIR &= ~BIT5;
    P4->SEL0 |= BIT2;                       // Set as ACLK pin, second function
    P4->DIR |= BIT2;
    // Configure Port J
    PJ->DIR |= (BIT2 | BIT3); PJ->OUT &= ~(BIT2 | BIT3);
    PJ->SEL0 |= BIT0 | BIT1;                // set LFXT pin as second function

    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL2 |= CS_CTL2_LFXT_EN;            // LFXT on
    // Loop until XT1, XT2 & DCO fault flag is cleared
    do
        {
           // Clear XT2,XT1,DCO fault flags
           CS->CLRIFG |= CS_CLRIFG_CLR_DCOR_OPNIFG | CS_CLRIFG_CLR_HFXTIFG |
                   CS_CLRIFG_CLR_LFXTIFG | CS_CLRIFG_CLR_FCNTLFIFG;
           SYSCTL_A->NMI_CTLSTAT &= ~ SYSCTL_A_NMI_CTLSTAT_CS_SRC;
        } while ((SYSCTL_A->NMI_CTLSTAT | SYSCTL_A_NMI_CTLSTAT_CS_FLG)
                && (CS->IFG & CS_IFG_LFXTIFG)); // Test oscillator fault flag
    // Select ACLK as LFXTCLK
    CS->CTL1 &= ~(CS_CTL1_SELA_MASK) | CS_CTL1_SELA_0;
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Timer0_A3 Setup
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_CM_1 | // Capture rising edge,
            TIMER_A_CCTLN_CCIS_0 |          // Use CCI2A=ACLK,
            TIMER_A_CCTLN_CCIE |            // Enable capture interrupt
            TIMER_A_CCTLN_CAP |             // Enable capture mode,
            TIMER_A_CCTLN_SCS;              // Synchronous capture

    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 | // Use SMCLK as clock source,
            TIMER_A_CTL_MC_2 |              // Start timer in continuous mode
            TIMER_A_CTL_CLR;                // clear TA0R

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);

    __sleep();
    __no_operation();
}

// Timer A0 interrupt service routine
void TA0_N_IRQHandler(void)
{
    volatile uint32_t i;

    if (timerAcapturePointer >= NUMBER_TIMER_CAPTURES)
    {
        while (1)
        {
            P1->OUT ^= 0x01;                         // Toggle P1.0 (LED)
            for (i = 30000; i > 0; i--);
        }
    }
    else
    {
        timerAcaptureValues[timerAcapturePointer++] = TIMER_A0->CCR[2];
    }

    // Clear the interrupt flag
    TIMER_A0->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG);
}
