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
//  MSP432P401 Demo - ADC14, Window Comparator, 2.5V ref
//
//  Description; A1 is sampled in single ch/ single conversion mode.
//  Window comparator is used to generate interrupts to
//  indicate when the input voltage goes above the High_Threshold or below the
//  Low_Threshold or is in between the high and low thresholds. TimerA0 is used
//  as an interval timer used to control the LED at P1.0 to toggle slow/fast
//  or turn off according to the ADC12 Hi/Lo/IN interupts.
//
//               MSP432P401x
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//           |                 |
//       >---|P5.4/A1     P1.0 |--> LED
//
//   William Goh
//   Texas Instruments Inc.
//   Oct 2016 (updated) | September 2015 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

#define High_Threshold 0x3333               // ~2V
#define Low_Threshold  0x1999               // ~1V

volatile unsigned int SlowToggle_Period = 10000-1;
volatile unsigned int FastToggle_Period = 1000-1;


int main(void) {
    volatile unsigned int i;
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // GPIO Setup
    P1->OUT &= ~BIT0;                       // Clear LED to start
    P1->DIR |= BIT0;                        // Set P1.0/LED to output
    P5->SEL1 |= BIT4;                       // Configure P5.4 for ADC A1
    P5->SEL0 |= BIT4;

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);  // Enable ADC interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);  // Enable Timer_A0 interrupt in NVIC module

    // Configure internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);// If ref generator busy, WAIT
    REF_A->CTL0 |= REF_A_CTL0_VSEL_3 |      // Select internal ref = 2.5V
            REF_A_CTL0_ON;                  // Internal Reference ON
    for (i = 75; i > 0; i--);               // Delay (~75us) for Ref to settle

    // Configure ADC14
    // tsample = 16ADC14CLK cycles, tconvert = 16 ADC12CLK cycles
    // software trigger for SOC, MODOSC, single ch-single conversion,
    // tsample controlled by SHT0x settings
    // Channel 1, reference = internal, enable window comparator
    // Set thresholds for ADC14 interrupts
    // Enable Interrupts
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON | ADC14_CTL0_SHS_0 | ADC14_CTL0_SSEL_0 | ADC14_CTL0_CONSEQ_0;
    ADC14->CTL1 =  ADC14_CTL1_RES_3;

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1 | ADC14_MCTLN_VRSEL_1 | ADC14_MCTLN_WINC;
    ADC14->HI0 = High_Threshold;
    ADC14->LO0 = Low_Threshold;
    ADC14->IER1 |= ADC14_IER1_HIIE | ADC14_IER1_LOIE | ADC14_IER1_INIE;

    // Configure TA0 period-timer for LED toggle
    // CCR0 interrupt enabled
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    // ACLK, clear TBR
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 | TIMER_A_CTL_CLR;

    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    while (1)
    {
        for (i = 20000; i > 0; i--);        // Delay
        // Start sampling/conversion
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        __sleep();

        __no_operation();                   // For debugger
    }
}

// ADC14 interrupt service routine

void ADC14_IRQHandler(void) {
    if (ADC14->IFGR1 & ADC14_IFGR1_HIIFG)
    {
        // Clear interrupt flag
        ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRHIIFG;
        // Turn off Timer
        TIMER_A0->CTL &= ~TIMER_A_CTL_MC_1;
        // Set Timer Period for fast LED toggle
        TIMER_A0->CCR[0] = FastToggle_Period;
        TIMER_A0->CTL |= TIMER_A_CTL_MC_1;
    }
    if (ADC14->IFGR1 & ADC14_IFGR1_LOIFG)
    {
        // Clear interrupt flag
        ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRLOIFG;
        // Turn off Timer
        TIMER_A0->CTL &= ~TIMER_A_CTL_MC_1;
        // Turn off LED
        P1->OUT &= ~BIT0;
    }
    if (ADC14->IFGR1 & ADC14_IFGR1_INIFG)
    {
        // Clear interrupt flag
        ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRINIFG;
        // Turn off Timer
        TIMER_A0->CTL &= ~TIMER_A_CTL_MC_1;
        // Set Timer Period for slow LED toggle
        TIMER_A0->CCR[0] = SlowToggle_Period;
        TIMER_A0->CTL |= TIMER_A_CTL_MC_1;
    }
}

// Timer A0 interrupt service routine

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    P1->OUT ^= BIT0;
}

