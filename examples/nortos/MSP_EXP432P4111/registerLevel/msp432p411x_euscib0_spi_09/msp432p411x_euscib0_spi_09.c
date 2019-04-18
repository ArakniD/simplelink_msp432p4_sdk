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
//   MSP432P4111 Demo - eUSCI_B0, SPI 3-Wire Master with clock polarity high
//
//   This example can be used with MSP432P411x_euscib0_spi_10.c 3-wire Slave.
//
//   Description: SPI master talks to SPI slave using 3-wire mode. This example
//   increments data sent by the master (TXData) starting at 0x01. Received data is
//   expected to be same as the previous transmission RXData = TXData - 1 if
//   this example is used with MSP432P411x_euscib0_spi_10.c.
//
//   NOTE- The slave MUST be started before the Master to ensure communications
//         are synchronized. The LaunchPad's Red LED is set high in case of any
//         errors.
//
//   USCI RX ISR is used to handle communication with the CPU, normally in LPM0.
//
//             MSP432P411x Master                    MSP432P411x Slaves
//             -----------------                      -----------------
//         /|\|                 |                 /|\|                 |
//          | |                 |                  | |                 |
//          --|RST              |                  --|RST              |
//            |                 |                    |                 |
//            |                 | Data In (UCB0SIMO) |                 |
//            |             P1.6|------------------->|P1.6             |
//            |                 |                    |                 |
//            |                 | Data OUT (UCB0SOMI)|                 |
//            |             P1.7|<-------------------|P1.7             |
//            |                 |                    |                 |
//            |                 | S Clock (UCB0CLK)  |                 |
//            |             P1.5|------------------->|P1.5             |
//            |                 |                    |                 |
//            |             P1.0|--->LED             |                 |
//
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | June 2014 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

static uint8_t RXData = 0;
static uint8_t TXData;

int main(void)
{
    volatile uint32_t i;
    uint8_t previousData;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    P1->OUT &= ~BIT0;
    P1->DIR |= BIT0;                        // Set P1.0 LED

    P1->SEL0 |= BIT5 | BIT6 | BIT7;         // Set P1.5, P1.6, and P1.7 as
                                            // SPI pins functionality

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put eUSCI state machine in reset
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST | // Remain eUSCI state machine in reset
            EUSCI_B_CTLW0_MST |             // Set as SPI master
            EUSCI_B_CTLW0_SYNC |            // Set as synchronous mode
            EUSCI_B_CTLW0_CKPL |            // Set clock polarity high
            EUSCI_B_CTLW0_MSB;              // MSB first

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__ACLK; // ACLK
    EUSCI_B0->BRW = 0x01;                   // /2,fBitClock = fBRCLK/(UCBRx+1).
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;// Initialize USCI state machine

    TXData = 0x01;                          // Initialize TX data to 0x01

    // Enable global interrupt
    __enable_irq();

    // Enable eUSCI_B0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    while(1)
    {
        EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;// Clear TXIFG flag
        EUSCI_B0->IE |= EUSCI_B_IE_TXIE;    // Enable TX interrupt
        __sleep();
        __no_operation();                   // For debug,Remain in LPM0

        // Check the received data
        previousData = TXData - 1;
//        if (RXData != (previousData))
        if ((RXData - previousData)>2)
        {
            // If the Received data is not equal to TXData-1, then
            // Set P1.0 LED
            P1->OUT |= BIT0;
        }
        else
        {
            // If the Received data is equal to TXData-1, then clear P1.0 LED
            P1->OUT &= ~BIT0;
        }

        for (i = 2000; i > 0; i--);         // Delay before next transmission
        TXData++;                           // Increment transmit data
    }
}

// SPI interrupt service routine
void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)
    {
        EUSCI_B0->TXBUF = TXData;           // Transmit characters
        EUSCI_B0->IE &= ~EUSCI_B__TXIE;

        // Wait till a character is received
        while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));
        RXData = EUSCI_B0->RXBUF;

        // Clear the receive interrupt flag
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG;
    }
}
