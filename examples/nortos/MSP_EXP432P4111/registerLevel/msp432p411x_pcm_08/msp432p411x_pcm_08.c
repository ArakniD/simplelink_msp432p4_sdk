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
//   MSP432P411 Demo - Use DC-DC Regulator
//
//
//   Description: Configure PCM module to use the DC-DC regulator instead of
//   the default LDO. Note that DC-DC usage requires a 4.7uH inductor connected
//   between VSW and VCORE pins. VCORE pin still requires its regular 100nF and
//   4.7uF capacitors.
//
//   Note: the code in this example assumes the device is currently in LDO mode
//   AM_LDO_VCORE0 or AM_LDO_VCORE1 (Active Mode using LDO, VCore=0/1 respectively)
//
//   Transition from DCDC mode to Low-Frequency Mode requires intermediate
//   transition through LDO mode. For more information refer to the PCM chapter
//   in the device user's guide.
//
//   AM_DCDC_VCORE1  <--@-->  AM_LDO_VCORE1  <-----> AM_LF_VCORE1
//                                  ^
//                                  |
//                                  @
//                                  |
//                                  v
//   AM_DCDC_VCORE0  <----->  AM_LDO_VCORE0* <-----> AM_LF_VCORE0
//
//   *: power state condition after reset
//   @: transitions demonstrated in this code example
//
//                MSP432P411x
//             -----------------
//         /|\|                 |
//          | |           VCORE |------
//          --|RST              |     |
//            |                 |    4.7uH
//            |                 |     |
//            |             VSW |------
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017 (updated) | November 2013 (created)
//   Built with CCSv7.1, IAR 8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

void error(void);

int main(void)
{
    uint32_t currentPowerState;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Terminate all remaining pins on the device
    P1->DIR |= 0xFF; P1->OUT = 0;
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

    // Get current power state
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;

    // Transition to DCDC from current LDO power state properly
    switch (currentPowerState)
    {
        case PCM_CTL0_CPM_0:                // AM_LDO_VCORE0, need to switch to AM_DCDC_VCORE0
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_4;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
                error();                    // Error if transition was not successful
            break;
        case PCM_CTL0_CPM_1:                // AM_LDO_VCORE1, need to switch to AM_DCDC_VCORE1
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_5;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
                error();                    // Error if transition was not successful
            break;
        case PCM_CTL0_CPM_4:                // Device is already in AM_LF_VCORE0
            break;
        case PCM_CTL0_CPM_5:                // Device is already in AM_LF_VCORE1
            break;
        default:                            // Device is in some other state, which is unexpected
            error();
    }

    P1->OUT |= BIT0;                        // LDO --> DCDC switching sequence successful
    __no_operation();
    while(1);
}

void error(void)
{
    volatile uint32_t i;

    while (1)
    {
        P1->OUT ^= BIT0;
        for(i = 0; i < 20000; i++);         // Blink LED forever
    }
}
