/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2015 - 2017  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the RTT protocol and J-Link.                       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* conditions are met:                                                *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this list of conditions and the following disclaimer.    *
*                                                                    *
* o Redistributions in binary form must reproduce the above          *
*   copyright notice, this list of conditions and the following      *
*   disclaimer in the documentation and/or other materials provided  *
*   with the distribution.                                           *
*                                                                    *
* o Neither the name of SEGGER Microcontroller GmbH & Co. KG         *
*   nor the names of its contributors may be used to endorse or      *
*   promote products derived from this software without specific     *
*   prior written permission.                                        *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: V2.52d                                    *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_Config_FreeRTOS.c
Purpose : Sample setup configuration of SystemView with FreeRTOS.
Revision: $Rev: 7745 $
*/
#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"

extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "MAT"

// The target device name
#define SYSVIEW_DEVICE_NAME     "MSP432P4011"

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_GET_TIMESTAMP in SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ  (configCPU_CLOCK_HZ)

// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ        configCPU_CLOCK_HZ

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x00000000)

/********************************************************************* 
*
*       _cbSendSystemDesc()
*
*  Function description
*    Sends SystemView description strings.
*/
static void _cbSendSystemDesc(void) {
  SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",D="SYSVIEW_DEVICE_NAME",O=FreeRTOS,C=Cortex-M4");
  SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");

  /* Establish all interrupt numbers to name here */
  SEGGER_SYSVIEW_SendSysDesc("I#46=DMA_ERR");
  SEGGER_SYSVIEW_SendSysDesc("I#47=DMA_INT3_ads1222");
  SEGGER_SYSVIEW_SendSysDesc("I#48=DMA_INT2_adc14");
  SEGGER_SYSVIEW_SendSysDesc("I#49=DMA_INT1_oled");
  SEGGER_SYSVIEW_SendSysDesc("I#50=DMA_INT0_cc2564_trx");
  SEGGER_SYSVIEW_SendSysDesc("I#41=T32_INT1_ads1222_smpl");
  SEGGER_SYSVIEW_SendSysDesc("I#42=T32_INT2_cc2564_to");
  //SEGGER_SYSVIEW_SendSysDesc("I#51=PORT1");
  //SEGGER_SYSVIEW_SendSysDesc("I#52=PORT2");
  //SEGGER_SYSVIEW_SendSysDesc("I#53=PORT3");
  //SEGGER_SYSVIEW_SendSysDesc("I#54=PORT4");
  SEGGER_SYSVIEW_SendSysDesc("I#55=PORT5_qi_cts_sw");
  SEGGER_SYSVIEW_SendSysDesc("I#40=ADC14");
  SEGGER_SYSVIEW_SendSysDesc("I#36=EUSCIB0_ADS1222");
  SEGGER_SYSVIEW_SendSysDesc("I#38=EUSCIB2_i2cHAT");
  SEGGER_SYSVIEW_SendSysDesc("I#39=EUSCIB3_pmBus");
  SEGGER_SYSVIEW_SendSysDesc("I#32=EUSCIA0_debug");
  SEGGER_SYSVIEW_SendSysDesc("I#33=EUSCIA1_none");
  SEGGER_SYSVIEW_SendSysDesc("I#34=EUSCIA2_cc2564");
  SEGGER_SYSVIEW_SendSysDesc("I#24=TA0_0_pushBtn");
  SEGGER_SYSVIEW_SendSysDesc("I#14=PendSV");
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
void SEGGER_SYSVIEW_Conf(void) {
  SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ, 
                      &SYSVIEW_X_OS_TraceAPI, _cbSendSystemDesc);
  SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

/*************************** End of file ****************************/
