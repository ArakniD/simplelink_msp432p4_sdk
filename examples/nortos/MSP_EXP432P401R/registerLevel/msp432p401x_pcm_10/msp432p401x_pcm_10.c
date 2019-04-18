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
//   MSP432P401 Demo - Enter LPM3.5 mode with GPIO wake-up
//
//   Description: Go to LPM3.5 mode with port pin interrupt enabled.
//   Upon wake-up, the code checks if the device is waking up from LPM3.5 or
//   if it is a cold bootup. If the device is waking up from LPM3.5, it then
//   re-initializes all the port pins except clearing the interrupt flag so that
//   the interrupt is triggered when the interrupts are enabled. Within the
//   interrupt service routine, it toggles the LED on P1.0 before re-entering
//   LPM3.5 again.
//
//   NOTE: To ensure that LPM3.5 is entered properly, you would need to exit
//         debug mode first and use an external power supply.
//
//   For LPM3.5 with RTC operation, refer to msp432p401x_rtc_lpm35_01.c code
//   example.
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |            P1.1 |<---- GPIO
//            |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

void boardInit(void);

int main(void)
{
    volatile uint32_t i;
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Initialize GPIO and clocks
    boardInit();

    // Check if the MCU is waking-up from LPM3.5
    if (RSTCTL->PCMRESET_STAT & RSTCTL_PCMRESET_STAT_LPM35)
    {
        // Configuring P1 as output and P1.1 (switch) as input with pull-up resistor
        // Note that the interrupt flag is not cleared here
        P1->DIR &= ~(BIT1);
        P1->OUT |= BIT1;
        P1->REN = BIT1;                     // Enable pull-up resistor (P1.1 output high)
        P1->SEL0 = 0;
        P1->SEL1 = 0;
        P1->IE = BIT1;                      // Enable interrupt for P1.1
        P1->IES = BIT1;                     // Interrupt on high-to-low transition

        // Clear LPM lock
        PCM->CTL1 = PCM_CTL1_KEY_VAL;
    }
    else
    {
        // Else it is a cold-start

        // Configuring P1 as output and P1.1 (switch) as input with pull-up resistor
        // Rest of pins are configured as output low
        // Notice intentional '=' assignment since all P1 pins are being deliberately
        // configured.
        P1->DIR = ~(BIT1);
        P1->OUT = BIT1;
        P1->REN = BIT1;                     // Enable pull-up resistor (P1.1 output high)
        P1->SEL0 = 0;
        P1->SEL1 = 0;
        P1->IFG = 0;                        // Clear all P1 interrupt flags
        P1->IE = BIT1;                      // Enable interrupt for P1.1
        P1->IES = BIT1;                     // Interrupt on high-to-low transition
    }

    // Turn off PSS high-side supervisors
    PSS->KEY = PSS_KEY_KEY_VAL;
    PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
    PSS->KEY = 0;

    // Enable all SRAM bank retentions prior to going to LPM3 (Deep-sleep)
    SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKRET_BNK7_RET;

    // Enable global interrupt
    __enable_irq();

    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    // Enter or Re-enter LPM3.5
    // Set the Power Mode 3.5
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_LPMR__LPM35;

    // Setting the sleep deep bit
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

    // Ensure that SLEEPDEEP occurs immediately
    __DSB();

    __sleep();
    __no_operation();                       // For debugger

    // Blink LED slowly if LPM3.5 not entered properly
    while(1)
    {
        P1->OUT ^= BIT0;                    // XOR P1.0
        for (i = 500000; i > 0; i--);       // Delay
    }
}

// Port 1 interrupt service routine
void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    P1->OUT |= BIT0;                        // Set P1.0
    for (i = 100000; i > 0; i--);           // Delay
    P1->OUT &= ~BIT0;                       // Clear P1.0

    P1->IFG &= ~BIT1;                       // Clear P1.1 IFG
}

void boardInit(void)
{
    // GPIO Port Configuration for lowest power configuration
    P1->OUT = 0x00; P1->DIR = 0xFF;
    P2->OUT = 0x00; P2->DIR = 0xFF;
    P3->OUT = 0x00; P3->DIR = 0xFF;
    P4->OUT = 0x00; P4->DIR = 0xFF;
    P5->OUT = 0x00; P5->DIR = 0xFF;
    P6->OUT = 0x00; P6->DIR = 0xFF;
    P7->OUT = 0x00; P7->DIR = 0xFF;
    P8->OUT = 0x00; P8->DIR = 0xFF;
    P9->OUT = 0x00; P9->DIR = 0xFF;
    P10->OUT = 0x00; P10->DIR = 0xFF;
    PJ->OUT = 0x00; PJ->DIR = 0xFF;
}
