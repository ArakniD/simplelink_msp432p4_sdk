/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//****************************************************************************
//
// main.c - MSP-EXP432P401R + Battery Boosterpack MkII - Fuel Guage demo
//
//          Initializes BQ27441 Fuel Gauge configurations suitable for the
//          included battery and periodically prints information to serial
//          backchannel uart.
//
//****************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HAL_I2C.h"
#include "HAL_UART.h"
#include "HAL_BQ27441.h"

void CS_init(void);
void GPIO_init(void);
char *itoa(int, char*, int);


int main(void)
{
	/* Halting WDT and disabling master interrupts */
	MAP_WDT_A_holdTimer();
	MAP_Interrupt_disableMaster();

	GPIO_init();
	CS_init();

	__delay_cycles(1000000);

	/* Initialize I2C */
	I2C_initGPIO();
	I2C_init();

	/* Initialize UART */
	UART_initGPIO();
	UART_init();

	__delay_cycles(1000000);

	if (!BQ27441_initConfig())
	{
		UART_transmitString("Error initializing BQ27441 Config\r\n");
		UART_transmitString("Make sure BOOSTXL-BATPAKMKII is connected and switch is flipped to \"CONNECTED\"\r\n");
	}

	/*
	 * Clear the Battery Insertion Enable (BIE) bit when Jumper JP6 BIN is not populated
	 * Comment this out if JP6 BIN is shorted
	 */
	while (!BQ27441_initOpConfig())
	{
		__delay_cycles(1000000);
		UART_transmitString("Clearing BIE in Operation Configuration\r\n");
	}

	/*
	 * When BIE is cleared, the host MCU need to issue the BAT_INSERT subcommand to indicate battery presence
	 * Comment this out if JP6 BIN is shorted
	 */
	while (!BQ27441_control(BAT_INSERT, 1000))
	{
	    __delay_cycles(1000000);
	    UART_transmitString("Sending software command to indicate Battery Insertion to BQ27441\r\n");
	}

	/* Display Battery information */
	while(1)
	{
		short result16 = 0;
		char str[64];
		UART_transmitString("\r\n");
		UART_transmitString("\r\n");
		UART_transmitString("*************************************\r\n");
		UART_transmitString("Battery Information\r\n");
		UART_transmitString("*************************************\r\n");

		/* Read Design Capacity */
		if(!BQ27441_read16(DESIGN_CAPACITY, &result16, 1000))
			UART_transmitString("Error Reading Design Capacity \r\n");
		else
		{
			sprintf(str, "Design Capacity: %dmAh\r\n", result16);
			UART_transmitString(str);
		}

		/* Read Remaining Capacity */
		if(!BQ27441_read16(REMAINING_CAPACITY, &result16, 1000))
			UART_transmitString("Error Reading Remaining Capacity \r\n");
		else
		{
			sprintf(str, "Remaining Capacity: %dmAh\r\n", result16);
			UART_transmitString(str);
		}

		/* Read State Of Charge */
		if(!BQ27441_read16(STATE_OF_CHARGE, &result16, 1000))
			UART_transmitString("Error Reading State Of Charge \r\n");
		else
		{
			sprintf(str, "State of Charge: %d%%\r\n", (unsigned short)result16);
			UART_transmitString(str);
		}

		/* Read Temperature */
		if(!BQ27441_read16(TEMPERATURE, &result16, 1000))
			UART_transmitString("Error Reading Temperature \r\n");
		else
		{
			sprintf(str, "Temperature: %dC\r\n", result16/10 - 273);
			UART_transmitString(str);
		}

		/* Read Voltage */
		if(!BQ27441_read16(VOLTAGE, &result16, 1000))
			UART_transmitString("Error Reading Voltage \r\n");
		else
		{
			sprintf(str, "Voltage: %dmV\r\n", result16);
			UART_transmitString(str);
		}

		/* Read Average Current */
		if(!BQ27441_read16(AVERAGE_CURRENT, &result16, 1000))
			UART_transmitString("Error Reading Average Current \r\n");
		else
		{
			sprintf(str, "Average Current: %dmA\r\n", result16);
			UART_transmitString(str);
			if (result16 > 0) {
				UART_transmitString("Status : charging\r\n");
			} else {
				UART_transmitString("Status : discharging\r\n");
			}
		}

		__delay_cycles(20000000);
	}
}


/* Initializes Clock System */
void CS_init()
{
    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}


/* Initializes GPIO */
void GPIO_init()
{
	/* Terminate all GPIO pins to Output LOW to minimize power consumption */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);
}
