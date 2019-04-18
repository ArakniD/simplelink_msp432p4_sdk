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
// HAL_BQ27441.c - Hardware abstraction layer for interfacing BQ27441
//
//****************************************************************************

#include <stdio.h>
#include <stdint.h>
#include "HAL_BQ27441.h"
#include "HAL_I2C.h"
#include "HAL_UART.h"

/* Configures BQ27441 device properties */
bool BQ27441_initConfig()
{
	short result = 0;

	UART_transmitString("*************************************\r\n");
	UART_transmitString("Initializing BQ27441 Configuration\r\n");
	UART_transmitString("*************************************\r\n");

	//Default Config, DesignCapacity = 1200mAh, DesignEnergy = 1200mAh*3.7V, Terminate Voltage = 3200mV, Taper Current = 120mA
	char str[64];
	sprintf(str, "DesignCapacity = %dmAh\r\n", CONF_DESIGN_CAPACITY);
	UART_transmitString(str);
	sprintf(str, "DesignEnergy = %dmAh * 3.7 = %dJ\r\n", CONF_DESIGN_CAPACITY, CONF_DESIGN_ENERGY);
	UART_transmitString(str);
	sprintf(str, "TerminateVoltage = %dmV\r\n", CONF_TERMINATE_VOLTAGE);
	UART_transmitString(str);
	sprintf(str, "TaperRate = %dmAh/(0.1*%dmA) = %d\r\n", CONF_DESIGN_CAPACITY, CONF_TAPER_CURRENT, CONF_TAPER_RATE);
	UART_transmitString(str);

	if (!BQ27441_read16(FLAGS, &result, 1000))
		return 0;

	/* Check if ITPOR bit is set in FLAGS */
	if (result & 0x0020)
	{
		/* Instructs fuel gauge to enter CONFIG UPDATE mode. */
		if (!BQ27441_control(SET_CFGUPDATE, 1000))
			return 0;

		__delay_cycles(1000000);

		result = 0;
		/* Check if CFGUPMODE bit is set in FLAGS */
		while(!(result & 0x0010))
		{
			if (!BQ27441_read16(FLAGS, &result, 1000))
				return 0;
		}

		/* Enable Block data memory control */
		if (!BQ27441_command(BLOCK_DATA_CONTROL, 0x00, 1000))
			return 0;

		/* Set the data class to be accessed */
		if (!BQ27441_command(DATA_CLASS, 0x52, 1000))
			return 0;

		/* Write the block offset loaction */
		if (!BQ27441_command(DATA_BLOCK, 0x00, 1000))
			return 0;

		__delay_cycles(1000000);

		char old_chksum = 0;
		char new_chksum = 0;
		char tmp_chksum = 0;
		char chksum = 0;
		do
		{
			/* Read Block Data Checksum */
			if (!BQ27441_readChecksum(&old_chksum, 1000))
				return 0;

			__delay_cycles(1000000);

			/* Checksum calculation */
			tmp_chksum = old_chksum;

			short old_designCapacity = 0;
			short old_designEnergy = 0;
			short old_terminateVoltage = 0;
			short old_taperRate = 0;

			/* Read old design capacity */
			if (!BQ27441_read16(0x4A, &old_designCapacity, 1000))
				return 0;

			tmp_chksum = computeCheckSum(tmp_chksum, old_designCapacity, CONF_DESIGN_CAPACITY);

			/* Read old design energy */
			if (!BQ27441_read16(0x4C, &old_designEnergy, 1000))
				return 0;

			tmp_chksum = computeCheckSum(tmp_chksum, old_designEnergy, CONF_DESIGN_ENERGY);

			/* Read old terminate voltage */
			if (!BQ27441_read16(0x50, &old_terminateVoltage, 1000))
				return 0;

			tmp_chksum = computeCheckSum(tmp_chksum, old_terminateVoltage, CONF_TERMINATE_VOLTAGE);

			/* Read old taper rate */
			if (!BQ27441_read16(0x5B, &old_taperRate, 1000))
				return 0;

			/* Checksum calculation */
			tmp_chksum = computeCheckSum(tmp_chksum, old_taperRate, CONF_TAPER_RATE);

			/* Write new design capacity */
			if (!BQ27441_write16(0x4A, swapMSB_LSB(CONF_DESIGN_CAPACITY), 1000))
				return 0;

			/* Write new design energy */
			if (!BQ27441_write16(0x4C, swapMSB_LSB(CONF_DESIGN_ENERGY), 1000))
				return 0;

			/* Write new terminate voltage */
			if (!BQ27441_write16(0x50, swapMSB_LSB(CONF_TERMINATE_VOLTAGE), 1000))
				return 0;

			/* Write new taper rate */
			if (!BQ27441_write16(0x5B, swapMSB_LSB(CONF_TAPER_RATE), 1000))
				return 0;

			/* Checksum calculation */
			new_chksum = tmp_chksum;

			/* Write new taper rate */
			if (!BQ27441_command(BLOCK_DATA_CHECKSUM, new_chksum, 1000))
				return 0;

			__delay_cycles(1000000);

			/* Read Block Data Checksum */
			if (!BQ27441_readChecksum(&chksum, 1000))
				return 0;

			__delay_cycles(1000000);
		}
		while(new_chksum != chksum);

		/* Write new terminate voltage */
		if (!BQ27441_control(SOFT_RESET, 1000))
			return 0;

		__delay_cycles(1000000);

		result = 0;
		/* Check if CFGUPMODE bit is cleared in FLAGS */
		while(result & 0x0010)
		{
			if (!BQ27441_read16(FLAGS, &result, 1000))
				return 0;
		}

		UART_transmitString("*************************************\r\n");
		UART_transmitString("BQ27441 config inialized succesfully\r\n");
		UART_transmitString("*************************************\r\n");
		return 1;
	}
	else
	{
		UART_transmitString("*************************************\r\n");
		UART_transmitString("BQ27441 config inialized already\r\n");
		UART_transmitString("*************************************\r\n");
		return 1;
	}
}


/* Configures BQ27441 opconfig */
bool BQ27441_initOpConfig()
{
	short result = 0;

	/* Instructs fuel gauge to enter CONFIG UPDATE mode. */
	if (!BQ27441_control(SET_CFGUPDATE, 1000))
		return 0;

	__delay_cycles(1000000);

	result = 0;
	/* Check if CFGUPMODE bit is set in FLAGS */
	while(!(result & 0x0010))
	{
		if (!BQ27441_read16(FLAGS, &result, 1000))
			return 0;
	}

	/* Enable Block data memory control */
	if (!BQ27441_command(BLOCK_DATA_CONTROL, 0x00, 1000))
		return 0;

	/* Set the data class to be accessed */
	if (!BQ27441_command(DATA_CLASS, 0x40, 1000))
		return 0;

	/* Write the block offset location */
	if (!BQ27441_command(DATA_BLOCK, 0x00, 1000))
		return 0;

	__delay_cycles(1000000);

	char old_chksum = 0;
	char new_chksum = 0;
	char tmp_chksum = 0;
	char chksum = 0;
	do
	{
		/* Read Block Data Checksum */
		if (!BQ27441_readChecksum(&old_chksum, 1000))
			return 0;

		__delay_cycles(1000000);

		/* Checksum calculation */
		tmp_chksum = old_chksum;

		short old_opconfig= 0;

		/* Read old opconfig */
		if (!BQ27441_read16(0x40, &old_opconfig, 1000))
			return 0;

		tmp_chksum = computeCheckSum(tmp_chksum, old_opconfig, 0x05F8);

		/* Write new opconfig */
		if (!BQ27441_write16(0x40, swapMSB_LSB(0x05F8), 1000))
			return 0;

		/* Checksum calculation */
		new_chksum = tmp_chksum;

		/* Write new checksum */
		if (!BQ27441_command(BLOCK_DATA_CHECKSUM, new_chksum, 1000))
			return 0;

		__delay_cycles(1000000);

		/* Read Block Data Checksum */
		if (!BQ27441_readChecksum(&chksum, 1000))
			return 0;

		__delay_cycles(1000000);
	}
	while(new_chksum != chksum);

	/* Send SOFT_RESET control command */
	if (!BQ27441_control(SOFT_RESET, 1000))
		return 0;

	__delay_cycles(1000000);

	result = 0;
	/* Check if CFGUPMODE bit is cleared in FLAGS */
	while(result & 0x0010)
	{
		if (!BQ27441_read16(FLAGS, &result, 1000))
			return 0;
	}

	// Read the Operation Config
	short result16 = 0;
    if (!BQ27441_read16(OP_CONFIG, &result16, 1000))
        return 0;

    // Check if BIE is cleared in Operation Config
    if (result16 & 0x2000)
    {
    	return 0;
    }

	return 1;
}


/* Send control subcommand */
bool BQ27441_control(short subcommand, unsigned int timeout)
{
	I2C_init();

	/* Specify slave address for BQ27441 */
	I2C_setslave(BQ27441_SLAVE_ADDRESS);

	if (!I2C_write16(CONTROL, subcommand, timeout))
		return 0;

	return 1;
}


/* Send control subcommand then read from control command */
bool BQ27441_controlRead(short subcommand, short *result, unsigned int timeout)
{
	if (!BQ27441_control(subcommand, timeout))
		return 0;

	__delay_cycles(1000000);

	if (!BQ27441_read16(CONTROL, result, timeout))
		return 0;

	return 1;
}


/* Send command */
bool BQ27441_command(short command, char data, unsigned int timeout)
{
	/* Specify slave address for BQ27441 */
	I2C_setslave(BQ27441_SLAVE_ADDRESS);

	if (!I2C_write8(command, data, timeout))
		return 0;

	return 1;
}

/* Write word to address */
bool BQ27441_write16(short addr, short data, unsigned int timeout)
{
	/* Specify slave address for BQ27441 */
	I2C_setslave(BQ27441_SLAVE_ADDRESS);

	if (!I2C_write16(addr, data, timeout))
		return 0;

	return 1;
}


/* Read from standard command*/
bool BQ27441_read16(short stdcommand, short *result, unsigned int timeout)
{
	I2C_init();

	/* Specify slave address for BQ27441 */
	I2C_setslave(BQ27441_SLAVE_ADDRESS);

	if (!I2C_read16(stdcommand, result, timeout))
		return 0;

	return 1;
}


/* Read block data checksum */
bool BQ27441_readChecksum(char *result, unsigned int timeout)
{
	I2C_init();

	/* Specify slave address for BQ27441 */
	I2C_setslave(BQ27441_SLAVE_ADDRESS);

	if (!I2C_read8(BLOCK_DATA_CHECKSUM, result, timeout))
		return 0;

	return 1;
}


/* Computes checksum for fuel gauge */
unsigned char computeCheckSum(unsigned char oldCheckSum, int oldData, int newData)
{
	unsigned char tmpCheckSum = 0xFF - oldCheckSum - ( unsigned char )oldData - ( unsigned char )( oldData >> 8 );
	unsigned char newCheckSum = 0xFF - (  tmpCheckSum + ( unsigned char )newData + ( unsigned char )( newData >> 8 ) );
	return newCheckSum;
}


/* Swaps the MSB and LSB of a word */
int swapMSB_LSB(int data)
{
	int tmp = ( unsigned char )data;
	tmp = tmp << 8;
	tmp += ( unsigned char )( data >> 8 );
	return tmp;
}


//*****************************************************************************
//
//! Provides a small delay.
//!
//! \param ui32Count is the number of delay loop iterations to perform.
//!
//! This function provides a means of generating a delay by executing a simple
//! 3 instruction cycle loop a given number of times.  It is written in
//! assembly to keep the loop instruction count consistent across tool chains.
//!
//! It is important to note that this function does NOT provide an accurate
//! timing mechanism.  Although the delay loop is 3 instruction cycles long,
//! the execution time of the loop will vary dramatically depending upon the
//! application's interrupt environment (the loop will be interrupted unless
//! run with interrupts disabled and this is generally an unwise thing to do)
//! and also the current system clock rate and flash timings (wait states and
//! the operation of the prefetch buffer affect the timing).
//!
//! For best accuracy, a system timer should be used with code either polling
//! for a particular timer value being exceeded or processing the timer
//! interrupt to determine when a particular time period has elapsed.
//!
//! \return None.
//
//*****************************************************************************
#if defined(__ICCARM__) || defined(DOXYGEN)
void
SysCtlDelay(uint32_t ui32Count)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}
#endif
#if defined(codered) || defined(__GNUC__) || defined(sourcerygxx)
void __attribute__((naked))
SysCtlDelay(uint32_t ui32Count)
{
    __asm("    subs    r0, #1\n"
          "    bne     SysCtlDelay\n"
          "    bx      lr");
}
#endif
#if defined(rvmdk) || defined(__CC_ARM)
__asm void
SysCtlDelay(uint32_t ui32Count)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}
#endif
