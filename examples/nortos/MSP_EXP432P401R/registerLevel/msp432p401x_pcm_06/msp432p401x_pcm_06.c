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
//   MSP432P401 Demo - Use TimerA in Low-Frequency mode with LPM0
//
//
//   Description: Configure device to operate in Low-Frequency LPM0 with TimerA
//   running and sourced by a low-frequency clock source. This examples builds
//   up on msp432p401_pcm_05, which exercises the transition into LF active mode
//   from active mode (LDO).
//
//   Note: the code in this example assumes the device is currently in LDO mode
//   AM0_LDO or AM1_LDO (Active Mode using LDO, VCore=0/1 respectively)
//
//   Transition from DCDC mode to LF requires intermediate transition through
//   LDO mode. For more information refer to the PCM chapter in the device user's
//   guide.
//
//          AM1_DCDC <----->  AM1_LDO  <--@--> AM1_LF
//                               ^
//                               |
//                               |
//                               v
//          AM0_DCDC <----->  AM0_LDO* <--@--> AM0_LF
//                                               ^
//                                               |
//                                               |
//                                               v
//                                             LPM0_LF
//   *: power state condition after reset
//   @: transitions demonstrated in this code example
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |             P1.0|--->LED
//            |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

void error(void);

int main(void)
{
    uint32_t currentPowerState;
    
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;


    // Terminate all remaining pins on the device
    P1->DIR |= 0xFF; P1->OUT = 0;
    P2->DIR |= 0xFF; P2->OUT = 0;
    P3->DIR |= 0xFF; P3->OUT = 0;
    P4->DIR |= 0xFF; P4->OUT = 0;
    P5->DIR |= 0xFF; P5->OUT = 0;
    P6->DIR |= 0xFF; P6->OUT = 0;
    P7->DIR |= 0xFF; P7->OUT = 0;
    P8->DIR |= 0xFF; P8->OUT = 0;
    P9->DIR |= 0xFF; P9->OUT = 0;
    P10->DIR |= 0xFF; P10->OUT = 0;
    PJ->DIR |= 0xFF; PJ->OUT = 0;

    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module
    // Switch MCLK, SMCLK, ACLK sources over to REFO clock for
    // low frequency operation first
    CS->CTL1 = CS_CTL1_SELM_2 |
            CS_CTL1_SELS_2 |
            CS_CTL1_SELA_2;
    CS->KEY = 0;                            // Lock CS module

    // Get current power state
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;

    // Transition to Low-Frequency Mode from current LDO power state properly
    switch (currentPowerState)
    {
        case PCM_CTL0_CPM_0:                // AM0_LDO, need to switch to AM0_Low-Frequency Mode
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_8;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
                error();                    // Error if transition was not successful
            break;
        case PCM_CTL0_CPM_1:                // AM1_LDO, need to switch to AM1_Low-Frequency Mode
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_9;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
                error();                    // Error if transition was not successful
            break;
        case PCM_CTL0_CPM_8:                // Device is already in AM0_Low-Frequency Mode
            break;
        case PCM_CTL0_CPM_9:                // Device is already in AM1_Low-Frequency Mode
            break;
        default:                            // Device is in some other state, which is unexpected
            error();
    }

    // Configure TimerA to run off SMCLK = REFO
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // Enable capture compare interrupt
    TIMER_A0->CCR[0] = 1000;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK
            TIMER_A_CTL_MC__CONTINUOUS;     // continuous mode

    // Enable TimerA3_CC0 interrupt on the NVIC
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

    // Enable global interrupt
    __enable_irq();

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Do not wake up on exit from ISR

    // Ensures the SLEEPONEXIT mask is set.
    __DSB();

    // Go to LPM0
    __sleep();
}

// TimerA3_0 interrupt service routine handler
void TA0_0_IRQHandler(void)
{
    P1->OUT ^= BIT0;                  // Toggle P1.0 LED
    TIMER_A0->CCTL[0] &= ~ TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCR[0] +=1000;
}

void error(void)
{
    volatile uint32_t i;

    while (1)
    {
        P1->OUT ^= BIT0;
        for(i = 0; i < 20000; i++);         // Blink LED forever
    }
}
