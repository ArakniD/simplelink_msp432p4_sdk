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
 * MSP432 Power Supply System
 *
 * Description: This code example shows how to use the high side
 * monitor in the PSS module in MSP432
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P4.4  |---> SVMHOUT
 *            |                  |
 *            |            P1.0  |---> LED
 *            |            P2.0  |---> LED
 *            |                  |
 *            |                  |
 *
 *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Typical values will vary from part to part (so it is very important to
 * check the SVSH section of the data sheet. For reference only, the typical
 * MSP432 101 values are listed below:
 *  - 0 --> 1.57V
 *  - 1 --> 1.62V
 *  - 2 --> 1.83V
 *  - 3 --> 2V
 *  - 4 --> 2.25V
 *  - 5 --> 2.4V
 *  - 6 --> 2.6V
 *  - 7 --> 2.8V */
#define TRIGGER_VOLTAGE_0_AT_1_57V       (0)
#define TRIGGER_VOLTAGE_1_AT_1_62V       (1)
#define TRIGGER_VOLTAGE_2_AT_1_83V       (2)
#define TRIGGER_VOLTAGE_3_AT_2_00V       (3)
#define TRIGGER_VOLTAGE_4_AT_2_25V       (4)
#define TRIGGER_VOLTAGE_5_AT_2_40V       (5)
#define TRIGGER_VOLTAGE_6_AT_2_60V       (6)
#define TRIGGER_VOLTAGE_7_AT_2_80V       (7)

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Configure SVMHOUT on P4.4 for testing purposes */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4,
                                                    GPIO_PIN4, GPIO_SECONDARY_MODULE_FUNCTION);

    /* Disables the PSS interrupt as a source of NMI
     * using PSS_IRQHandler */
    MAP_SysCtl_disableNMISource(SYSCTL_PSS_SRC);

    /* Disables high side voltage supervisor/monitor */
    MAP_PSS_disableHighSide();

    /* Sets the high side voltage supervisor to monitor mode */
    MAP_PSS_enableHighSideMonitor();

    /* Sets the voltage level at which the high side of the device voltage */
    MAP_PSS_setHighSideVoltageTrigger(TRIGGER_VOLTAGE_1_AT_1_62V);

    /* False if signal should be high when SVSMHIFG is set */
    MAP_PSS_enableHighSidePinToggle(false);

    /* Make sure that the SVSMHIFG bit is cleared before enabling interrupt.
     * Otherwise an unexpected NMI may be seen due to an earlier dip in DVCC while
     * interrupt was disabled. */
    MAP_PSS_clearInterruptFlag();

    /* Enables high side voltage supervisor/monitor */
    MAP_PSS_enableHighSide();

    /* Enables the power supply system interrupt source */
    MAP_PSS_enableInterrupt();

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_PSS);
    MAP_Interrupt_enableMaster();

    /* GPIO's for testing purposes */
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);

    while(1)
    {
        /* Toggle GPIO P1.0 */
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        __delay_cycles(2000000);
    }
}

void PSS_IRQHandler(void)
{
    /* Turn off P1.0 */
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    while(1)
    {
        /* Toggle GPIO P2.0 when getting the SVS interrupt */
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
        __delay_cycles(2000000);
    }
}
