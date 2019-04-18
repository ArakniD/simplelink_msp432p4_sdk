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
//  MSP432P401 Demo - eUSCI_B0 I2C 4 Hardware I2C slaves RX single byte
//
//  Description: This demo connects two MSP432's via the I2C bus.
//  This code configures the MSP432 USCI to be addressed as 4 independent I2C
//  slaves. Each slave has its own interrupt flag and data variable to store
//  incoming data.
//  Use with MSP430G6021x_uscib0_i2c_15.c
//
//
//    *****used with "msp432p401_euscia0_i2c_master_multislave.c"****
//
//                                 |    |
//               MSP432P401      10k  10k     MSP432P401
//                   slave         |    |        master
//             -----------------   |    |   -----------------
//            |     P1.6/UCB0SDA|<-|----|->|P1.6/UCB0SDA     |
//            |                 |  |       |                 |
//            |                 |  |       |                 |
//            |     P1.7/UCB0SCL|<-|------>|P1.7/UCB0SCL     |
//            |                 |          |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | June 2014 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

uint8_t RXData0 = 0;
uint8_t RXData1 = 0;
uint8_t RXData2 = 0;
uint8_t RXData3 = 0;

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    // Configure Pins for I2C
    P1->SEL0 |= BIT6 | BIT7;                // I2C pins

    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIB0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

    // Configure USCI_B0 for I2C mode
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain in reset
            EUSCI_B_CTLW0_MODE_3;           // I2C slave mode, SMCLK
    EUSCI_B0->I2COA0 = 0x0A |               // SLAVE0 own address is 0x0A
            EUSCI_B_I2COA2_OAEN;            // Enable slave own address 0
    EUSCI_B0->I2COA1 = 0x0B |               // SLAVE1 own address is 0x0B
            EUSCI_B_I2COA2_OAEN;            // Enable slave own address 1
    EUSCI_B0->I2COA2 = 0x0C |               // SLAVE2 own address is 0x0C
            EUSCI_B_I2COA2_OAEN;            // Enable slave own address 2
    EUSCI_B0->I2COA3 = 0x0D |               // SLAVE3 own address is 0x0D|
            EUSCI_B_I2COA2_OAEN;            // Enable slave own address 3
    EUSCI_B0->CTLW0 &=~EUSCI_A_CTLW0_SWRST; // clear reset register

    EUSCI_B0->IE |=  EUSCI_B_IE_RXIE0 |     // Enable receive interrupts
            EUSCI_B_IE_RXIE1 |
            EUSCI_B_IE_RXIE2 |
            EUSCI_B_IE_RXIE3;

    // Don't wake up on exit from ISR
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    // Enter LPM0
    __sleep();
    __no_operation();
}

// I2C interrupt service routine
void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG3)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG3;
        RXData3 = EUSCI_B0->RXBUF;
    }
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG2)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG2;
        RXData2 = EUSCI_B0->RXBUF;
    }
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG1)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG1;
        RXData1 = EUSCI_B0->RXBUF;
    }
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG0;
        RXData0 = EUSCI_B0->RXBUF;
    }
}
