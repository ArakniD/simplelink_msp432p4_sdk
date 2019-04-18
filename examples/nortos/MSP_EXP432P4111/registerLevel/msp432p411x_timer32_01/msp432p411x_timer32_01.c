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
//  MSP432P4111 Demo - Timer32 Example, One-shot mode in free run
//
//  Description: Timer 32 (module 0) is set up in 32-bit, free-run, and one-shot
//  mode. Timer count is loaded with an initial value of 0xFFFFFF.
//  After the timer counts down and reaches the 0, the timer interrupt triggers
//  and the ISR is executed. Within the ISR, it just clears the interrupt flag
//  and exits back to the main application and enters a while loop which blinks
//  the P1.0 LED.
//
//           MSP432P411x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017
//   Built with CCSv7.1, IAR EWARM v8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    // Timer32 set up in one-shot, free run, 32-bit, no pre-scale
    TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE |
            TIMER32_CONTROL_ONESHOT;

    // Load Timer32 Counter with initial value
    TIMER32_1->LOAD= 0xFFFFFF;

    // Enalbe the Timer32 interrupt in NVIC
    __enable_irq();
    NVIC->ISER[0] = 1 << ((T32_INT1_IRQn) & 31);

    // Start Timer32 and enable interrupt
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE |
            TIMER32_CONTROL_IE;

    // Disable sleep on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    // Enter LPM0 and wait for the timer interrupt to wake-up
    __sleep();
    __no_operation();

    // Timer interrupt wakes-up
    while (1)
    {
        P1->OUT ^= BIT0;                    // Toggle P1.0 LED
        for (i = 100000; i > 0; i--);       // Delay
    }
}

void T32_INT1_IRQHandler(void)
{
    TIMER32_1->INTCLR |= BIT0;              // Clear Timer32 interrupt flag

    // Disable the timer and interrupt
    TIMER32_1->CONTROL &= ~(TIMER32_CONTROL_ENABLE |
            TIMER32_CONTROL_IE);
}
