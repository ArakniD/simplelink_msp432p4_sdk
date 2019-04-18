/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Comparator - Interrupt Capability; Vcompare is compared against
 *                    internal 1.2V reference
 *
 * Description: Use COMPE and internal reference to determine if input'Vcompare'
 * is high or low.  For the first time, when Vcompare exceeds the 1.2V internal
 * reference, CEIFG is set and device enters the COMPE ISR. In the ISR, CEIES is
 * toggled such that when Vcompare is less than 1.2V internal reference;
 * CEIFG is set. The LED is toggled inside the ISR to illustrate the change.
 * The filter feature of the Comp module is used to filter out any noise on
 * the line and assure no spurious interrupts occur.
 *
 *                 MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST     P5.7/C1.6 |<--Vcompare
 *            |                  |
 *            |              P1.0|----> 'high'(Vcompare>1.2V); 'low'(Vcompare<1.2)
 *            |                  |  |
 *            |                  |  | LED 'ON'(Vcompare>1.2V); 'OFF'(Vcompare<1.2V)
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

//![Simple Comp Config] 
/* Comparator configuration structure */
const COMP_E_Config compConfig =
{
        COMP_E_VREF,                  // Positive Input Terminal
        COMP_E_INPUT6,                // Negative Input Terminal
        COMP_E_FILTEROUTPUT_DLYLVL4,  // Delay Level 4 Filter (max)
        COMP_E_NORMALOUTPUTPOLARITY   // Normal Output Polarity
};
//! [Simple Comp Config] 

int main(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    /* Set P1.0 as an output pin for LED */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Set P5.7 to be comparator in (C1.6) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN7,
            GPIO_TERTIARY_MODULE_FUNCTION);

    //![Simple COMP_E Example]

    /* Initialize the Comparator module
     *  Comparator Instance 1
     *  Reference Voltage to Postive(+) Terminal
     *  Pin CE16 to negative(-) Terminal
     *  Normal Power Mode
     *  Output Filter On with max delay
     *  Non-Inverted Output Polarity
     */
    MAP_COMP_E_initModule(COMP_E1_BASE, &compConfig);

    /*
     * Base Address of Comparator E,
     * Reference Voltage of 1.2 V,
     * Lower Limit of 1.2*(32/32) = 1.2V,
     * Upper Limit of 1.2*(32/32) = 1.2V
     */
    MAP_COMP_E_setReferenceVoltage(COMP_E1_BASE, COMP_E_VREFBASE1_2V, 32, 32);

    /* Enable COMP_E Interrupt on default rising edge for CEIFG */
    MAP_COMP_E_setInterruptEdgeDirection(COMP_E1_BASE, COMP_E_RISINGEDGE);

    /*  Enable Interrupts
     *  Comparator Instance 1,
     *  Enable COMPE Interrupt on default rising edge for CEIFG
     */
    MAP_COMP_E_clearInterruptFlag(COMP_E1_BASE, COMP_E_OUTPUT_INTERRUPT);
    MAP_COMP_E_enableInterrupt(COMP_E1_BASE, COMP_E_OUTPUT_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_COMP_E1);
    MAP_Interrupt_enableMaster();
    //![Simple COMP_E Example]

    /* Allow power to Comparator module */
    MAP_COMP_E_enableModule(COMP_E1_BASE);

    while (1)
    {
        /* Goto Sleep */
        MAP_PCM_gotoLPM0();
    }
}

/******************************************************************************
 *
 * This is the COMP_VECTOR interrupt vector service routine.
 *
 ******************************************************************************/
void COMP_E1_IRQHandler(void)
{

    /* Toggle the edge at which an interrupt is generated */
    MAP_COMP_E_toggleInterruptEdgeDirection(COMP_E1_BASE);

    /* Clear Interrupt flag */
    MAP_COMP_E_clearInterruptFlag(COMP_E1_BASE, COMP_E_OUTPUT_INTERRUPT_FLAG);

    /* Toggle P1.0 output pin */
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

}
