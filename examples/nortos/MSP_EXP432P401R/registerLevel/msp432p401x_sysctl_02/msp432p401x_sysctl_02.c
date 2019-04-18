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
//   MSP432P401 Demo - Enable/disable SRAM bank accesses
//
//   Description: Configure SRAM banks to enable & disable selected banks.
//   Unused SRAM banks can be disabled to minimize power consumption.
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

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
    SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK7_EN;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKEN & SYSCTL_SRAM_BANKEN_SRAM_RDY));

    // Write & read back from an SRAM memory location in bank 7
    address = (uint32_t *) 0x2000EFFC;
    *address = 0xDEADBEEF;
    i = *address;
    if (i != 0xDEADBEEF)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Banks 3 & below are enabled, Banks 7-4 are disabled
    SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK3_EN;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKEN & SYSCTL_SRAM_BANKEN_SRAM_RDY));

    // Previous memory location in Bank 7 should return 0 now
    i = *address;
    if (i != 0)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Write & read back from an SRAM memory location in bank 3
    address = (uint32_t *) 0x20006FFC;
    *address = 0xCAFEBABE;
    i = *address;
    if (i != 0xCAFEBABE)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Enable bank 0 only
    SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK0_EN ;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKEN & SYSCTL_SRAM_BANKEN_SRAM_RDY));

    // Write & read back from an SRAM memory location in bank 3
    // the read back value should be 0
    i = *address;
    if (i != 0x0)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Turn on all SRAM banks for active mode usage.
    // Banks 7 & below are enabled
    SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK7_EN;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL->SRAM_BANKEN & SYSCTL_SRAM_BANKEN_SRAM_RDY));

    while(1)
    {
        P1->OUT ^= BIT0;                    // Toggle P1.0 LED
        for (i = 100000; i > 0; i--);       // Delay
    }
}

