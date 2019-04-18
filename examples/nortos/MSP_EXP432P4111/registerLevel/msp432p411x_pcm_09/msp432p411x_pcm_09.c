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
//   MSP432P411 Demo - Device operates at 48MHz with DC-DC Regulator
//
//   Description: Configure device to operate at 48MHz and use the DC-DC
//   regulator instead of the default LDO. The following steps must be taken in
//   sequence shown below to ensure proper Flash operation at 48MHz and with
//   DC-DC regulator:
//   1. VCORE LEVEL = 1
//   2. Switch from LDO to DC-DC.
//   3. Configure flash Wait-state = 1 (Flash max frequency = 24MHz)
//   4. Configure DCO to 48MHz
//   5. Switch MCLK to use DCO as source
//
//   Note: DC-DC usage requires a 4.7uH inductor connected between VSW and VCORE
//   pins. VCORE pin still requires its regular 100nF and 4.7uF capacitors.
//
//   Note: the code in this example assumes the device is currently in AM0_LDO
//   (Active Mode using LDO, VCore=0/1 respectively). Refer to previous
//   MSP432P411_pcm_0x code examples for more complete power state transitions
//   between active modes.
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
//            |           VSW   |------
//            |                 |
//            |           P1.0  |------> LED
//            |                 |
//            |           P4.3  |------> MCLK
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
											
    // Configure P5.3 as ADC-in. Avoid driving this pin on the MSP432P4111
    //   LaunchPad since it's connected to the LMT70 temp sensor.  
    //   Driving P5.3 as a digital-low can result in 1mA+ of current on Vcc.
    P5->SEL1 |= BIT3;
    P5->SEL0 |= BIT3;
	
    // NOTE: This example assumes the default power state is AM_LDO_VCORE0.

    // Step 1: Transition to VCORE Level 1: AM_LDO_VCORE0 --> AM_LDO_VCORE1

    // Get current power state, if it's not AM_LDO_VCORE0, error out
    currentPowerState = PCM->CTL0 & PCM_CTL0_CPM_MASK;

    // Skip the power mode config if PCM is already in AM1_DCDC
    if (currentPowerState != PCM_CTL0_CPM_5)
    {
        if (currentPowerState != PCM_CTL0_CPM_0)
            error();

        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
            error();                            // Error if transition was not successful
        if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1)
            error();                            // Error if device is not in AM1_LDO mode

        // Step 2: Transition from AM1_LDO to AM1_DCDC
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_5;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
            error();                            // Error if transition was not successful
        if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_5 )
            error();                            // Error if device is not in AM_DCDC_VCORE1 mode
    }

    // Step 3: Configure Flash wait-state to 1 for both banks 0 & 1
    FLCTL_A->BANK0_RDCTL = (FLCTL_A->BANK0_RDCTL & ~(FLCTL_A_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_A_BANK0_RDCTL_WAIT_1;
    FLCTL_A->BANK1_RDCTL = (FLCTL_A->BANK0_RDCTL & ~(FLCTL_A_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_A_BANK1_RDCTL_WAIT_1;

    // Step 4&5: Configure DCO to 48MHz, ensure MCLK uses DCO as source
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    // Select MCLK = DCO, no divider
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) |
            CS_CTL1_SELM_3;
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    // Output MCLK to port pin to demonstrate 48MHz operation
    P4->DIR |= BIT3;
    P4->SEL0 |=BIT3;                        // Output MCLK
    P4->SEL1 &= ~(BIT3);

    while (1)                               // continuous loop
    {
        P1->OUT |= BIT0;                    // Blink P1.0 LED
    }
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
