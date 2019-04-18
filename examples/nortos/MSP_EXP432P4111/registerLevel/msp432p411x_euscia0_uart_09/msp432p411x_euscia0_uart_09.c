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
//   MSP432P411 Demo - USCI_A0 wake up from LPM3 by UART interrupt
//
//  Description: This demo achieves waking up from LPM3 by using UART as interrupt,
//  and when UART communication is over by receiving a certain data (here using
//  ASCII 0x55 (an uppercase 'U')), MCU goes back to LPM3 and waits for next UART
//  communication.
//
//  The application needs to set ACLK = LFXT1 = 32,768Hz
//                               MCLK = DCO = 48MHz, and SMCLK = DCO/4 = 12MHz
//  and UART baudrate is 9600bps.
//
//
//                   MSP43P432P411
//             -----------------------
//         /|\|            LFXIN(PJ.0)|--
//          | |                       |  ~32768Hz
//          --|RST        LFXOUT(PJ.1)|--
//            |                       |
//            |                       |
//            |     P1.3/EUSCI_A0->TXD|---->
//            |                       |
//            |     P1.2/EUSCI_A0->RXD|<----
//            |                       |
//            |                       |
//
//   Bob Landers
//   Texas Instruments Inc.
//   November 2017 (updated) | March 2016 (created)
//   Built with CCSv7.3, IAR EWARM 8.20.1, Keil 5.24.1, GCC 6.3.1
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>


uint8_t recData; //receive buffer

int main(void)
{
    // Halting the Watchdog
    WDT_A->CTL = WDT_A_CTL_PW |
            WDT_A_CTL_HOLD;

    // Terminate all IO pins on the device to ensure lowest power by default
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

    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pins as second function

    PJ->SEL0 |= BIT0 | BIT1;                // set LFXT pin as second function

    // Change Vcore to 1
    // Transition to VCORE Level 1 from current power state properly
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

    // Configure Flash wait-state to 3 for both banks 0 & 1
    FLCTL_A->BANK0_RDCTL = (FLCTL_A->BANK0_RDCTL & ~(FLCTL_A_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_A_BANK0_RDCTL_WAIT_3;
    FLCTL_A->BANK1_RDCTL  = (FLCTL_A->BANK0_RDCTL & ~(FLCTL_A_BANK1_RDCTL_WAIT_MASK)) |
            FLCTL_A_BANK1_RDCTL_WAIT_3;

    // Enable retention for blocks at top (stack) and bottom (block0, which is always on)
    // (8blks/bank x 4banks=32 blocks)
    SYSCTL_A->SRAM_BLKRET_CTL0 = 0x80000001;

    // Disabling high side voltage monitor/supervisor
    PSS->KEY = PSS_KEY_KEY_VAL;
    PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
    PSS->KEY = 0;

    // Configuring Clocks
    //-----------------------------------------------------------------------------
    // Configure DCO to 48MHz, ensure MCLK uses DCO as source
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    // Select MCLK = DCO, no divider
    // Select SMCLK = DCO with a divider of 4
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK |
                            CS_CTL1_SELS_MASK | CS_CTL1_DIVS_MASK  )) |
               CS_CTL1_SELM_3 |
               CS_CTL1_SELS_3 |
               CS_CTL1_DIVS__4;

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
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Configuring UART Module
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST |     // Remain eUSCI in reset
                      EUSCI_A_CTLW0_SSEL__ACLK; // Select clock source from ACLK (=LFXT=32kHz)

    // Baud Rate calculation,
    // refer to TRM table 22-5, Section 22.3.10
    //
    EUSCI_A0->BRW = 3;                     // int(32768/9600)
    // Second modulation select, use oversampling (EUSCI_A_MCTLW_OS16) if BRCLK/BaudRate is >16
    EUSCI_A0->MCTLW = (0x92 << EUSCI_A_MCTLW_BRS_OFS);
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Initialize eUSCI

    // Enable EUSCIA0 Interrupt
    //---------------------------------------------------------------------------
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear the UART RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable the UART RX interrupt

    // Enable eUSCI_A0 interrupt in NVIC
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

    // Enable PCM rude mode, which allows to device to enter LPM3 without
    // waiting for peripherals
    PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;

    // Setting the sleep deep bit for LPM3 entry
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

    // Enable sleep on ISR exit
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    // Enable global interrupt
    __enable_irq();

    while(1)
    {
        // Enter LPM3 with UART functionality
        // To exit LPM3 and re-enter LPM0 with GPIO interrupt,
        // send 0x55 to wake-up the MCU from LPM3
        __sleep();

        // For the debugger PC when halting the application
        __no_operation();
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
        if (recData == 'U')
        {
            // Disable sleep-on-exit from ISR
            SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

            // Ensure above SLEEPONEXIT change takes effect immediately
            __DSB();

            P1->OUT ^= BIT0;
        }
    }
}

