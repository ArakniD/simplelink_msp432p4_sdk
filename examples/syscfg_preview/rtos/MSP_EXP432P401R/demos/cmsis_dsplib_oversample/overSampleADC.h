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

#ifndef OSAMPLEADC_H_
#define OSAMPLEADC_H_

/*
 * The DMA is used to collect 1024 measurement blocks.  Each block is decimated
 * by the oversampling factor and added to the final array.
 *
 */
#define NUM_OF_TAPS                 32
#define OVERSAMPLING_FACTOR         32
#define DECIMATION_FACTOR           OVERSAMPLING_FACTOR
#define BLOCK_SIZE                  1024          // DMA Size
#define DECIMATED_BlOCK_LENGTH      BLOCK_SIZE/OVERSAMPLING_FACTOR  //  1024/32 = 32
#define ARRAY_LENGTH                256*DECIMATED_BlOCK_LENGTH   //must be a multiple DECIMATED_BlOCK_LENGTH, 8K
#define BLOCK_REPEAT                ARRAY_LENGTH*OVERSAMPLING_FACTOR/BLOCK_SIZE  // 256 DMA transfers of 1K each

/*
 * The maximum size of the DMA transfer is 1K.  The ADCBUF utilizes the
 * PING-PONG mode of the DMA so that both this ADCBUFFERSIZE is applied
 * to both the primary and the alternate.
 */
#define ADCDMA_BUFFERSIZE          (BLOCK_SIZE)
#define NUMBER_OF_ADC_BUFFERS      (BLOCK_REPEAT)

#define ADC_REFERENCE_VOLTAGE       2.5

/*
 * 1Msps sampling rate
 */
#define SAMPLE_FREQEUNCY            1000000

#endif /* OSAMPLEADC_H_ */
