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
 * ITM.h
 *
 *  Created on: Jul 13, 2015
 *      Author: a0225807
 */

#ifndef ITM_H_
#define ITM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile uint32_t* ITM_port_t;

void delay(uint32_t num_loops);
void port_wait(ITM_port_t port);
void ITM_put_string(ITM_port_t port, const int8_t* data);
void ITM_put_32(ITM_port_t port, uint32_t data);
void ITM_put_16(ITM_port_t port, uint16_t data);
void ITM_put_08(ITM_port_t port, uint8_t data);

#define ONE_PIN_TPIU       1
#define TWO_PIN_TPIU       2
#define FOUR_PIN_TPIU      8

ITM_port_t port;
//uint32_t port_address;
//uint32_t port_num = 1;

/////////////////////////////////////
// select port configuration
/////////////////////////////////////
#define NUM_OF_PINS 	   ONE_PIN_TPIU
#define ATB_ID_VALUE       0xF     // 7-bit value

#ifdef __cplusplus
}
#endif

#endif /* ITM_H_ */
