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
//   MSP432P401 Demo - Power mode transition between LPM3 and AM_LF
//
//   Description: Change power mode from AM_LF to LPM3 and then use GPIO
//   interrupt to wake up device from LPM3 to AM_LF to toggle LED on P1.0.
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |            P1.1 |<---- Push button
//            |                 |
//            |            P1.0 |----> LED
//            |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

void error(void);

int main(void)
{
    volatile uint32_t i;
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

    // Configuring P1.0 as output and P1.1 (switch) as input with pull-up resistor
    // Rest of pins are configured as output low
    // Notice intentional '=' assignment since all P1 pins are being deliberately configured
    P1->DIR = ~(BIT1);
    P1->OUT = BIT1;
    P1->REN = BIT1;                         // Enable pull-up resistor (P1.1 output high)
    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1->IFG = 0;                            // Clear all P1 interrupt flags
    P1->IE = BIT1;                          // Enable interrupt for P1.1
    P1->IES = BIT1;                         // Interrupt on high-to-low transition

    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    // NOTE: This example assumes the default power state is AM_LDO_VCORE0.
    // Refer to previous MSP432p401x_pcm_0x code examples for more complete
    // power state transitions between active modes.

    // Transition to AM_LF mode: AM_LDO_VCORE0 --> AM_LF_VCORE0

    // Get current power state, if it's not AM_LDO_VCORE0, error out
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;
    // Skip the power state change if the device is already in AM1_LDO mode
    if (currentPowerState != PCM_CTL0_CPM_8)
    {
        if (currentPowerState != PCM_CTL0_CPM_0)
            error();

        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR__AM_LF_VCORE0;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
            error();                        // Error if transition was not successful
        if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_8)
            error();                        // Error if device is not in AM1_LDO mode
    }

    // Turn off PSS high-side supervisors
    PSS->KEY = PSS_KEY_KEY_VAL;
    PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
    PSS->KEY = 0;

    // Enable PCM rude mode, which allows to device to enter LPM3
    // without waiting for peripherals
    PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;

    // Enable all SRAM bank retentions prior to going to LPM3
    SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKRET_BNK7_RET;

    // Enable global interrupt
    __enable_irq();

    while(1)
    {
        // Setting the sleep deep bit
        SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

        // Ensures that SLEEPDEEP occurs immediately
        __DSB();

        // Go to LPM3
        __sleep();

        P1->OUT |= BIT0;                    // Set P1.0 LED on

        for(i = 0; i < 10000; i++);         // Short delay to blink the LED

        P1->OUT &= ~BIT0;                   // Set P1.0 LED off
    }
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

void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    // Delay for switch debounce
    for(i = 0; i < 10000; i++);

    P1->IFG &= ~BIT1;
}
