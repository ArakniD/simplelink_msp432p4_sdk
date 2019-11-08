/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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

#ifndef ULPMEASURE_H_
#define ULPMEASURE_H_

#define UP      0
#define DOWN    1

#define NUMBER_OF_ZEROCROSSINGS     3

/*
 * The maximum size of the DMA transfer is 1K.  The ADCBUF utilizes the
 * PING-PONG mode of the DMA so that both this ADCBUFFERSIZE is applied
 * to both the primary and the alternate.
 */
#define ADCDMA_BUFFERSIZE          (1024)
#define NUMBER_OF_ADC_BUFFERS      (4)

#define ADC_REFERENCE_VOLTAGE       2.5

/*
 * In ulpMeasure.c the resolution is set to 12-bit and the output format is
 * set to signed binary.  This means that the data will be in a 16-bit
 * signed format with the four lsbs '0'.
 * 0x8000 to 0x7FF0
 * -2048 to +2047
 */
#define ZERO_CROSS                  0

#define SAMPLE_FREQEUNCY            200000

#endif /* ULPMEASURE_H_ */
