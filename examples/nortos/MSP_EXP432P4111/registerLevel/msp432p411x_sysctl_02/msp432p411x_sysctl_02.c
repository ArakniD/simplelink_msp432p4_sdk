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
//   MSP432P411 Demo - Enable/disable SRAM bank accesses
//
//   Description: Configure SRAM banks to enable & disable selected banks.
//   Unused SRAM banks can be disabled to minimize power consumption.
//	 This example assumes use of 4 Banks of 64k (256kB SRAM configuration)
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
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
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
    // Banks 3 & below are enabled (each Bank=64kB, x4 = 256kB)
    SYSCTL_A->SRAM_BANKEN_CTL0 = SYSCTL_A_SRAM_BANKEN_CTL0_BNK0_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK1_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK2_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK3_EN;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL_A->SRAM_STAT & SYSCTL_A_SRAM_STAT_BNKEN_RDY));

    // Write & read back from an SRAM memory location in bank 3
    address = (uint32_t *) 0x2003EFFC;
    *address = 0xDEADBEEF;
    i = *address;
    if (i != 0xDEADBEEF)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Banks 0 & 1 are enabled, Banks 2 & 3 are disabled
    SYSCTL_A->SRAM_BANKEN_CTL0 = SYSCTL_A_SRAM_BANKEN_CTL0_BNK0_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK1_EN;
    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL_A->SRAM_STAT & SYSCTL_A_SRAM_STAT_BNKEN_RDY));

    // Previous memory location in Bank 3 should return 0 now
    i = *address;
    if (i != 0)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Write & read back from an SRAM memory location in bank 1
    address = (uint32_t *) 0x20016FFC;
    *address = 0xCAFEBABE;
    i = *address;
    if (i != 0xCAFEBABE)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Enable only SRAM bank 0 retention prior to going to LPM3 (Deep-sleep)
    // SYSCTL_A->SRAM_BLKRET_CTL0 |= SYSCTL_A_SRAM_BLKRET_CTL0_BLK0_EN;
     SYSCTL_A->SRAM_BANKEN_CTL0 = SYSCTL_A_SRAM_BANKEN_CTL0_BNK0_EN;
     SYSCTL_A->SRAM_BLKRET_CTL0 = 0x000000FF;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL_A->SRAM_STAT & SYSCTL_A_SRAM_STAT_BNKEN_RDY));

    // Write & read back from an SRAM memory location in bank 1
    // the read back value should be 0
    i = *address;
    if (i != 0x0)
    {
        P1->OUT |= BIT0;                    // P1.0 LED is on
        while(1);
    }

    // Turn on all SRAM banks for active mode usage.
    // Banks 3 & below are enabled (each Bank=64kB, x4 = 256kB)
    SYSCTL_A->SRAM_BANKEN_CTL0 = SYSCTL_A_SRAM_BANKEN_CTL0_BNK0_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK1_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK2_EN
                                | SYSCTL_A_SRAM_BANKEN_CTL0_BNK3_EN;

    // Check to ensure the SRAM banks are ready after
    // enable/disable of SRAM bank
    while(!(SYSCTL_A->SRAM_STAT & SYSCTL_A_SRAM_STAT_BNKEN_RDY));

    while(1)
    {
        P1->OUT ^= BIT0;                    // Toggle P1.0 LED
        for (i = 100000; i > 0; i--);       // Delay
    }
}

