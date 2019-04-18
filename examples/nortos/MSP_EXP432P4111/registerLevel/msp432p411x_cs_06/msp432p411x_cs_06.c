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
 * effects. Also see MSP432 SDK http://www.ti.com/tool/simplelink-msp432-sdk
 * for an API library & https://dev.ti.com/pinmux/ for a GUI-based approach
 * to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP432P4111 Demo:LFXT sources ACLK with fault detection.Outputs ACLK on P4.2
//
//  Description: This program demonstrates using XT1 to source ACLK
//  ACLK = LFXT1 = 32,768Hz with oscillator fault detection enabled.
//  It then blinks the LED when the crystal has stabilized. But, if there is an
//  oscillator fault detected, the Clock System interrupt handler is
//  automatically triggered to start clearing and checking the oscillator
//  fault flag. During this period of time, the LED blinks at a faster rate.
//
//  To force an oscillator fault flag, short the LFXIN and LFXOUT pins.
//
//  An external 32.768kHz crystal between LFXIN & LFXOUT is required for ACLK
//
//                MSP432P411x
//            -------------------
//        /|\|        LFXIN(PJ.0)|--
//         | |                   |  ~32768Hz
//         --|RST    LFXOUT(PJ.1)|--
//           |                   |
//           |              P4.2 |--> ACLK=32.768kHz
//           |                   |
//           |              P1.0 |--> LED
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017
//   Built with CCSv7.1, IAR EWARM V8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

int main(void)
{
    volatile unsigned int i = 0;

    WDT_A->CTL = WDT_A_CTL_PW |
            WDT_A_CTL_HOLD;                 // Stop WDT

    // Configure GPIO
    P1->OUT &= ~BIT0;
    P1->DIR |= BIT0;

    P4->DIR |= BIT2;                        // P4.2 output
    P4->SEL0 |= BIT2;                       // P4.2 option select

    PJ->SEL0 |= BIT0 | BIT1;                // set LFXT pin as second function

    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access

    CS->CTL2 |= CS_CTL2_LFXT_EN;            // LFXT on

    // Loop until LFXT fault flag is cleared
    do
    {
       // Clear XT2,XT1,DCO fault flags
       CS->CLRIFG |= CS_CLRIFG_CLR_DCOR_OPNIFG | CS_CLRIFG_CLR_HFXTIFG |
               CS_CLRIFG_CLR_LFXTIFG | CS_CLRIFG_CLR_FCNTLFIFG;
       SYSCTL_A->NMI_CTLSTAT &= ~ SYSCTL_A_NMI_CTLSTAT_CS_SRC;

    } while ((SYSCTL_A->NMI_CTLSTAT | SYSCTL_A_NMI_CTLSTAT_CS_FLG)
            && (CS->IFG & CS_IFG_LFXTIFG)); // Test LFXT oscillator fault flag

    // Select ACLK as LFXTCLK
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELA_MASK | CS_CTL1_DIVA_MASK)) | CS_CTL1_SELA_0;

    // Enable LFXT fault interrupt
    CS->IE |= CS_IE_LFXTIE;

    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Enable global interrupt
    __enable_irq();

    // Enable CS interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((CS_IRQn) & 31);

    while (1)
    {
        P1->OUT ^= BIT0;                    // Toggle LED

        // Wait 160,000 CPU Cycles
        for(i=0; i<160000; i++);
    }
}

// Clock system fault interrupt handlers
// This interrupt handler is triggered upon a crystal fault and will attempt
// to check and clear the LFXT interrupt flag.
void CS_IRQHandler(void)
{
    volatile uint32_t i;

    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access

    // Loop until fault flag is cleared
    do
    {
       // Clear XT2,XT1,DCO fault flags
       CS->CLRIFG |= CS_CLRIFG_CLR_DCOR_OPNIFG | CS_CLRIFG_CLR_HFXTIFG |
               CS_CLRIFG_CLR_LFXTIFG | CS_CLRIFG_CLR_FCNTLFIFG;

       SYSCTL_A->NMI_CTLSTAT &= ~ SYSCTL_A_NMI_CTLSTAT_CS_SRC;

       // Blink the LED at a faster rate to indicate the application
       // is handling the lfxt fault condition
       P1->OUT ^= BIT0;                    // Toggle LED

       // Wait 40,000 CPU Cycles
       for(i=0; i<40000; i++);

    } while ((SYSCTL_A->NMI_CTLSTAT | SYSCTL_A_NMI_CTLSTAT_CS_FLG)
            && (CS->IFG & CS_IFG_LFXTIFG)); // Test oscillator fault flag

    CS->KEY = 0;                            // Lock CS module from unintended accesses
}
