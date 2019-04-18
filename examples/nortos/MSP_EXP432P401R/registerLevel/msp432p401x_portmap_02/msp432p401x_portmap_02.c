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
//   MSP432P401 Demo - Port Map a single function to multiple pins
//
//   Description:  Port2 is port mapped to output TimerA digital signal. TACCR1
//   and TACCR3 generate different PWM duty cycles and outputs to Port2 pins.
//   The frequency of TACCR1 and TACCR3 is ACLK/2^10 ~32Hz.
//
//   MCLK = SMCLK = default DCO; ACLK = REFO~32kHz;
//
//                      MSP432P401x
//             ---------------------------
//         /|\|                           |
//          | |                           |
//          --|RST                        |
//            |     P2.0(TIMER_A0->CCR[1])|--> 25%
//            |     P2.1(TIMER_A0->CCR[1])|--> 25%
//            |     P2.2(TIMER_A0->CCR[1])|--> 25%
//            |     P2.3(TIMER_A0->CCR[1])|--> 25%
//            |     P2.4(TIMER_A0->CCR[3])|--> 75%
//            |     P2.5(TIMER_A0->CCR[3])|--> 75%
//            |     P2.6(TIMER_A0->CCR[3])|--> 75%
//            |     P2.7(TIMER_A0->CCR[3])|--> 75%
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

// Port2 Port Mapping definitions
const uint8_t P2Mapping[8] = {
        PMAP_TA0CCR1A,
        PMAP_TA0CCR1A,
        PMAP_TA0CCR1A,
        PMAP_TA0CCR1A,
        PMAP_TA0CCR3A,
        PMAP_TA0CCR3A,
        PMAP_TA0CCR3A,
        PMAP_TA0CCR3A
};


void Port_Mapping(void)
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
        *ptr = P2Mapping[i];
        ptr++;
    }

    PMAP->KEYID = 0;                        // Disable Write-Access to modify port mapping registers

    // Re-configure the interrupt if it was enabled
    __set_PRIMASK(interruptState);
}

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    Port_Mapping();

    // Setup Port Pins
    P2->DIR |= 0xFF;                        // P2.0 - P2.7 output
    P2->SEL0 |= 0xFF;                       // P2.0 - P2.6 Port Map functions
    P2->SEL1 = 0;                           // P2.0 - P2.6 Port Map functions

    // Setup TA0
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_OUTMOD_4; // CCR0 toggle/set
    TIMER_A0->CCR[0] = 256;                 // PWM Period/2
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_6; // CCR1 toggle/set
    TIMER_A0->CCR[1] = 192;                 // CCR1 PWM duty cycle
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_6; // CCR3 toggle/set
    TIMER_A0->CCR[3] = 96;                  // CCR3 PWM duty cycle

    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 |  // ACLK
            TIMER_A_CTL_MC_3;               // Up-down mode

    // Go to LPM0 mode
    __sleep();
    __no_operation();                       // For debugger
}

