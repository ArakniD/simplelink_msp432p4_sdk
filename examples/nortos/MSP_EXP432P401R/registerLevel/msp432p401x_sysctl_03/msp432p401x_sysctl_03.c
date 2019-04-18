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
//   MSP432P401 Demo - Enable/disable SRAM bank access & retention
//
//   Description: Configure SRAM banks to enable & disable selected banks.
//   Unused SRAM banks can be disabled prior going to LPM3 to
//   minimize power consumption.
//
//               MSP432P401x
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//     /|\   |                 |
//      --o--|P1.1         P1.0|-->LED
//     \|/
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

#define CPUSYS_SRAM_BANKRET_SRAM_RDY 0x10000

// Allocate these variables on the top of the SRAM memory (Bank 0)
uint32_t * address;
volatile uint32_t i;

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    P1->OUT &= ~BIT0;                       // Clear P1.0
    P1->DIR |= BIT0;                        // P1.0 set as output

    // Turn on all SRAM banks for active mode usage.
    // Banks 7 & below are enabled
    SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK7_EN ;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKEN & SYSCTL_SRAM_BANKEN_SRAM_RDY));

    // Write & read back from an SRAM memory location in bank 7
    address = (uint32_t *) 0x2000EFFC;
    *address = 0xDEADBEEF;
    i = *address;

    if (i != 0xDEADBEEF)
    {
        P1->OUT |= BIT0;
        while(1);
    }

    // Disable SRAM retention during LPM3
    // Then check if memory location in bank 7 is retained after
    // wake up

    // Enable SRAM retention for BANK0 only to minimize power
    // consumption
    SYSCTL->SRAM_BANKRET = SYSCTL_SRAM_BANKRET_BNK0_RET;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKRET & SYSCTL_SRAM_BANKRET_SRAM_RDY));

    // Configure GPIO
    P1->OUT |= BIT1;                        // Pull-up resistor on P1.1
    P1->REN |= BIT1;                        // Select pull-up mode for P1.1
    P1->DIR &= ~BIT1;                       // Set all but P1.1 to output direction
    P1->IES |= BIT1;                        // P1.1 Hi/Lo edge
    P1->IFG = 0;                            // Clear all P1 interrupt flags
    P1->IE |= BIT1;                         // P1.1 interrupt enabled

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Entering LPM3 with GPIO interrupt

    // Setting the sleep deep bit
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

    // Ensures SLEEPDEEP takes effect immediately
    __DSB();

    // Enable global interrupt
    __enable_irq();
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    __sleep();
    __no_operation();                       // For debugger

    // Clearing the sleep deep bit
    SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk);

    // Ensures SLEEPDEEP Mask is cleared immediately
    __DSB();

    // Wait until SRAM banks are ready for DSL entry
    while (! (SYSCTL->SRAM_BANKRET & SYSCTL_SRAM_BANKRET_SRAM_RDY));

    // Confirm Bank 7 is still enabled in active mode
    // Since the value was not retained in LPM3, we should
    // re-write the value to Bank 7 and read the value back
    *address = 0xDEADBEEF;
    i = *address;

    if (i != 0xDEADBEEF)
    {
        P1->OUT |= BIT0;
        while(1);
    }

    // Re-enable SRAM retention for BANK7 so that the
    // debugger can reconnect
    SYSCTL->SRAM_BANKRET = SYSCTL_SRAM_BANKRET_BNK7_RET;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKRET & SYSCTL_SRAM_BANKRET_SRAM_RDY));

    while(1)
    {
        P1->OUT ^= BIT0;                    // Toggle P1.0 LED
        for (i = 100000; i > 0; i--);       // Delay
    }
}

/* Port 1 interrupt service routine */
void PORT1_IRQHandler(void)
{
    P1->IE &= ~BIT1;                        // Disable interrupt
    P1->IFG &= ~BIT1;                       // Clear P1.1 IFG
}
