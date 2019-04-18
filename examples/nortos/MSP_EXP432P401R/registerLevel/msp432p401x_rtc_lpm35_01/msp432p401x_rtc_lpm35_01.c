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
//   MSP432P401 Demo - RTC, LPM3.5, & alarm
//
//   Description: The RTC module is used to set the time, start RTC operation,
//   and read the time from the respective RTC registers. Software will set the
//   original time to 11:59:45 am on Friday October 7, 2011. Then the RTC will
//   be activated through software, and an alarm will be created for the next
//   minute (12:00:00 pm). The device will then enter LPM3.5 awaiting
//   the event interrupt. Upon being woken up by the event, the LED on the board
//   will be set.
//
//   NOTE: To ensure that LPM3.5 is entered properly, you would need to exit
//         debug mode first and use an external power supply.
//
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~3MHz
//
//                MSP432P401x
//
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> LED
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"

void boardInit(void);
void rtcInit(void);
void enterLpm35(void);
void wakeUpLPM35(void);

int main(void)
{
    volatile unsigned int i;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Check if the MCU is waking-up from LPM3.5
    if (RSTCTL->PCMRESET_STAT & RSTCTL_PCMRESET_STAT_LPM35)
    {
        wakeUpLPM35();
    }
    else
    {
        // Else it is a cold-start

        // Initialize GPIO and clocks
        boardInit();

        // Initialize RTC
        rtcInit();

        // Turn off PSS high-side & low-side supervisors
        PSS->KEY = PSS_KEY_KEY_VAL;
        PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
        PSS->KEY = 0;

        // Enter LPM3.5
        enterLpm35();
    }

    // Code should NOT get here. This means that LPM3.5 was not properly entered.
    // Ensure that an external power supply was ued. Or else JTAG will put the CPU
    // in LPM0 mode.

    // Stop the RTC
    RTC_C->CTL0 = RTC_C_KEY_VAL;            // Unlock RTC_C module
    RTC_C->CTL13 |= RTC_C_CTL13_HOLD;       // RTC hold
    RTC_C->CTL0 &= ~(RTC_C_CTL0_KEY_MASK);  // Lock RTC_C module

    // Blink LED slowly if LPM3.5 not entered properly
    while(1)
    {
        P1->OUT ^= BIT0;                    // XOR P1.0
        for (i = 500000; i > 0; i--);       // Delay
    }
}

void wakeUpLPM35(void)
{
    // Re-initialize the GPIO and system clocks
    boardInit();

    // Configure RTC_C but ensure the IFG flag is not cleared
    RTC_C->CTL0 = (RTC_C->CTL0 & ~(RTC_C_CTL0_KEY_MASK)) |
            RTC_C_KEY_VAL |                 // Unlock RTC key protected registers
            RTC_C_CTL0_TEVIE;               // Enable RTC time event interrupt
    RTC_C->CTL13 |= RTC_C_CTL13_BCD |       // BCD mode
            RTC_C_CTL13_TEV_0 |             // Set RTCTEV for 1 minute alarm event interrupt
            RTC_C_CTL13_HOLD;               // RTC hold

    RTC_C->CTL13 &= ~(RTC_C_CTL13_HOLD);    // Start RTC calendar mode
    RTC_C->CTL0 &= ~(RTC_C_CTL0_KEY_MASK);  // Lock the RTC registers

    // Clear LPM lock
    PCM->CTL1 = PCM_CTL1_KEY_VAL;

    // Re-enable the interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((RTC_C_IRQn) & 31);
}

void rtcInit(void)
{
    // Configure RTC_C
    RTC_C->CTL0 = RTC_C_KEY_VAL |           // Unlock RTC key protected registers
            RTC_C_CTL0_TEVIE;               // Enable RTC time event interrupt
    RTC_C->CTL13 |= RTC_C_CTL13_BCD |       // BCD mode
            RTC_C_CTL13_TEV_0 |             // Set RTCTEV for 1 minute alarm event interrupt
            RTC_C_CTL13_HOLD;               // RTC hold

    RTC_C->YEAR = 0x2016;                   // Year = 0x2016
    RTC_C->DATE = (0x10 << RTC_C_DATE_MON_OFS) | // Month = 0x10 = October
            (0x07 | RTC_C_DATE_DAY_OFS);    // Day = 0x07 = 7th
    RTC_C->TIM1 = (0x05 << RTC_C_TIM1_DOW_OFS) | // Day of week = 0x05 = Friday
            (0x11 << RTC_C_TIM1_HOUR_OFS);  // Hour = 0x11
    RTC_C->TIM0 = (0x59 << RTC_C_TIM0_MIN_OFS) | // Minute = 0x59
            (0x45 << RTC_C_TIM0_SEC_OFS);   // Seconds = 0x45

    RTC_C->CTL13 &= ~(RTC_C_CTL13_HOLD);    // Start RTC calendar mode
    RTC_C->CTL0 &= ~(RTC_C_CTL0_KEY_MASK);  // Lock the RTC registers

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((RTC_C_IRQn) & 31);
}

void boardInit(void)
{
    // GPIO Port Configuration for lowest power configuration
    P1->OUT = 0x00; P1->DIR = 0xFF;
    P2->OUT = 0x00; P2->DIR = 0xFF;
    P3->OUT = 0x00; P3->DIR = 0xFF;
    P4->OUT = 0x00; P4->DIR = 0xFF;
    P5->OUT = 0x00; P5->DIR = 0xFF;
    P6->OUT = 0x00; P6->DIR = 0xFF;
    P7->OUT = 0x00; P7->DIR = 0xFF;
    P8->OUT = 0x00; P8->DIR = 0xFF;
    P9->OUT = 0x00; P9->DIR = 0xFF;
    P10->OUT = 0x00; P10->DIR = 0xFF;
    PJ->OUT = 0x00; PJ->DIR = 0xFF;

    // Initialize LFXT1
    PJ->SEL0 |= BIT0 | BIT1;                // Select for LFXT ports
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL2 |= CS_CTL2_LFXT_EN;            // LFXT on

    // Loop until XT1, XT2 & DCO fault flag is cleared
    do
    {
       // Clear XT2,XT1,DCO fault flags
       CS->CLRIFG |= CS_CLRIFG_CLR_DCOR_OPNIFG | CS_CLRIFG_CLR_HFXTIFG |
               CS_CLRIFG_CLR_LFXTIFG | CS_CLRIFG_CLR_FCNTLFIFG;
       SYSCTL->NMI_CTLSTAT &= ~ SYSCTL_NMI_CTLSTAT_CS_SRC;
    } while ((SYSCTL->NMI_CTLSTAT | SYSCTL_NMI_CTLSTAT_CS_FLG)
            && (CS->IFG & CS_IFG_LFXTIFG)); // Test oscillator fault flag

    // Select ACLK as LFXTCLK
    CS->CTL1 &= ~(CS_CTL1_SELA_MASK) | CS_CTL1_SELA_0;
    CS->KEY = 0;                            // Lock CS module from unintended accesses
}

void enterLpm35(void)
{
    // Set the Power Mode 3.5
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_LPMR__LPM35;

    // Setting the sleep deep bit
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);

    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures that SLEEPDEEP occurs immediately
    __DSB();

    __sleep();
    __no_operation();                       // For debugger
}

void RTC_C_IRQHandler(void)
{
    volatile unsigned int i;

    if (RTC_C->CTL0 & RTC_C_CTL0_TEVIFG)
    {
        // If woken up at noon, set LED and stay loop there
        if((((RTC_C->TIM1 & (RTC_C_TIM1_HOUR_LD_MASK | RTC_C_TIM1_HOUR_HD_MASK))>>RTC_C_TIM1_HOUR_OFS) == 0x12) &&
        (((RTC_C->TIM0 & (RTC_C_TIM0_MIN_LD_MASK | RTC_C_TIM0_MIN_HD_MASK))>>RTC_C_TIM0_MIN_OFS) == 0x00) &&
        (((RTC_C->TIM0 & (RTC_C_TIM0_SEC_LD_MASK | RTC_C_TIM0_SEC_HD_MASK))>>RTC_C_TIM0_SEC_OFS) == 0x00))
        {
            P1->OUT |= BIT0;                // Set P1.0 on
            while(1);
        }
        // If values do not match, blink LED fast
        else
        {
            while(1)                        // continuous loop
            {
                P1->OUT ^= BIT0;            // XOR P1.0
                for (i = 20000; i > 0; i--);// Delay
            }
        }
    }
}
