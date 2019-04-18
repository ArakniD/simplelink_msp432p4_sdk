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

#include "npi_message.h"

/*******************************************************************************
 * @fn      NPI_setFCS(npiMessage_t *rx)
 *
 * @brief   Function that calculates and sets the FCS byte of the message
 *
 * @param   *msg     pointer to an npiMessage_t that needs its FCS byte set
 *
 * @return  None.
 ******************************************************************************/
void NPI_setFCS(npiMessage_t* msg)
{
    uint8_t fcs;
    uint16_t total;

    /*
     * Calculate the total length of the payload
     */
    total = msg->len1 << 8;
    total |= msg->len0;

    /*
     * Calculate the require FCS byte
     */
    fcs = msg->len0 ^ msg->len1;
    fcs ^= msg->type;
    fcs ^= msg->command;
    uint16_t k;
    for (k = 0; k < total; k++)
    {
        fcs ^= msg->payload[k];
    }

    /*
     * Set the FCS byte of the message
     */
    msg->fcs = fcs;
}

/********************************************************************************
 * @fn      NPI_validMsg(npiMessage_t *rx)
 *
 * @brief   Function that calculates the message's FCS and ensures the
 *          calculated FCS matches the transmitted FCS
 *
 * @param   *msg     pointer to an npiMessage_t that needs its FCS byte validated
 *
 * @return  None.
 *******************************************************************************/
uint8_t NPI_validMsg(npiMessage_t* rx)
{

    uint8_t fcs;
    uint16_t total;
    uint16_t k;

    /*
     * Calculating total length of the payload
     */
    total = rx->len1 << 8;
    total |= rx->len0;

    /*
     * calculating the FCS of the entire message
     */
    fcs = rx->len0 ^ rx->len1;
    fcs ^= rx->type;
    fcs ^= rx->command;
    for (k = 0; k < total; k++)
    {
        fcs ^= rx->payload[k];
    }

    /*
     * Determining if the fcs matches the fcs set in the message
     */
    if (fcs == rx->fcs)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
 * @fn      NPI_buildBuffer(uint8_t *buf, npiMessage_t *msg)
 *
 * @brief   Function that converts the message into a buffer for transmission.
 *
 * @param   *msg    pointer to an npiMessage_t that needs converted to a buffer
 *
 * @param   *buf    pointer to a buffer that will store the converted message
 *
 * @return  None.
 ******************************************************************************/
void NPI_buildBuffer(uint8_t *buf, npiMessage_t *msg)
{
    uint8_t k;
    uint16_t total;

    /*
     * In case it was forgotten, calculate and set the FCS byte of the message
     */
    NPI_setFCS(msg);

    /*
     * Calculating the total length of the payload
     */
    total = msg->len1 << 8;
    total |= msg->len0;

    /*
     * Place npiMessage elements into a buffer
     */
    *(buf) = msg->len0;
    *(buf + 1) = msg->len1;
    *(buf + 2) = msg->type;
    *(buf + 3) = msg->command;
    for (k = 4; k < (4 + total); k++)
    {
        *(buf + k) = msg->payload[k - 4];
    }
    *(buf + total + 4) = msg->fcs;
}

/************************************************************************************
 * @fn      NPI_parseMsg(uint8_t *buf, npiMessage_t *rx)
 *
 * @brief   Extracts the message from the buffer
 *
 * @param   *buf    pointer to an array that contains a communication transaction
 *
 * @param   *rx     pointer to an npiMessage that will have its structure set
 *                  depending on the data contained in the buffer
 *
 * @return  None.
 ***********************************************************************************/
void NPI_parseMsg(uint8_t *buf, npiMessage_t *rx)
{
    uint16_t total;
    uint8_t k;

    /*
     * The bytes in the buffer correspond to
     */
    rx->len0 = *(buf);
    rx->len1 = *(buf + 1);
    rx->type = (npi_type_t) *(buf + 2);
    rx->command = (npi_cmd_t) *(buf + 3);

    /*
     * Calculating the total length of the payload
     */
    total = (rx->len1 << 8);
    total |= rx->len0;

    /*
     * Inserting the buffer's payload into the npiMessage payload
     */
    for (k = 4; k < (4 + total); k++)
    {
        rx->payload[k - 4] = *(buf + k);
    }

    /*
     * Copying the fcs byte into the npiMessage fcs
     */
    rx->fcs = *(buf + 4 + total);
}

/************************************************************************************
 * @fn      NPI_parsePayload(npiMessage_t *msg, uint8_t *storePayload)
 *
 * @brief   Parses the payload from the message
 *
 * @param   *msg     pointer to an npiMessage_t that contains the desired payload
 *
 * @param   *storePayload   pointer to an array that will store the desired payload
 *
 * @return  None.
 ***********************************************************************************/
void NPI_parsePayload(npiMessage_t *msg, uint8_t *storePayload)
{
    uint8_t i;
    uint16_t total;

    /*
     * Calculating the total length of the payload
     */
    total = msg->len1 << 8;
    total |= msg->len0;

    /*
     * Copying the npiMessage payload into the storePayload array
     */
    for (i = 0; i < total; i++)
    {
        *(storePayload + i) = msg->payload[i];
    }
}
