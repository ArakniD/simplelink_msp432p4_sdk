/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
/*
 * ITM.c
 *
 *  Created on: Jul 13, 2015
 *      Author: a0225807
 */

#ifndef DeviceFamily_MSP432P4x1xI
#define DeviceFamily_MSP432P4x1xI
#endif

#include <ti/devices/DeviceFamily.h>
#include <stdio.h>
#include <stdint.h>
#include <ITM.h>

const uint32_t ITM_BASE_ADDRESS = 0xE0000000;
const uint32_t ITM_NUM_PORTS = 32;
const uint32_t NUM_TRIALS = 2;

void delay(uint32_t num_loops)
{
  uint32_t i;
  for (i=0; i<num_loops; i++)
  {
	  asm (" NOP");
  }
}

void port_wait(ITM_port_t port)
{
	delay(10);
	/* Wait while fifo ready */
	while (*port == 0);
}


/* Send a nul terminated string to the port */
void ITM_put_string(ITM_port_t port, const int8_t* data)
{
	uint32_t datapos  = 0;
	uint32_t portpos  = 0;
	uint32_t portdata = 0;

	while('\0' != data[datapos])
	{
		port_wait(port);
		portdata = 0;

		/* Get the next 4 bytes of data */
		for (portpos=0; portpos<4; ++portpos) {
			portdata |= data[datapos] << (8*portpos);
			if ('\0' != data[datapos]) {
				++datapos;
			}
		}

		/* Write the next 4 bytes of data */
		*port = portdata;
	}
}

/* Send a 32 bit value to the port */
void ITM_put_32(ITM_port_t port, uint32_t data)
{
    if (port <= (ITM_port_t)32) {
        port = (ITM_port_t)(((uint32_t)port << 2) | (uint32_t)ITM_BASE_ADDRESS) ;
    }
	port_wait(port);
	*port = data;
}


/* Send a 16 bit value to the port */
void ITM_put_16(ITM_port_t port, uint16_t data)
{
	/* Cast port for 16-bit data */
	volatile uint16_t* myport = (volatile uint16_t*)port;
	port_wait(port);
	*myport = data;
}


/* Send a 8 bit value to the port */
void ITM_put_08(ITM_port_t port, uint8_t data)
{
	/* Cast port for 8-bit data */
	volatile uint8_t* myport = (volatile uint8_t*)port;
	port_wait(port);
	*myport = data;
}

#define ONE_PIN_TPIU       1
#define TWO_PIN_TPIU       2
#define FOUR_PIN_TPIU      8

#define NUM_OF_PINS 	   ONE_PIN_TPIU
#define ATB_ID_VALUE       0xF     // 7-bit value
