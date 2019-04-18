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
//  MSP432P411 Demo - Use SysTick to measure cycle count of a function
//
//  Description: Use SysTick to count the number of clock cycles elapsed in a
//  function. Note that for extremely intensive function/code that might
//  take more than 2^24 CPU cycles to complete, SysTick will overflow. An
//  interrupt service routine is used to track the number of overflows.
//  Interrupt latency and ISR execution (one C instruction) are not accounted
//  for in the calculation since they are small and can be negligible (compared
//  to multiples of 2^24 cycles)
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
//   August 2017 (updated) | November 2013 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

uint32_t overflow = 0;
volatile uint32_t totalCycles = 0;
void foo(void);

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    // Configure SysTick, module not started yet
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    // Set SysTick period = 0x1000000000000 = 2^24
    SysTick->LOAD = 0xFFFFFF;

    // Enable global interrupt
    __enable_irq();

    // Enable SysTick interrupt
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    // Start SysTick Timer
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    // Call the function to measure clock cycles
    foo();

    // Disable SysTick Timer
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    // Calculate the # of cycles
    totalCycles = (0x1000000 - SysTick->VAL) + 0x1000000 * overflow ;

    if (totalCycles > 0)
        P1->OUT |= BIT0;

    while(1);
}

void foo(void)
{
    volatile uint32_t i;
    for (i=0; i<0xFFFF;i++);
}

void SysTick_Handler(void)
{
    /* This should never happen unless the function takes more > 2^24 cycles
     * to complete.
     */
    overflow++;                             // Increment # of systick timer overlows
}

