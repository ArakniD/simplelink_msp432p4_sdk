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
/*******************************************************************************
 * MSP432 PCM - Going to LPM3 wake-up from EUSCIA0 UART.
 *
 * Description: In this very simple example, the use of the PCM API to go to
 * LPM3 is demonstrated. The device is configured for EUSCI A0 UART interrupt
 * (to wake the device up when UART characters are received). P1.0 LED is toggled
 * when the character 'U' is received. The device is put into LPM3 using the
 * PCM_gotoLPM3 function. Furthermore, the device is configured in a manner to
 * minimize power consumption and achieve datasheet specifications with
 * regard to power consumption.
 *
 *
 *               MSP432P4111
 *             -----------------
 *            |                 |
 *            |                 |
 *            |                 |
 *       RST -|     P1.3/UCA0TXD|----> PC (echo)
 *            |                 |
 *            |                 |
 *            |     P1.2/UCA0RXD|<---- PC
 *            |             P1.0|---> LED
 *            |                 |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

//![Simple UART Config]
/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_ACLK,           // ACLK Clock Source = 32768Hz
        3,                                       // BRDIV = 3
        0,                                       // UCxBRF = 0
        146,                                     // UCxBRS = 146
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION,  // Low-Freq Baud Rate Generation
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};
//![Simple UART Config]

static volatile uint16_t rxData = 0;

int main(void)
{
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Terminating all remaining pins to minimize power consumption. This is
        done by register accesses for simplicity and to minimize branching API
        calls */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ, PIN_ALL16);

    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Set P1.0 as output for LED */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Starting LFXT and sourcing ACLK from it */
    MAP_CS_setExternalClockSourceFrequency(32000,48000000);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting LFXT to lowest drive strength and current consumption */
    MAP_CS_startLFXT(CS_LFXT_DRIVE0);

    MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Enable retention for block 0 and block 31, disable retention for
     * blocks 1-30. */
    MAP_SysCtl_A_disableSRAMRetention(0x20002000, 0x2003C000);

    /* Disabling high side voltage monitor/supervisor */
    MAP_PSS_disableHighSide();

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Enable EUSCIA0 Interrupt */
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);

    /* Enabling "rude" mode which forces device to go to sleep regardless of
     * outstanding clock requests
     */
    MAP_PCM_enableRudeMode();

    /* Enable Sleep on ISR Exit */
    MAP_Interrupt_enableSleepOnIsrExit();

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    /* Going to LPM3 */
    while (1)
    {

        /* Note that while this examples just goes to LPM3, LPM4 is essentially
            just LPM3 with WDT_A/RTC_C disabled. For convenience, the user can 
            use the MAP_PCM_gotoLPM4() function if they want the API to handle
            the disabling of these components */
        MAP_PCM_gotoLPM3();
    }
}

/* EUSCI A0 UART ISR - Echoes data back to PC host and watches for 'U' character */
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        rxData = MAP_UART_receiveData(EUSCI_A0_BASE);
        MAP_UART_transmitData(EUSCI_A0_BASE, rxData);

        if(rxData == 'U'){
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    }

}
