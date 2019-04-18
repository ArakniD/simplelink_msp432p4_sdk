/*
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
 */

#ifndef NPIMESSAGE_H_
#define NPIMESSAGE_H_

#include <stdint.h>

#define FIXED_LENGTH    20

typedef enum
{
    CAPT_TYPE = 0xCA
} npi_type_t;

typedef enum
{
    INFO_CMD = 1,
    CAPT_MSG = 0xAA
} npi_cmd_t;


// Every message should use this structure
typedef struct
{
    uint8_t len0;
    uint8_t len1;
    npi_type_t type;
    npi_cmd_t command;
    uint8_t payload[FIXED_LENGTH - 5];
    uint8_t fcs;
} npiMessage_t;

void NPI_setFCS(npiMessage_t* tx);

uint8_t NPI_validMsg(npiMessage_t* rx);

void NPI_buildBuffer(uint8_t* buf, npiMessage_t* msg);

void NPI_parseMsg(uint8_t* buf, npiMessage_t* msg);

void NPI_parsePayload(npiMessage_t *msg, uint8_t *storePayload);

#endif /* NPIMESSAGE_H_ */
