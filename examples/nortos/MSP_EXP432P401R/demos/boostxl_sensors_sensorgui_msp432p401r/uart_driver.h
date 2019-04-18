//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// uart_driver.c

//****************************************************************************

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

// Choose baudrate
#define BAUDRATE                (115200)

// Global variables
#define MAX_STR_LENGTH 271

#define FALSE 0
#define TRUE  1

typedef struct{
	unsigned char newStringReceived;
	char          txString[MAX_STR_LENGTH];
	char          rxString[MAX_STR_LENGTH];
}s_test;

extern s_test test;
//extern char rxString[MAX_STR_LENGTH];     // The entire input string from the last 'return'
//extern char txString[MAX_STR_LENGTH];
//extern char partOfString[MAX_STR_LENGTH];
//extern unsigned char newStringReceived;

// Use handshake to secure UART communication
//#define USE_HANDSHAKE
#ifdef USE_HANDSHAKE
    #define RTS_INIT	            {P2DIR |= BIT3;}
    #define RTS_SET   	            {P2OUT |= BIT3;}
    #define RTS_CLEAR               {P2OUT &= ~BIT3;}
    #define CTS_INIT                {P2DIR &= ~BIT4;}
    #define CTS_IS_SET              ((P2IN & BIT4)==BIT4)
#else
    #define RTS_INIT	            { }
    #define RTS_SET   	            { }
    #define RTS_CLEAR               { }
    #define CTS_INIT                { }
    #define CTS_IS_SET              (1)
#endif

void uartReceive(char data);
void uartInit(void);
void uartSend(char * buf, unsigned char len);
void sendText(void);
bool receiveText(char* data, int maxNumChars);

#endif /* UART_DRIVER_H_ */
