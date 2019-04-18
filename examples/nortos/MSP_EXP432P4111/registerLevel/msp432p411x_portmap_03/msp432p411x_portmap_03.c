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
//   MSP432P411 Demo - Port Map single function to multiple pins;
//      Multiple runtime configurations
//
// Description:  The port mapping of TimerA0 is changed with each WDT interrupt.
//               TimerA0 provides 4 PWMs with 4 different duty cycles.
//               TimerA0 is sourced from ACLK.  ACLK is REFO, 32kHz
//
//                 MSP432P411x
//             ------------------------
//         /|\|                        |
//          | |                        |
//          --|RST                     |
//            |     P2.0(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.1(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.2(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.3(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.4(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.5(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.6(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//            |     P2.7(TB0CCR1,2,3,4)|--> 25,50,75,87.5%
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | November 2013 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include "stdint.h"

#define PORT_MAP_RECFG                      // Multiple runtime Port Map configurations

/* Port2 Port Mapping definitions */
const uint8_t PortSequence[4] = {
        PMAP_TA0CCR1A,
        PMAP_TA0CCR2A,
        PMAP_TA0CCR3A,
        PMAP_TA0CCR4A
};

void Port_Mapping(uint8_t count)
{
    uint8_t i;
    uint32_t interruptState;
    volatile uint8_t *ptr;

    // Get the current interrupt state and store it temporarily
    interruptState = __get_PRIMASK();

    // Disable the global interrupt before configuring the
    // port mapping registers
    __disable_irq();

    PMAP->KEYID = PMAP_KEYID_VAL;                  // Enable Write-access to modify port mapping registers

#ifdef PORT_MAP_RECFG
    PMAP->CTL = PMAP_CTL_PRECFG;            // Allow reconfiguration during runtime
#endif

    ptr = (volatile uint8_t *) (&P2MAP->PMAP_REGISTER[0]);
    for (i = 0; i < 8; i++)
    {
        *ptr = PortSequence[count];
        ptr++;
    }

    PMAP->KEYID = 0;                        // Disable Write-Access to modify port mapping registers

    // Re-configure the interrupt if it was enabled
    __set_PRIMASK(interruptState);
}

volatile uint32_t interruptState;

int main(void)
{
    uint8_t count = 0;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    Port_Mapping(count);

    // Setup Port Pins
    P2->DIR = 0xFF;                         // P2.0 - P2.7 output
    P2->SEL0 = 0xFF;                        // P2.0 - P2.6 Port Map functions
    P2->SEL1 = 0;                           // P2.0 - P2.6 Port Map functions

    // Setup TA0
    TIMER_A0->CCR[0] = 256;                 // PWM Period/2
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_6; // CCR1 toggle/set
    TIMER_A0->CCR[1] = 192;                 // CCR1 PWM duty cycle
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_6; // CCR2 toggle/set
    TIMER_A0->CCR[2] = 128;                 // CCR2 PWM duty cycle
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_6; // CCR3 toggle/set
    TIMER_A0->CCR[3] = 64;                  // CCR3 PWM duty cycle
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_6; // CCR4 toggle/set
    TIMER_A0->CCR[4] = 32;                  // CCR4 PWM duty cycle
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 |  // ACLK
            TIMER_A_CTL_MC_3;               // Up-down mode

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up after ISR

    WDT_A->CTL = WDT_A_CTL_PW |
            WDT_A_CTL_SSEL__ACLK |          // ACLK
            WDT_A_CTL_TMSEL |               // Interval timer
            WDT_A_CTL_CNTCL |               // Clear the counter to start
            WDT_A_CTL_IS_4;                 // WDT 1s

    __enable_irq();

    interruptState = __get_interrupt_state();

    NVIC->ISER[0] = 1 << ((WDT_A_IRQn) & 31);

    while (1)
    {
        // Go to LPM0 mode
        __sleep();
        __no_operation();                   // For debugger

        // Re-configure the port-mapping to the next Timer CCR
        Port_Mapping(count++);

        if(count==4)
          count = 0;
    }
}


// Watchdog Timer interrupt service routine
void WDT_A_IRQHandler(void)
{
    // Do nothing, just need to wake up to main
}


