/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
 * CONTRIBUTORS BE LIABLE FOR ANY ->DIRECT, IN->DIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY ->OUT OF THE USE OF THIS SOFTWARE,
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
//   MSP432P401 Demo - USCI_A0 wake up from LPM3 by RxD pin as GPIO interrupt
//
//  Description: This demo achieves waking up from LPM3 by using RxD pin in UART as
//  GPIO interrupt, and when UART communication is over by receiving a certain data
//  (here using 0x55), MCU goes back to LPM3 and waits for next UART communication.
//
//  The application needs to set MCLK = DCO = 48MHz, and SMCLK = DCO/4 = 12MHz
//  and UART baudrate is 9600bps.
//
//
//                   MSP43P432P401
//             -----------------------
//         /|\|                       |
//          | |                       |
//          --|RST                    |
//            |                       |
//            |                       |
//            |     P1.3/EUSCI_A0->TXD|---->
//            |                       |
//            |     P1.2/EUSCI_A0->RXD|<----
//            |                       |
//            |                       |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | March 2016 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

void setP1_2Int(void);
void setRxInt(void);

uint8_t recData; //receive buffer

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    // Terminate all remaining pins on the device
    P1->DIR |= 0xff; P1->OUT = 0;
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

    // Change Vcore to 1
    // Transition to VCORE Level 1 from current power state properly
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

    // Configure Flash wait-state to 1 for both banks 0 & 1
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
            FLCTL_BANK1_RDCTL_WAIT_1;

    // Configure DCO to 48MHz, ensure MCLK uses DCO as source
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    // Select MCLK = DCO, no divider
    // Select SMCLK = DCO with a divider of 4
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK |
            CS_CTL1_SELS_MASK | CS_CTL1_DIVS_MASK)) |
            CS_CTL1_SELM_3 |
            CS_CTL1_SELS_3 |
            CS_CTL1_DIVS__4;
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as second function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_A_CTLW0_SSEL__SMCLK;      // Select clock source from SMCLK
    // Baud Rate calculation
    // refer to TRM table 22-5
    EUSCI_A0->BRW = 78;                     // 12000000/16/9600
    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Initialize eUSCI

    // Enable global interrupt
    __enable_irq();

    // Enable all SRAM bank retentions prior to going to LPM3
    SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKRET_BNK7_RET;
    PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;

    while(1)
    {
        // Enable GPIO interrupt on P1.2 instead UART RX pin
        setP1_2Int();

        // Enter LPM3 with P1.2 GPIO interrupt
        SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

        // Do not sleep on exit from ISR
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

        // Ensures SLEEPONEXIT takes effect immediately
        __DSB();

        __sleep();

        // For the debugger PC when halting the application
        __no_operation();

        // Enable sleep on exit from ISR
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

        // Ensures SLEEPONEXIT takes effect immediately
        __DSB();

        // Enter LPM0 with UART functionality
        // To exit LPM0 and re-enter LPM3 with GPIO interrupt,
        // send 0x55 to wake-up the MCU from LPM0
        __sleep();

        // For the debugger PC when halting the application
        __no_operation();

        // Disable the interrupt before setting up P1.2
        // as GPIO interrupt
        EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;
        NVIC->ISER[0] = 0;
    }
}

void setRxInt(void)
{
    P1->SEL0 |= BIT2;                       // Reconfigure P1.2 as UART RX function
    P1->REN &= ~BIT2;                       // Disable resistor enable
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear the UART RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable the UART RX interrupt

    // Enable eUSCI_A0 interrupt in NVIC
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
}

void setP1_2Int(void)
{
    P1->SEL0 &= ~BIT2;//P1.2 as IO
    P1->DIR &= ~BIT2;//P1.2 as input
    P1->OUT |= BIT2;//P1.2 pull up
    P1->REN |= BIT2;//P1.2 resistor enable
    P1->IES |= BIT2;//High-to int
    P1->IFG &= ~BIT2;//clear flag
    P1->IE |= BIT2;//enable int

    // Enable Port1 interrupt in NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
}

// UART interrupt service routine
void PORT1_IRQHandler(void)
{
    if(P1->IFG & BIT2)
    {
        setRxInt();
        P1->IE &= ~BIT2;
        NVIC->ISER[1] = 0;
        P1->IFG &= ~BIT2;
    }
}

// UART interrupt service routine
void EUSCIA0_IRQHandler(void)
{
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        recData = EUSCI_A0->RXBUF;
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = recData;
        if (recData == 0x55)
        {
            // Exit from sleep on exit from ISR
            SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

            // Ensures SLEEPONEXIT takes effect immediately
            __DSB();
        }
    }
}

