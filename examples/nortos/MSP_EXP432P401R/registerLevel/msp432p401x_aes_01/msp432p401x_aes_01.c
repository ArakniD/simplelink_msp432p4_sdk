/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *
 *******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see http://www.ti.com/tool/mspdriverlib for an API functional
 * library & https://dev.ti.com/pinmux/ for a GUI approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP432P401 Demo - AES256 Encryption & Decryption
//
//  Description: This example shows a simple example of encryption and
//  decryption using the AES256 module. Example blinks P1.0 LED if successful.
//
//                MSP432P401x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//  Key: 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
//  Plaintext: 00112233445566778899aabbccddeeff
//  Ciphertext: 8ea2b7ca516745bfeafc49904b496089
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

uint8_t Data[16] = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff };

uint8_t CipherKey[32] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f };

uint8_t DataAESencrypted[16];       // Encrypted data
uint8_t DataAESdecrypted[16];       // Decrypted data


int main(void) {
    uint32_t i;
    uint16_t sCipherKey, tempVariable;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    P1->DIR |= BIT0;                        // P1.0 set as output
    P1->OUT &= ~BIT0;                       // Turn off P1.0 LED

    /* Step 1: Load cipher key */
    AES256->CTL0 &= ~AES256_CTL0_OP_3;      // Set AES module to encrypt mode

    /* Set AES key length to 256 bits */
    AES256->CTL0 = (AES256->CTL0 & ~(AES256_CTL0_KL_1 + AES256_CTL0_KL_2))  | AES256_CTL0_KL__256BIT;

    /* Load 256-bit cipher key to the AES256->KEY register */
    for(i = 0; i < 256/8; i = i + 2)
    {
        /* Concatenate 2 8-bit blocks into one 16-bit block */
        sCipherKey =(uint16_t)(CipherKey[i]);
        sCipherKey = sCipherKey |((uint16_t)(CipherKey[i + 1]) << 8);
        /* Load 16-bit key block to AES256->KEY register */
        AES256->KEY = sCipherKey;
    }

    /* Wait until key is written */
    while((AES256->STAT & AES256_STAT_KEYWR ) == 0);


    /* Step 2: Encrypt data and store to DataAESencrypted */


    /* Load 128-bit block of data to encrypt to module */
    for(i = 0; i < 16; i = i + 2)
    {
        /* Concatenate 2 8-bit blocks into one 16-bit block */
        tempVariable =(uint16_t)(Data[i]);
        tempVariable = tempVariable |((uint16_t)(Data[i + 1]) << 8);
        /* Load 16-bit key block to AESADIn register
         * Last write to DIN automatically starts the encryption */
        AES256->DIN = tempVariable;
    }

    /* Wait unit finished ~167 MCLK */
    while( AES256->STAT & AES256_STAT_BUSY );

    /* Write 128-bit block of encrypted data back to DataAESencrypted */
    for(i = 0; i < 16; i = i + 2)
    {
        tempVariable = AES256->DOUT;

        DataAESencrypted[i] = (uint8_t)tempVariable;
        DataAESencrypted[i+1] = (uint8_t)(tempVariable >> 8);
    }


    /* Step 3: Reload AES key */

    /* Set AES module to decrypt mode */
    AES256->CTL0 |= AES256_CTL0_OP_1;


    /* Set AES key length to 256 bits */
    AES256->CTL0 = (AES256->CTL0 & ~(AES256_CTL0_KL_1 + AES256_CTL0_KL_2))  | AES256_CTL0_KL__256BIT;

    /* Load 256-bit cipher key to the AES256->KEY register */
    for(i = 0; i < 256/8; i = i + 2)
    {
        /* Concatenate 2 8-bit blocks into one 16-bit block */
        sCipherKey =(uint16_t)(CipherKey[i]);
        sCipherKey = sCipherKey |((uint16_t)(CipherKey[i + 1]) << 8);
        /* Load 16-bit key block to AES256->KEY register */
        AES256->KEY = sCipherKey;
    }

    /* Wait until key is written */
    while((AES256->STAT & AES256_STAT_KEYWR ) == 0);


    /*Step 4: Decrypt data with keys that were generated during encryption
         * takes 214 MCLK.
         * This function will generate all round keys needed for decryption first
         * and then the encryption process starts */

    // Write 128-bit block of data to decrypt to module
    for (i = 0; i < 16; i = i + 2)
    {
        tempVariable = (uint16_t) (DataAESencrypted[i + 1] << 8);
        tempVariable = tempVariable | ((uint16_t) (DataAESencrypted[i]));
        AES256->DIN = tempVariable;
    }


    /* Wait until finished ~167 MCLK */
    while(AES256->STAT & AES256_STAT_BUSY);

    /* Write 128-bit block of encrypted data back to DataAESdecrypted */
    for(i = 0; i < 16; i = i + 2)
    {
        tempVariable = AES256->DOUT;
        DataAESdecrypted[i] = (uint8_t)tempVariable;
        DataAESdecrypted[i+1] =(uint8_t)(tempVariable >> 8);
    }

    /* Step 4: Confirm decrypted data is identical to original data */
    for(i = 0; i < 16; i ++)
       if (DataAESdecrypted[i] != Data[i])
           while(1);                        // Set breakpoint here for error

    while(1)
    {
        P1->OUT ^= BIT0;                    // Blink P1.0 LED == success
        for (i = 20000; i > 0; i--);        // Delay
    }
}

