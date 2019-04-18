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
//   MSP432P401 Demo - Configure and use the Memory Protection Unit (MPU)
//
//   Description: Confirm the number of MPU regions in the device. Enable &
//   disable different MPU regions with different attributes (read/write/execute).
//   If device does not have the right number of MPU regions, P1.0 LEG toggles
//   indefinitely. After programming the MPU region to have execute-only access,
//   a read attempt triggers MPU fault interrupt, turning on the P1.0 LED.
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
#include <stdint.h>
#include "ti/devices/msp432p4xx/inc/msp.h"

#define MPU_RGN_SIZE_4K             (11 << 1)
#define MPU_RGN_PERM_NOEXEC         0x10000000
#define MPU_RGN_PERM_PRV_RW_USR_RO  0x02000000
#define MPU_RGN_PERM_PRV_RO_USR_RO  0x06000000
#define MPU_SUB_RGN_DISABLE_0       0x00000100
#define MPU_SUB_RGN_DISABLE_1       0x00000200
#define MPU_SUB_RGN_DISABLE_2       0x00000400
#define MPU_SUB_RGN_DISABLE_3       0x00000800
#define MPU_SUB_RGN_DISABLE_4       0x00001000
#define MPU_SUB_RGN_DISABLE_5       0x00002000
#define MPU_SUB_RGN_DISABLE_6       0x00004000
#define MPU_SUB_RGN_DISABLE_7       0x00008000
#define MPU_RGN_PERM_PRV_NO_USR_NO  0x00000000

uint32_t foo[4] = { 1, 1, 1, 1 };

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    P1->OUT &= ~BIT0;                       // Set P1.0 as low
    P1->DIR |= BIT0;                        // P1.0 set as output

    // Confirm that device does have 8 MPU regions
    if (((MPU->TYPE & MPU_TYPE_DREGION_Msk)>> 8) !=8 )
    {
        while (1)                           // Spin in here forever
        {
            P1->OUT ^= BIT0;                // Toggle LED
            for (i = 20000; i > 0; i--);    // Delay
        }
    }

    // Program region 0 with starting add = 0x20000
    MPU->RBAR = 0x20000 | 0 | MPU_RBAR_VALID_Msk;

    // Configure MPU flags - 4K region with no access for
    // privileged and user accesses
    MPU->RASR = (MPU_RGN_SIZE_4K |
            MPU_RGN_PERM_PRV_NO_USR_NO |
            MPU_RASR_S_Msk |
            MPU_RASR_B_Msk |
            MPU_SUB_RGN_DISABLE_7 |
            MPU_RASR_ENABLE_Msk) & ~(MPU_RASR_TEX_Msk | MPU_RASR_C_Msk);

    // Enabling Fault interrupt
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    // Enable MPU module in Privilege Default mode
    MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;

    // Enabling master interrupts
    __enable_irq();

    // Unauthorized read access should trigger MPU Fault Handler
    foo[0] = *(uint32_t*) 0x20000;

    // Should never get here. Application should have triggered
    // a MPU fault
    while (1);
}

// MPU Fault ISR Handler
void MemManage_Handler(void)
{
    P1->OUT |= BIT0;                        // Turn on P1.0 LED
    while (1);                              // Spin in here forever
}
