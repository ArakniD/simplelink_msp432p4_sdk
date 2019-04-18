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
//
// 		Description: Source code for the implementation of the Bosch BoosterPack with
// 		an MSP432P401R launchpad for low power consumption. BoosterPack includes:
// 		- Inertial Measurement Unit (IMU) sensor with an accelerometer and gyroscope
// 		- Magnetometer
// 		- Environmental sensor with pressure, ambient temperature and humidity
// 		- Ambient light sensor
// 		- Infrared temperature sensor
//
// Adapted by Michael Arriete
//****************************************************************************

#include <ti/devices/msp432p4xx/inc/msp432.h>
#include "stdio.h"
#include "string.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "uart_driver.h"

#define SMCLK_FREQUENCY_HZ			(12000000)

s_test test = {
	FALSE,
	"",
	""
};

/*
* \brief uartReceive
*  adds bytes to the rx buffer
*
* @return void
*/
void uartReceive(char data){
	static char rxInProgress = FALSE;
	static char pieceOfString[MAX_STR_LENGTH] = "";           // Holds the new addition to the string
	static int charCnt = 0;

	if( !rxInProgress){
		if ((data != '\n') ){
			pieceOfString[0] = '\0';
			rxInProgress = TRUE;
			pieceOfString[0] = data;
			charCnt = 1;
		}
	}else{ // in progress
		if((data == '\n')){
			rxInProgress = FALSE;
			if (test.newStringReceived == FALSE){ // don't mess with the string while main processes it.
				pieceOfString[charCnt]='\0';
				__no_operation();
				charCnt++;
				strncpy(test.rxString,pieceOfString,charCnt);
				__no_operation();
				test.newStringReceived = TRUE;
				__no_operation();
			}
		}else{
			if (charCnt >= MAX_STR_LENGTH){
				rxInProgress = FALSE;
			}else{
				pieceOfString[charCnt++] = data;
				//charCnt++;
			}
		}
	}
}


/*
* \brief uartInit
*
* @return void
*/
void uartInit()
{
    P1SEL0 |= BIT2 | BIT3;
    UCA0CTLW0 |= 0x0001;
    UCA0CTLW0 &= ~(UCSSEL_3);
    UCA0CTLW0 |= UCSSEL__SMCLK;
    //UCA0BRW = 0x68;

    // Calculate settings for UART registers
	double Num = SMCLK_FREQUENCY_HZ/BAUDRATE;

	// Different register settings due to UART calculation
	// Use UCBRFO and UC0S16 when (SMCLK / baudrate) >= 16
	if(Num >=16)
	{
		UCA0BRW = (unsigned int)(Num/ 16);
		#if (SMCLK_FREQUENCY_HZ == 1000000)
				UCA0MCTLW = 0xFF81;
		#else
				UCA0MCTLW |= UCOS16 |
							(UCBRF_M *
							((unsigned int)(0.5+(((Num/16)-
							((unsigned int)(Num/16)))*16))));
//				if((SMCLK_FREQUENCY_HZ == 12000000) && (BAUDRATE == 115200)) {
//					UCA0MCTLW &= 0x00FF;
//					UCA0MCTLW |= 0x1100;
//				}
		#endif
			}
	// Use only UCBRS0 when (SMCLK / baudrate) < 16
	else
	{
		UCA0BRW = (unsigned int)(Num);
		UCA0MCTLW |= (UCBRS_M *
					 ((unsigned int)(0.5+(((Num)-
					 ((unsigned int)(Num)))*8))));
	}

    UCA0CTLW0 &= ~(UCSWRST);

    UCA0IE |= UCRXIE;

    NVIC->ISER[0] |= 1 << ((EUSCIA0_IRQn) & 31);

	test.newStringReceived = FALSE;
}

/* EUSCI A0 UART ISR - Echos data back to PC host */
void EUSCIA0_IRQHandler(void)
{
//	int receiveByte = UCA0RXBUF;
//	UCA0TXBUF = receiveByte;

    RTS_CLEAR;
    char data = UCA0RXBUF;  //RXBUF0;

    //#define ECHO_ON
#ifdef ECHO_ON
    while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
    TXBUF0 = data;                          // RXBUF0 to TXBUF0
#endif

    uartReceive(data);
    if(test.newStringReceived == TRUE){
//        _bic_SR_register_on_exit(LPM4_bits); /* Exit Low Power Mode 4 */
    }
    RTS_SET;
}


/**
* \brief uartSend
*
* Send out message string
*
* @param char * buf			String of characters to send
* @param unsigned char len	Number of characters to send
*
* @return void
*/
void sendText(){
   unsigned int i;

	for (i = 0; i < MAX_STR_LENGTH; ++i)
    {
	   // wait until UART ready
	   while (!(UCA0IFG & UCTXIFG)) {
	       ;// USCI_A0 TX buffer ready?
	   }
		   if (test.txString[i] != 0)
		  {
			  EUSCI_A_UART_transmitData(EUSCI_A0_BASE, test.txString[i]);
		  }
		  else{
			  break;
		  }
    }
}

bool receiveText(char* data, int maxNumChars){
	bool result = false;
    if(test.newStringReceived == TRUE){
    	result = true;
    	strncpy(data,test.rxString,maxNumChars);
    	test.newStringReceived = FALSE;
    }
    return result;
}
