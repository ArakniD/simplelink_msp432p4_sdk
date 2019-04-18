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
// HAL_BQ27441.h - Prototypes of hardware abstraction layer for interfacing BQ27441
//
//****************************************************************************

#include <stdbool.h>

#ifndef __HAL_BQ27441_H_
#define __HAL_BQ27441_H_


/*CONSTANTS*/
#define BQ27441_SLAVE_ADDRESS              0x55
#define BAT_CAPACITY                       1200


/* Standard Commands */
#define CONTROL                            0x00
#define TEMPERATURE                        0x02
#define VOLTAGE                            0x04
#define FLAGS                              0x06
#define NOMINAL_AVAILABLE_CAPACITY         0x08
#define FULL_AVAILABLE_CAPACITY            0x0A
#define REMAINING_CAPACITY                 0x0C
#define FULL_CHARGE_CAPACITY               0x0E
#define AVERAGE_CURRENT                    0x10
#define STANDBY_CURRENT                    0x12
#define MAX_LOAD_CURRENT                   0x14
#define AVERAGE_POWER                      0x18
#define STATE_OF_CHARGE                    0x1C
#define INTERNAL_TEMPERATURE               0x1E
#define STATE_OF_HEALTH                    0x20
#define REMAINING_CAPACITY_UNFILTERED      0x28
#define REMAINING_CAPACITY_FILTERED        0x2A
#define FULL_CHARGE_CAPACITY_UNFILTERED    0x2C
#define FULL_CHARGE_CAPACITY_FILTERED      0x2E
#define STATE_OF_CHARGE_UNFILTERED         0x30

/* Extended Data Commands */
#define OP_CONFIG                          0x3A
#define DESIGN_CAPACITY                    0x3C
#define DATA_CLASS                         0x3E
#define DATA_BLOCK                         0x3F
#define BLOCK_DATA                         0x40
#define BLOCK_DATA_CHECKSUM                0x60
#define BLOCK_DATA_CONTROL                 0x61


/* Control Subcommands */
#define CONTROL_STATUS                     0x0000
#define DEVICE_TYPE                        0x0001
#define FW_VERSION                         0x0002
#define DM_CODE                            0x0004
#define PREV_MACWRITE                      0x0007
#define CHEM_ID                            0x0008
#define BAT_INSERT                         0x000C
#define BAT_REMOVE                         0x000D
#define SET_HIBERNATE                      0x0011
#define CLEAR_HIBERNATE                    0x0012
#define SET_CFGUPDATE                      0x0013
#define SHUTDOWN_ENABLE                    0x001B
#define SHUTDOWN                           0x001C
#define SEALED                             0x0020
#define TOGGLE_GPOUT                       0x0023
#define RESET                              0x0041
#define SOFT_RESET                         0x0042
#define EXIT_CFGUPDATE                     0x0043
#define EXIT_RESIM                         0x0044


/* BQ27441 Configuration */
#define CONF_DESIGN_CAPACITY               1200    // Design Capacity = 1200mAh
#define CONF_DESIGN_ENERGY                 (int)(CONF_DESIGN_CAPACITY * 3.7)  //DesignEnergy = DesignCapacity(mAh) * 3.7V
#define CONF_TERMINATE_VOLTAGE             3200    //TerminateVoltage = 3200mV
#define CONF_TAPER_CURRENT                 115     //TaperCurrent = 115mA
#define CONF_TAPER_RATE                    (int)(CONF_DESIGN_CAPACITY / (0.1 * CONF_TAPER_CURRENT))


/* Function Declarations */
bool BQ27441_initConfig(void);
bool BQ27441_initOpConfig(void);
bool BQ27441_control(short subcommand, unsigned int timeout);
bool BQ27441_controlRead(short subcommand, short *result, unsigned int timeout);
bool BQ27441_command(short command, char data, unsigned int timeout);
bool BQ27441_write16(short addr, short data, unsigned int timeout);
bool BQ27441_read16(short stdcommand, short *result, unsigned int timeout);
bool BQ27441_readChecksum(char *result, unsigned int timeout);
unsigned char computeCheckSum(unsigned char oldCheckSum, int oldData, int newData);
int swapMSB_LSB(int data);

/* Custom __delay_cycles() for non CCS Compiler */
#if !defined(__TI_ARM__)
#undef __delay_cycles
#define __delay_cycles(x)     SysCtlDelay(x)
void SysCtlDelay(uint32_t);
#endif


#endif /* __HAL_BQ27441_H_ */
