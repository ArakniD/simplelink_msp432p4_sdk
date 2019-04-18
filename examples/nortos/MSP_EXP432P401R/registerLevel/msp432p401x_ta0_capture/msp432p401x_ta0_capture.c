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
//  MSP432P401 Demo - Timer0_A3 Capture of ACLK
//
//  Description: Capture a number of periods of the ACLK clock and store them in an array.
//  Upon capture completion, the difference between each capture is calculated in main.
//  The difference is then checked if it is within a +/- 10 SMCLK cycles range. If the
//  check passes, the LED on P1.0 is toggled. Else, LED on P1.0 remains ON if the check
//  fails.
//  This example assumes MCLK == SMCLK which is 3MHz. If the SMCLK frequency is changed,
//  the REFO_CYCLES_TO_MCLK defined value needs to be adjusted.
//
//  timerAcaptureValues[] array is used to capture the raw value of the capture compare
//                        register.
//
//  timerAcaptureCalcVal[] array is used to capture the difference between each capture
//                         compare value. This value represents the number of SMCLK
//                         cycles relative to ACLK.
//
//  NOTE: Ensure to connect ACLK out (P4.2) to TA0.CCI2A (P2.5) with a jumper.
//
//  ACLK = REFOCLK = 32kHz, MCLK = SMCLK = default DCODIV = 3MHz.
//
//                MSP432P401
//             -----------------
//         /|\|                 |
//          | |             P2.5|<-- TA0.CCI2A
//          --|RST              |  |
//            |             P4.2|--> ACLK
//            |                 |
//            |             P1.0|-->LED
//
//
//   William Goh
//   Texas Instruments Inc.
//   October 2016 (updated) | June 2014 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//***************************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

#define REFO_CYCLES_TO_MCLK          92
#define NUMBER_TIMER_CAPTURES        20

uint16_t timerAcaptureValues[NUMBER_TIMER_CAPTURES] = {0};
uint16_t timerAcaptureCalcVal[NUMBER_TIMER_CAPTURES - 1] = {0};
uint16_t timerAcapturePointer = 0;

int main(void)
{
    uint32_t i;
    uint32_t error = 0;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;                        // Set P1.0 as output
    P1->OUT |= BIT0;                        // P1.0 high
    P2->SEL0 |= BIT5;                       // TA0.CCI2A input capture pin, second function
    P2->DIR &= ~BIT5;
    P4->SEL0 |= BIT2;                       // Set as ACLK pin, second function
    P4->DIR |= BIT2;

    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    // Select ACLK = REFO, SMCLK = MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 |
            CS_CTL1_SELS_3 |
            CS_CTL1_SELM_3;
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Timer0_A3 Setup
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_CM_1 | // Capture rising edge,
            TIMER_A_CCTLN_CCIS_0 |          // Use CCI2A=ACLK,
            TIMER_A_CCTLN_CCIE |            // Enable capture interrupt
            TIMER_A_CCTLN_CAP |             // Enable capture mode,
            TIMER_A_CCTLN_SCS;              // Synchronous capture

    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 | // Use SMCLK as clock source,
            TIMER_A_CTL_MC__CONTINUOUS |    // Start timer in continuous mode
            TIMER_A_CTL_CLR;                // clear TA0R

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);

    __sleep();
    __no_operation();

    // Validate the captured clocks
    for (i = 0; i < (NUMBER_TIMER_CAPTURES - 1); i++)
    {
        timerAcaptureCalcVal[i] = timerAcaptureValues[i + 1] - timerAcaptureValues[i];

        if ((timerAcaptureCalcVal[i] > (REFO_CYCLES_TO_MCLK + 10)) ||
                (timerAcaptureCalcVal[i] < (REFO_CYCLES_TO_MCLK - 10)))
        {
            // Set an error flag
            error = 1;
        }
    }

    while (1)
    {
        if (error)
        {
            // Sets the P1.0 (LED) high
            P1->OUT = 0x01;
        }
        else
        {
            // Toggle P1.0 (LED)
            P1->OUT ^= 0x01;
            for (i = 30000; i > 0; i--);
        }
    }
}

// Timer A0 interrupt service routine
void TA0_N_IRQHandler(void)
{

    if (timerAcapturePointer >= NUMBER_TIMER_CAPTURES)
    {
        // Disable capture timer
        TIMER_A0->CTL &= ~(TIMER_A_CTL_MC__CONTINUOUS);

        // Disable sleep on exit upon returning from ISR
        // to continue executing code in main
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

        // Ensures SLEEPONEXIT takes effect immediately
        __DSB();
    }
    else
    {
        // Capture the CCR register value into the array
        timerAcaptureValues[timerAcapturePointer++] = TIMER_A0->CCR[2];
    }

    // Clear the interrupt flag
    TIMER_A0->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG);
}
