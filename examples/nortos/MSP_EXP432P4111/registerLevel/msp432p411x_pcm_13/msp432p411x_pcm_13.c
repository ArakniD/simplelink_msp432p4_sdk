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
//   MSP432P411 Demo - Use TimerA in Low-Frequency mode with LPM3
//
//
//   Description: Configure device to operate in Low-Frequency LPM3 with TimerA
//   running and sourced by a low-frequency clock source (LFXT).
//   The device is configured in a manner to minimize power consumption and
//   achieve datasheet specifications with regard to power consumption.
//   Note that the LED toggle must be disabled in the ISR to achieve this.
//
//                MSP432P411x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |             P1.0|--->LED
//            |                 |
//
//   Bob Landers
//   Texas Instruments Inc.
//   November 2017 (created)
//   Built with CCSv7.3, IAR 8.20.1, Keil 5.24.1, GCC 6.3.1
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

void error(void);

int main(void)
{
    // Halting the Watchdog
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // Terminate all pins on the device to minimize power consumption
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

    // Starting LFXT and sourcing ACLK from it
    //========================================================================
    // set LFXT pin as second function
    PJ->SEL0 |= BIT0 | BIT1;

    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module
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
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELA_MASK | CS_CTL1_DIVA_MASK)) | CS_CTL1_SELA_0;

    // Enable LFXT fault interrupt
    CS->IE |= CS_IE_LFXTIE;

    CS->KEY = 0;                            // Lock CS module

    /* Timer_A Continuous Mode Configuration Parameters */
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;  // Enable capture compare interrupt
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK | // ACLK
            TIMER_A_CTL_MC__CONTINUOUS ;
    TIMER_A0->CCR[0] = 10000;

    // Enable retention for blocks at top (stack) and bottom (block0, which is always on)
    // (8blks/bank x 4banks=32 blocks)
    SYSCTL_A->SRAM_BLKRET_CTL0 = 0x80000001;

    // Disabling high side voltage monitor/supervisor
    PSS->KEY = PSS_KEY_KEY_VAL;
    PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
    PSS->KEY = 0;

    // Enable PCM rude mode, which allows to device to enter LPM3 without
    // waiting for peripherals
    PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;

    // Enable Sleep on ISR Exit
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Enable TimerA0 interrupt on the NVIC
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

    // Enable global interrupt
    __enable_irq();

    // Going to LPM3
    //------------------------------------------
    // Setting the sleep deep bit for LPM3 entry
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
    __DSB();
    // Go to LPM3
    __sleep();
}

// TimerA0_0 interrupt service routine handler
void TA0_0_IRQHandler(void)
{
    // Toggle P1.0 LED (comment out this line for power measurement)
    P1->OUT ^= BIT0;

    TIMER_A0->CCTL[0] &= ~ TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCR[0] +=10000;
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
