/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       UARTMSP432.h
 *
 *  @brief      UART driver implementation for a EUSCIA peripheral for MSP432
 *
 *  This UART driver implementation is designed to operate on a EUSCI controller
 *  in UART mode for MSP432 devices.
 *
 *  The UART header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/UART.h>
 *  #include <ti/drivers/uart/UARTMSP432.h>
 *  @endcode
 *
 *  Refer to @ref UART.h for a complete description of APIs and example of use.
 *
 *  # Device Specific Pin Mode Macros #
 *  This header file contains port/pin macros for pin configuration. These
 *  macros are used to select the pins used for UART TX and RX in the
 *  UARTMSP432DMA_HWAttrsV1 structure.  The following is a description of the
 *  port/pin macro bit masks.
 *
 *  The port mapping controller on MSP432P401x MCUs allows reconfigurable
 *  mapping of digital functions on ports P2, P3, and P7.  For these ports,
 *  the pin function value is encoded in bits 10-15 of the port/pin
 *  macro.  This value ranges from 0 to 31.  (You can find the port mapping
 *  definitions in the device data sheet.)
 *
 *  The pin ranges from 0 - 7 and is encoded in bits 0 - 3 (bit 3 is not
 *  actually used).  Bits 4 - 7 of the port/pin macro are used to encode
 *  the port.  For ports P2, P3, and P7, bits 8 and 9 are not used.  The
 *  table below shows the bit mapping for P2, P3, and P7 port/pin macros.
 *
 *    15 - 10 | 9 | 8 | 7 - 4 | 3 - 0 |
 *  ---------- | - | - | ----- | ------ |
 *    VALUE   | X | X | PORT | PIN |
 *
 *  Some pins that are not on ports P2, P3, or P7, can be configured as
 *  UART TX or UART RX pins (e.g., P1.2 and P1.3).  The macros for these pins
 *  will have 0 for bits 20 - 12.  Bits 8 and 9 are used to hold the control
 *  bits, PnSEL1.x and PnSEL0.x, which determine the module function (PRIMARY,
 *  SECONDARY, or TERTIALRY).  The table below shows the bit mapping for
 *  non port-mapped port/pin macros for pins that can be configured as UART
 *  TX or UART RX pins.
 *
 *    15 - 10 | 9 | 8 | 7 - 4 | 3 - 0 |
 *  ---------- | - | - | ----- | ------ |
 *    X   | PnSEL1.x | PnSEL0.x | PORT | PIN |
 *
 *
 *  ============================================================================
 */

#ifndef ti_drivers_uart_UARTMSP432DMA__include
#define ti_drivers_uart_UARTMSP432DMA__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/dma/UDMAMSP432.h>
#include <ti/drivers/utils/RingBuf.h>

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/dma.h>

#define UARTMSP432DMA_P1_2_UCA0RXD  0x00000112  /* Primary, port 1, pin 2 */
#define UARTMSP432DMA_P1_3_UCA0TXD  0x00000113  /* Primary, port 1, pin 3 */

/* Port 2, pin 0 */
#define UARTMSP432DMA_P2_0_UCA0RXD ((PMAP_UCA0RXD << 10) | 0x20)
#define UARTMSP432DMA_P2_0_UCA0TXD ((PMAP_UCA0TXD << 10) | 0x20)
#define UARTMSP432DMA_P2_0_UCA1RXD ((PMAP_UCA1RXD << 10) | 0x20)
#define UARTMSP432DMA_P2_0_UCA1TXD ((PMAP_UCA1TXD << 10) | 0x20)
#define UARTMSP432DMA_P2_0_UCA2RXD ((PMAP_UCA2RXD << 10) | 0x20)
#define UARTMSP432DMA_P2_0_UCA2TXD ((PMAP_UCA2TXD << 10) | 0x20)

/* Port 2, pin 1 */
#define UARTMSP432DMA_P2_1_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x21)
#define UARTMSP432DMA_P2_1_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x21)
#define UARTMSP432DMA_P2_1_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x21)
#define UARTMSP432DMA_P2_1_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x21)
#define UARTMSP432DMA_P2_1_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x21)
#define UARTMSP432DMA_P2_1_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x21)

/* Port 2, pin 2 */
#define UARTMSP432DMA_P2_2_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x22)
#define UARTMSP432DMA_P2_2_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x22)
#define UARTMSP432DMA_P2_2_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x22)
#define UARTMSP432DMA_P2_2_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x22)
#define UARTMSP432DMA_P2_2_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x22)
#define UARTMSP432DMA_P2_2_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x22)

/* Port 2, pin 3 */
#define UARTMSP432DMA_P2_3_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x23)
#define UARTMSP432DMA_P2_3_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x23)
#define UARTMSP432DMA_P2_3_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x23)
#define UARTMSP432DMA_P2_3_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x23)
#define UARTMSP432DMA_P2_3_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x23)
#define UARTMSP432DMA_P2_3_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x23)

/* Port 2, pin 4 */
#define UARTMSP432DMA_P2_4_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x24)
#define UARTMSP432DMA_P2_4_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x24)
#define UARTMSP432DMA_P2_4_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x24)
#define UARTMSP432DMA_P2_4_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x24)
#define UARTMSP432DMA_P2_4_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x24)
#define UARTMSP432DMA_P2_4_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x24)

/* Port 2, pin 5 */
#define UARTMSP432DMA_P2_5_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x25)
#define UARTMSP432DMA_P2_5_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x25)
#define UARTMSP432DMA_P2_5_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x25)
#define UARTMSP432DMA_P2_5_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x25)
#define UARTMSP432DMA_P2_5_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x25)
#define UARTMSP432DMA_P2_5_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x25)

/* Port 2, pin 6 */
#define UARTMSP432DMA_P2_6_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x26)
#define UARTMSP432DMA_P2_6_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x26)
#define UARTMSP432DMA_P2_6_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x26)
#define UARTMSP432DMA_P2_6_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x26)
#define UARTMSP432DMA_P2_6_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x26)
#define UARTMSP432DMA_P2_6_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x26)

/* Port 2, pin 7 */
#define UARTMSP432DMA_P2_7_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x27)
#define UARTMSP432DMA_P2_7_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x27)
#define UARTMSP432DMA_P2_7_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x27)
#define UARTMSP432DMA_P2_7_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x27)
#define UARTMSP432DMA_P2_7_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x27)
#define UARTMSP432DMA_P2_7_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x27)

/* Port 3, pin 0 */
#define UARTMSP432DMA_P3_0_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x30)
#define UARTMSP432DMA_P3_0_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x30)
#define UARTMSP432DMA_P3_0_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x30)
#define UARTMSP432DMA_P3_0_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x30)
#define UARTMSP432DMA_P3_0_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x30)
#define UARTMSP432DMA_P3_0_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x30)

/* Port 3, pin 1 */
#define UARTMSP432DMA_P3_1_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x31)
#define UARTMSP432DMA_P3_1_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x31)
#define UARTMSP432DMA_P3_1_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x31)
#define UARTMSP432DMA_P3_1_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x31)
#define UARTMSP432DMA_P3_1_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x31)
#define UARTMSP432DMA_P3_1_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x31)

/* Port 3, pin 2 */
#define UARTMSP432DMA_P3_2_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x32)
#define UARTMSP432DMA_P3_2_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x32)
#define UARTMSP432DMA_P3_2_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x32)
#define UARTMSP432DMA_P3_2_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x32)
#define UARTMSP432DMA_P3_2_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x32)
#define UARTMSP432DMA_P3_2_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x32)

/* Port 3, pin 3 */
#define UARTMSP432DMA_P3_3_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x33)
#define UARTMSP432DMA_P3_3_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x33)
#define UARTMSP432DMA_P3_3_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x33)
#define UARTMSP432DMA_P3_3_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x33)
#define UARTMSP432DMA_P3_3_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x33)
#define UARTMSP432DMA_P3_3_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x33)

/* Port 3, pin 4 */
#define UARTMSP432DMA_P3_4_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x34)
#define UARTMSP432DMA_P3_4_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x34)
#define UARTMSP432DMA_P3_4_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x34)
#define UARTMSP432DMA_P3_4_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x34)
#define UARTMSP432DMA_P3_4_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x34)
#define UARTMSP432DMA_P3_4_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x34)

/* Port 3, pin 5 */
#define UARTMSP432DMA_P3_5_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x35)
#define UARTMSP432DMA_P3_5_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x35)
#define UARTMSP432DMA_P3_5_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x35)
#define UARTMSP432DMA_P3_5_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x35)
#define UARTMSP432DMA_P3_5_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x35)
#define UARTMSP432DMA_P3_5_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x35)

/* Port 3, pin 6 */
#define UARTMSP432DMA_P3_6_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x36)
#define UARTMSP432DMA_P3_6_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x36)
#define UARTMSP432DMA_P3_6_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x36)
#define UARTMSP432DMA_P3_6_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x36)
#define UARTMSP432DMA_P3_6_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x36)
#define UARTMSP432DMA_P3_6_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x36)

/* Port 3, pin 7 */
#define UARTMSP432DMA_P3_7_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x37)
#define UARTMSP432DMA_P3_7_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x37)
#define UARTMSP432DMA_P3_7_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x37)
#define UARTMSP432DMA_P3_7_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x37)
#define UARTMSP432DMA_P3_7_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x37)
#define UARTMSP432DMA_P3_7_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x37)

/* Port 7, pin 0 */
#define UARTMSP432DMA_P7_0_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x70)
#define UARTMSP432DMA_P7_0_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x70)
#define UARTMSP432DMA_P7_0_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x70)
#define UARTMSP432DMA_P7_0_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x70)
#define UARTMSP432DMA_P7_0_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x70)
#define UARTMSP432DMA_P7_0_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x70)

/* Port 7, pin 1 */
#define UARTMSP432DMA_P7_1_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x71)
#define UARTMSP432DMA_P7_1_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x71)
#define UARTMSP432DMA_P7_1_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x71)
#define UARTMSP432DMA_P7_1_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x71)
#define UARTMSP432DMA_P7_1_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x71)
#define UARTMSP432DMA_P7_1_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x71)

/* Port 7, pin 2 */
#define UARTMSP432DMA_P7_2_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x72)
#define UARTMSP432DMA_P7_2_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x72)
#define UARTMSP432DMA_P7_2_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x72)
#define UARTMSP432DMA_P7_2_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x72)
#define UARTMSP432DMA_P7_2_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x72)
#define UARTMSP432DMA_P7_2_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x72)

/* Port 7, pin 3 */
#define UARTMSP432DMA_P7_3_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x73)
#define UARTMSP432DMA_P7_3_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x73)
#define UARTMSP432DMA_P7_3_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x73)
#define UARTMSP432DMA_P7_3_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x73)
#define UARTMSP432DMA_P7_3_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x73)
#define UARTMSP432DMA_P7_3_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x73)

/* Port 7, pin 4 */
#define UARTMSP432DMA_P7_4_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x74)
#define UARTMSP432DMA_P7_4_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x74)
#define UARTMSP432DMA_P7_4_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x74)
#define UARTMSP432DMA_P7_4_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x74)
#define UARTMSP432DMA_P7_4_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x74)
#define UARTMSP432DMA_P7_4_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x74)

/* Port 7, pin 5 */
#define UARTMSP432DMA_P7_5_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x75)
#define UARTMSP432DMA_P7_5_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x75)
#define UARTMSP432DMA_P7_5_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x75)
#define UARTMSP432DMA_P7_5_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x75)
#define UARTMSP432DMA_P7_5_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x75)
#define UARTMSP432DMA_P7_5_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x75)

/* Port 7, pin 6 */
#define UARTMSP432DMA_P7_6_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x76)
#define UARTMSP432DMA_P7_6_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x76)
#define UARTMSP432DMA_P7_6_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x76)
#define UARTMSP432DMA_P7_6_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x76)
#define UARTMSP432DMA_P7_6_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x76)
#define UARTMSP432DMA_P7_6_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x76)

/* Port 7, pin 7 */
#define UARTMSP432DMA_P7_7_UCA0RXD  ((PMAP_UCA0RXD << 10) | 0x77)
#define UARTMSP432DMA_P7_7_UCA0TXD  ((PMAP_UCA0TXD << 10) | 0x77)
#define UARTMSP432DMA_P7_7_UCA1RXD  ((PMAP_UCA1RXD << 10) | 0x77)
#define UARTMSP432DMA_P7_7_UCA1TXD  ((PMAP_UCA1TXD << 10) | 0x77)
#define UARTMSP432DMA_P7_7_UCA2RXD  ((PMAP_UCA2RXD << 10) | 0x77)
#define UARTMSP432DMA_P7_7_UCA2TXD  ((PMAP_UCA2TXD << 10) | 0x77)

#define UARTMSP432DMA_P9_6_UCA3RXD  0x00000196  /* Primary, port 9, pin 6 */
#define UARTMSP432DMA_P9_7_UCA3TXD  0x00000197  /* Primeary, port 9, pin 7 */

/**
 *  @addtogroup UART_STATUS
 *  UARTMSP432DMA_STATUS_* macros are command codes only defined in the
 *  UARTMSP432.h driver implementation and need to:
 *  @code
 *  #include <ti/drivers/uart/UARTMSP432.h>
 *  @endcode
 *  @{
 */

/* Add UARTMSP432DMA_STATUS_* macros here */

/** @}*/

/**
 *  @addtogroup UART_CMD
 *  UARTMSP432DMA_CMD_* macros are command codes only defined in the
 *  UARTMSP432DMA.h driver implementation and need to:
 *  @code
 *  #include <ti/drivers/uart/UARTMSP432DMA.h>
 *  @endcode
 *  @{
 */

/* Add UARTMSP432DMA_CMD_* macros here */

/** @}*/

/* UART function table pointer */
extern const UART_FxnTable UARTMSP432DMA_fxnTable;

/*!
 *  @brief      The definition of an optional callback function used by the UART
 *              driver to notify the application when a receive error (FIFO overrun,
 *              parity error, etc) occurs.
 *
 *  @param      UART_Handle             UART_Handle
 *
 *  @param      error                   The current value of the receive
 *                                      status register.  Please refer to the
 *                                      device data sheet to interpret this
 *                                      value.
 */
typedef long (*UARTMSP432DMA_ErrorCallback) (UART_Handle handle,  uint32_t error);

/*!
 *  @brief      UARTMSP432 Baudrate configuration
 *
 *  This structure is used to specifiy the EUSCI controller's clock divider
 *  settings to achieve the desired baudrate given the a clock input frequency.
 *  Divider values can be determined by referring to the MSP432 baudrate
 *  calculator.
 *  http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *
 *  A sample structure is shown below:
 *  @code
 *  //
 *  // The baudrate dividers were determined by using the MSP432 baudrate
 *  // calculator
 *  // http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *  //
 *  const UARTMSP432DMA_BaudrateConfig uartMSP432Baudrates[] = {
 *      // {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling}
 *      {921600, 24e6,   1, 10,   0, 1},
 *      {921600, 16e6,  17,  0,  74, 0},
 *      {921600,  6e6,   6,  0, 170, 0},
 *      {460800, 24e6,   3,  4,   2, 1},
 *      {460800, 16e6,   2,  2, 187, 1},
 *      {460800,  6e6,  13,  0,   0, 0},
 *      {230400, 24e6,   6,  8,  32, 1},
 *      {230400, 16e6,   4,  5,  85, 1},
 *      {230400,  6e6,   1, 10,   0, 1},
 *      {115200, 24e6,   13, 0,  37, 1},
 *      {115200, 16e6,   8, 10, 247, 1},
 *      {115200, 12e6,   6,  8,  32, 1},
 *      {115200, 06e6,   3,  4,   2, 1},
 *      {115200, 03e6,   1, 10,   0, 1},
 *      {9600,   24e6, 156,  4,   0, 1},
 *      {9600,   16e6, 104,  2, 182, 1},
 *      {9600,   12e6,  78,  2,   0, 1},
 *      {9600,   06e6,  39,  1,   0, 1},
 *      {9600,   03e6,  19,  8,  85, 1},
 *      {9600,  32768,   3,  0, 146, 0}
 *  };
 *  @endcode
 */
typedef struct UARTMSP432DMA_BaudrateConfig {
    uint32_t  outputBaudrate; /*!< Search criteria: desired baudrate */
    uint32_t  inputClockFreq; /*!< Search criteria: given this input clock frequency */

    uint16_t  prescalar;      /*!< Clock prescalar */
    uint8_t   hwRegUCBRFx;    /*!< UCBRFx lookup entry */
    uint8_t   hwRegUCBRSx;    /*!< UCBRSx lookup entry */
    uint8_t   oversampling;   /*!< Oversampling mode (1: True; 0: False) */
} UARTMSP432DMA_BaudrateConfig;

/*!
 *  @brief      UARTMSP432 Hardware attributes
 *
 *  These fields are used by driverlib APIs and therefore must be populated by
 *  driverlib macro definitions. For MSP432Ware these definitions are found in:
 *      - uart.h
 *
 *  intPriority is the UART peripheral's interrupt priority, as defined by the
 *  underlying OS.  It is passed unmodified to the underlying OS's interrupt
 *  handler creation code, so you need to refer to the OS documentation
 *  for usage.  For example, for SYS/BIOS applications, refer to the
 *  ti.sysbios.family.arm.m3.Hwi documentation for SYS/BIOS usage of
 *  interrupt priorities.  If the driver uses the ti.dpl interface
 *  instead of making OS calls directly, then the HwiP port handles the
 *  interrupt priority in an OS specific way.  In the case of the SYS/BIOS
 *  port, intPriority is passed unmodified to Hwi_create().
 *
 *  A sample structure is shown below:
 *  @code
 *
 *  #include <ti/drivers/UART.h>
 *  #include <ti/drivers/uart/UARTMSP432DMA.h>
 *
 *  static UARTMSP432DMA_Object uartMSP432Objects[MSP432P401X_UARTCOUNT];
 *  static unsigned char uartMSP432RingBuffer[MSP432P401X_UARTCOUNT][1024];
 *  static unsigned char uartMSP432WriteBuffer[MSP432P401X_UARTCOUNT][1024];
 *
 *  extern long hal_uart_error(UART_Handle handle,  uint32_t error);
 *
 *  const UARTMSP432DMA_HWAttrsV1 uartMSP432HWAttrs[MSP432P401X_UARTCOUNT] = {
 *      {
 *          .baseAddr = EUSCI_A2_BASE,
 *          .intNum = INT_EUSCIA2,
 *          .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
 *          .bitOrder = EUSCI_A_UART_LSB_FIRST,
 *          .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
 *              sizeof(UARTMSP432DMA_BaudrateConfig),
 *          .baudrateLUT = uartMSP432Baudrates,
 *          .ringBufPtr  = uartMSP432RingBuffer[MSP432P401X_UART_BT],
 *          .writeBufPtr = uartMSP432WriteBuffer[MSP432P401X_UART_BT],
 *          .ringBufSize = sizeof(uartMSP432RingBuffer[MSP432P401X_UART_BT]),
 *          .writeBufSize = sizeof(uartMSP432WriteBuffer[MSP432P401X_UART_BT]),
 *          .rxPin = UARTMSP432DMA_P2_1_UCA2RXD,
 *          .txPin = UARTMSP432DMA_P2_0_UCA2TXD,
 *          .rtsPinBitBand = (volatile uint16_t *)BITBAND_PERI_ADR(P3->OUT, 6), // P3.6 direct output
 *          .errorFxn = hal_uart_error,
 *          .timeoutTmrIdx = MSP432P401X_TIMER_T32_1,
 *          .dmaIntNum = DMA_INT0,
 *          .bfrDMAChannelIndex = DMA_CH3_RESERVED0,
 *          .rxDMAChannelIndex = DMA_CH5_EUSCIA2RX,
 *          .txDMAChannelIndex = DMA_CH4_EUSCIA2TX,
 *          .intPriority = NVIC_PRIO(DMA_PRIO)
 *      }
 *  };
 *
 *  const UART_Config UART_config[MSP432P401X_UARTCOUNT] = {
 *      {   // Bluetooth UART
 *          .fxnTablePtr = &UARTMSP432DMA_fxnTable,
 *          .object = &uartMSP432Objects[MSP432P401X_UART_BT],
 *          .hwAttrs = &uartMSP432HWAttrs[MSP432P401X_UART_BT]
 *      }
 *  };
 *
 *  const uint_least8_t UART_count = MSP432P401X_UARTCOUNT;
 *  @endcode
 */
typedef struct UARTMSP432DMA_HWAttrsV1 {
    /*! UART Peripheral's base address */
    unsigned int    baseAddr;
    /*! UART Peripheral's interrupt vector */
    unsigned int    intNum;
    /*! UART Peripheral's interrupt priority */
    unsigned int    intPriority;
    /*! UART Clock source */
    uint8_t         clockSource;
    /*! UART Bit order */
    uint32_t        bitOrder;
    /*! Number of UARTMSP432DMA_BaudrateConfig entries */
    uint8_t         numBaudrateEntries;
    /*! Pointer to a table of possible UARTMSP432DMA_BaudrateConfig entries */
    UARTMSP432DMA_BaudrateConfig const *baudrateLUT;
    /*! Pointer to a application ring buffer */
    unsigned char  *ringBufPtr;
    /*! Size of ringBufPtr */
    size_t          ringBufSize;
    /*! Number of ring buf blocks, must be a equal division of ringBufSize */
    unsigned char   ringBufBlockCnt;
    /*! Minimum block size to allows a DMA RX operation */
    size_t          ringBufBlockSizeMin;
    /*! Pointer to a application ring buffer */
    unsigned char  *writeBufPtr;
    /*! Size of ringBufPtr */
    size_t          writeBufSize;

    volatile uint16_t *rtsPinBitBand;

    uint16_t        rxPin;
    uint16_t        txPin;
    /*! Application error function to be called on receive errors */
    UARTMSP432DMA_ErrorCallback errorFxn;
    /* Index of the timer to use */
    int             timeoutTmrIdx;

    uint8_t  dmaIntNum;        /*!< DMA interrupt number */
    uint32_t bfrDMAChannelIndex; /*!< DMA bfrDMAChnelPingPong for ping pong of data */
    uint32_t rxDMAChannelIndex;  /*!< DMA rxDMAChannel for Rx data */
    uint32_t txDMAChannelIndex;  /*!< DMA txDMAChannel for Tx data */
} UARTMSP432DMA_HWAttrsV1;

/*!
 *  @brief      UARTMSP432 Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct UARTMSP432DMA_Object {
    /* UART state variable */
    struct {
        bool             opened:1;         /* Has the obj been opened */
        /*
         * Flag to determine if a timeout has occurred when the user called
         * UART_read(). This flag is set by the timeoutClk clock object.
         */
        bool             bufTimeout:1;
    } state;

    HwiP_Handle          hwiHandle;        /* Hwi handle for interrupts */
    Timer_Handle         timeoutTmr;       /* Use a hardware based timer */
    uint32_t             baudRate;         /* Baud rate for UART */
    UART_STOP            stopBits;         /* Stop bits for UART */
    UART_PAR             parityType;       /* Parity bit type for UART */

    Power_NotifyObj      perfChangeNotify;
    uint32_t             perfConstraintMask;

    /* UART read variables */
    RingBuf_Object       readBuffer;
    size_t               readAsyncPrev;    /* size of previous read still to be free */
    size_t               ringBlkSize;      /* Size used for each DMA Ping-Pong transfer */
    size_t               ringBlkSizeMin;   /* Minimum block size valid before RTS assertion */
    size_t               rx_readMin;       /* Minimum number of bytes to return */

    SemaphoreP_Handle    readSem;          /* UART read semaphore */
    unsigned int         readTimeout;      /* Timeout for read semaphore */

    UDMAMSP432_Handle    rxdmaHandle;
    UDMAMSP432_Handle    rxdmaBfrHandle;
    UDMAMSP432_PingPongTransfer rx_transfer;
    volatile uint8_t     rx_dmaRxBuffer;
    volatile uint32_t   *rx_dmaAltSetBitBand;
    volatile uint32_t   *rx_dmaAltClrBitBand;
    volatile uint32_t    rx_dmaSwReq;
    volatile uint32_t    rx_dmaIdleTimeout;
    volatile uint32_t    rx_dmaIdleControl;
    volatile uint32_t    rx_dmaTaskControl;
    volatile uint32_t   *rx_dmaCtrl[eUDMAPingPongMAX];
    volatile uint32_t   *rx_dmaTaskCtrl;
    volatile uint32_t   *rx_timer32LoadAdr;
    volatile uint32_t   *rx_timer32ControlAdr;
    volatile uint16_t   *rtsPinBitBand;    /* RTS pin used to signal that buffers are full */
    DMA_ControlTable     rx_dmaTaskList[4]; /* DMA task list */

    /* UART write variables */
    RingBuf_Object       writeBuffer;      /* local circular buffer object */
    uint8_t              tx_toggle;        /* ping-pong toggle */
    uint8_t              tx_depth;         /* ping-pong outstanding */
    /* UART write variables */
    size_t               written;          /* Number of bytes last written */
    SemaphoreP_Handle    writeSem;         /* UART write semaphore*/
    unsigned int         writeTimeout;     /* Timeout for write semaphore */

    UDMAMSP432_Handle    txdmaHandle;
    UDMAMSP432_PingPongTransfer  tx_transfer;
    bool                 tx_cancelInProgress;
    volatile uint32_t   *tx_dmaAltSetBitBand;
    volatile uint32_t   *tx_dmaAltClrBitBand;
    volatile uint32_t   *tx_dmaCtrl[eUDMAPingPongMAX];
    volatile uint32_t   *tx_dmaPPbit[eUDMAPingPongMAX];
} UARTMSP432DMA_Object, *UARTMSP432DMA_Handle;

int_fast32_t UARTMSP432DMA_readAsync(UART_Handle handle, size_t request, int release, volatile uint8_t **buffer, size_t *size);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_uart_UARTMSP432DMA__include */
