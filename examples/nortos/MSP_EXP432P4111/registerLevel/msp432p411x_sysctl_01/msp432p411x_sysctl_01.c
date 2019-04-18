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
//   MSP432P4111 Demo - Exercise SYSCTL features to retrieve device's memory size
//
//   Description: Get device's SRAM & FLASH sizes using SYSCTL registers.
//                The example toggles the LED on P1.0 if successful. Else, it
//                sets the LED on. Monitor variable "error" for results.
//                error = 1 (SRAM size mismatch), 2 (Flash size mismatch), 3 (both)
//
//                MSP432P411x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | November 2013 (created)
//   Built with CCS7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

int main(void)
{
    volatile uint32_t i;
    uint32_t error = 0;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    P1->DIR |= BIT0;                        // P1.0 set as output

    // Check if device's SRAM size is equal to 256kB (0x40000) 
	// or 128kB (0x20000)
    if ((SYSCTL_A->SRAM_SIZE != 0x00040000) &&
        (SYSCTL_A->SRAM_SIZE != 0x00020000))
    {
        error = 1;
    }

    // Check if device's USER FLASH  size is equal to 2048kB (0x200000),
    // or 1024kB (0x100000) or 512kB (0x080000)
    if ((SYSCTL_A->MAINFLASH_SIZE != 0x200000) &&
        (SYSCTL_A->MAINFLASH_SIZE != 0x100000) &&
        (SYSCTL_A->MAINFLASH_SIZE != 0x080000))
    {
        error += 2;
    }

    // If there are any errors, Hold the LED ON
    //  Else blink the P1.0 LED forever for errors
    //
    if (error > 0)
    {
        while (1)
        {
            // Sets P1.0 for any errors
            P1->OUT |= BIT0;
        }
    }
    else
    {
        while (1)
        {
            P1->OUT ^= BIT0;                // XOR P1.0
            for (i = 20000; i > 0; i--);    // Delay
        }
    }
}
