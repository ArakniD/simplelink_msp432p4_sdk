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

/* Driver Header files */
#include <ti/drivers/I2C.h>

/* Local Headers */
#include "npi_master.h"
#include "npi_message.h"
#include "npi_task.h"

/*******************************************************************************
 *                                CONSTANTS
 ******************************************************************************/
#define CAPTIVATE_I2C_SLAVE_ID    (0x0B)

/*******************************************************************************
 *                             LOCAL VARIABLES
 ******************************************************************************/
npi_capt_t captDevice;

/*******************************************************************************
 * @fn      NPIMaster_sendCaptInfoReq
 *
 * @brief   Function that sends an information request message to the slave
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void NPIMaster_sendCaptInfoReq(void)
{
    npiMessage_t send;

    /*
     * Set the elements for an INFO message, no payload required
     */
    send.len0 = 0;
    send.len1 = 0;
    send.type = CAPT_TYPE;
    send.command = INFO_CMD;

    /*
     * Write the message to the slave
     */
    NPIMaster_writeMsg(&send);
}

/*******************************************************************************
 * @fn      NPIMaster_setCaptInfo
 *
 * @brief   Function that sends a message to the slave with an information
 *          request
 *
 * @param   *rx     a pointer to an npiMessage_t that contains the received
 *                  received message with capTIvate info data
 *
 * @return  None.
 ******************************************************************************/
void NPIMaster_setCaptInfo(npiMessage_t *rx)
{

    /*
     * After determining the received message pointer has already been
     * validated as an INFO message, set the capTIvate device structure with
     * the necessary data.
     */
    captDevice.numKeypad = rx->payload[0];
    captDevice.numSliders = rx->payload[1];
    captDevice.numProximity = rx->payload[2];
    captDevice.numButtons = rx->payload[3];
}

/*******************************************************************************
 * @fn      NPIMaster_readMsg(npiMessage_t *rx)
 *
 * @brief   Function that reads a message from the slave device and fills the
 *          parameter with the message data
 *
 * @param   *rx     pointer to an npiMessage_t where the receiving message
 *                  will be stored
 *
 * @return  None.
 ******************************************************************************/
void NPIMaster_readMsg(npiMessage_t *rx)
{
    uint8_t msgBuffer[FIXED_LENGTH];
    I2C_Transaction masterTransaction;

    /* Select the slave's address and perform a read operation */
    masterTransaction.writeCount = 0;
    masterTransaction.writeBuf = NULL;
    masterTransaction.readCount = FIXED_LENGTH;
    masterTransaction.readBuf = &msgBuffer[0];
    masterTransaction.slaveAddress = CAPTIVATE_I2C_SLAVE_ID;
    I2C_transfer(i2cCaptivateHandle, &masterTransaction);

    /* Parse the buffer and save the data to the message pointer */
    NPI_parseMsg(&msgBuffer[0], rx);
}

/*******************************************************************************
 * @fn      NPIMaster_writeMsg(npiMessage_t *rx)
 *
 * @brief   Function that sends a message to the slave device with the data
 *          contained in the parameter pointer
 *
 * @param   *tx     pointer to an npiMessage_t that will be sent to the slave
 *
 * @return  None.
 ******************************************************************************/
void NPIMaster_writeMsg(npiMessage_t* tx)
{
    uint8_t msgBuffer[FIXED_LENGTH];
    I2C_Transaction masterTransaction;

    /* Determine the total length of the message */
    uint16_t len;
    len = tx->len1 << 8;
    len |= tx->len0;
    len += 5;

    /* If the message is longer than the FIXED_LENGTH, the entire message will
     * not be sent to the other device. Thus, don't send at all */
    if (len > FIXED_LENGTH)
    {
        return;
    }

    /* Take the message and move it into an array */
    NPI_buildBuffer(&msgBuffer[0], tx);

    /* Select the slave's I2C address, then send the buffer */
    masterTransaction.writeCount = FIXED_LENGTH;
    masterTransaction.writeBuf = &msgBuffer[0];
    masterTransaction.readCount = 0;
    masterTransaction.readBuf = NULL;
    masterTransaction.slaveAddress = CAPTIVATE_I2C_SLAVE_ID;
    I2C_transfer(i2cCaptivateHandle, &masterTransaction);
}
