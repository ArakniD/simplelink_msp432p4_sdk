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

/*
 *  ======== sensor_thread.h ========
 */

#ifndef SENSOR_THREAD_H_
#define SENSOR_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Standard Includes */
#include <stdint.h>
#include <math.h>

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* POSIX Header files */
#include <semaphore.h>

/* Driver Header files */
#include <ti/display/Display.h>

/* Sensor BoosterPack I2C Slave Addresses */
#define TMP007_SLAVE_ADDR    0x40    /*! ADR1 = 0, ADR0 = 0 */
#define OPT3001_SLAVE_ADDR   0x47    /*! ADDR = SCL         */

/* TMP007 Register Addresses (see datasheet) */
#define TMP007_VOBJ          0x0000  /*! Sensor voltage result register     */
#define TMP007_DIE_TEMP      0x0001  /*! DIE Temp result register           */
#define TMP007_CONFIG        0x0002  /*! Configuration register             */
#define TMP007_OBJ_TEMP      0x0003  /*! Object Temp Result Register        */
#define TMP007_STATUS        0x0004  /*! Status register                    */
#define TMP007_MASK_ENABLE   0x0005  /*! Status Mask and Enable Register    */
#define TMP007_OBJ_HI_LIM    0x0006  /*! Object Temp High limit register    */
#define TMP007_OBJ_LO_LIM    0x0007  /*! Object Temp Low limit register     */
#define TMP007_DIE_HI_LIM    0x0008  /*! DIE Temp High limit register       */
#define TMP007_DIE_LO_LIM    0x0009  /*! DIE Temp Low limit register        */
#define TMP007_S0            0x000A  /*! S0 coefficient register            */
#define TMP007_A0            0x000B  /*! A0 coefficient register            */
#define TMP007_A1            0x000C  /*! A1 coefficient register            */
#define TMP007_B0            0x000D  /*! B0 coefficient register            */
#define TMP007_B1            0x000E  /*! B1 coefficient register            */
#define TMP007_B2            0x000F  /*! B2 coefficient register            */
#define TMP007_C             0x0010  /*! C coefficient register             */
#define TMP007_TC0           0x0011  /*! TC0 coefficient register           */
#define TMP007_TC1           0x0012  /*! TC1 coefficient register           */
#define TMP007_DEVICEID      0x001F  /*! Manufacturer ID register           */
#define TMP007_MEMORY        0x002A  /*! Memory access register             */

/* Status Mask and Enable Register Bits */
#define TMP007_SMER_ALRTEN   0x8000  /*! Alert Flag Enable Bit              */
#define TMP007_SMER_CRTEN    0x4000  /*! Tem Conversion Ready Enable        */
#define TMP007_SMER_OHEN     0x2000  /*! Object Temp High Limit Enable      */
#define TMP007_SMER_OLEN     0x1000  /*! Object Temp Low Limit Enable       */
#define TMP007_SMER_LHEN     0x0800  /*! DIE Temp High Limit Enable         */
#define TMP007_SMER_LLEN     0x0400  /*! DIE Temp Low Limit Enable          */
#define TMP007_SMER_DVEN     0x0200  /*! Data Invalid Flag Enable Bit       */
#define TMP007_SMER_MEMC_EN  0x0100  /*! Memory Corrupt Enable Bit          */

/* TMP007 Status Register Bits */
#define TMP007_STAT_ALRTF    0x8000  /*! Cumulative Alert Flag              */
#define TMP007_STAT_CRTF     0x4000  /*! Conversion Ready Flag              */
#define TMP007_STAT_OHF      0x2000  /*! Object Temp High Limit Flag        */
#define TMP007_STAT_OLF      0x1000  /*! Object Temp Low Limit Flag         */
#define TMP007_STAT_LHF      0x0800  /*! Local Temp High Limit Flag         */
#define TMP007_STAT_LLF      0x0400  /*! Local Temp Low Limit Flag          */
#define TMP007_STAT_nDVF     0x0200  /*! Data Invalid Flag                  */
#define TMP007_STAT_MCRPT    0x0100  /*! Memory Corrupt Flag                */
#define TMP007_STAT_DOF      0x0080  /*! Data Overflow                      */

/* OPT3001 Register Addresses (see datasheet) */
#define OPT3001_RESULT       0x0000  /*! Result Register (Read-Only)  */
#define OPT3001_CONFIG       0x0001  /*! Configuration Register       */
#define OPT3001_LOLIMIT      0x0002  /*! Low-Limit Register           */
#define OPT3001_HILIMIT      0x0003  /*! High-Limit Register          */
#define OPT3001_MFTID        0x007E  /*! Manufacturer ID Register     */
#define OPT3001_DEVICEID     0x007F  /*! Device ID Register           */

/* OPT3001 Configuration Register Bits */
#define OPT3001_FL           0x0020  /*! Flag Low Field               */
#define OPT3001_FH           0x0040  /*! Flag High Field              */
#define OPT3001_CRF          0x0080  /*! Conversion Ready Field       */
#define OPT3001_OVF          0x0100  /*! Overflow Flag Field          */

#define OPT3001_CONTINUOUS   0x0600  /*! Continuous Conversion Mode   */
#define OPT3001_CONVRD_DIS   0x0000  /*! Conversions Ready            */
#define OPT3001_800MS        0x0800  /*! 800ms Conversion Time        */
#define OPT3001_FAULT1       0x0000  /*! 1 Fault to Trigger Interrupt */
#define OPT3001_AUTO         0xC000  /*! Automatic Range Scaling Mode */
#define OPT3001_LATCH        0x0010  /*! Latched Mode                 */

/* Semaphore to control compression */
extern sem_t semCompress;

/* Logging data */
struct data_packet_t {
    RTC_C_Calendar    timestamp;
    double_t          tDieTemp;
    double_t          tDieTempFahrenheit;
    double_t          tDieTempKelvin;
    double_t          tObjTemp;
    double_t          tObjTempFahrenheit;
    double_t          tObjTempKelvin;
    double_t          convertedLux;
};

extern struct data_packet_t packets[32];
extern Display_Handle displayHandle;
extern Display_Params params;

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_THREAD_H_ */
