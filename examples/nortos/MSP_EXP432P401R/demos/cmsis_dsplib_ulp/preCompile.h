/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/
#ifndef PRECOMPILE_H_
#define PRECOMPILE_H_

#define UP      0
#define DOWN    1

#define NUMBER_OF_ZEROCROSSINGS 3
#define ZERO_CROSS              2047

#define DMA_BLOCK_SIZE          32  // size of each scatter-gather task
#define INTERMEDIATE_PROC_SIZE  32*32  // 32 tasks and 32 conversions for each task

/*
 * Defines the number of samples to be taken, do not adjust sizes above
 * 4K without also increasing RAM allocation - and enable/disable correct
 * banks
 * when using 4K, SAMPLES_2K must also be defined. See dmaTasks.c
 */

#define SAMPLES_4K              4096
#define SAMPLE_FREQEUNCY        200000.0f
#define SETTLE_TIME_DELAY       1680-82

#define SAMPLES_2K              2048
//#define SAMPLE_FREQEUNCY        100000.0f
//#define SETTLE_TIME_DELAY       1680-202

#define SAMPLES_1K              1024
//#define SAMPLE_FREQEUNCY        50000.0f
//#define SETTLE_TIME_DELAY       1680-432

#define SAMPLE_LENGTH          SAMPLES_4K //1K  // 2K
#define NUMBER_OF_TASKS        (SAMPLE_LENGTH/DMA_BLOCK_SIZE)+(SAMPLE_LENGTH/1024) + 3

/*
 * Sample frequency is used to trigger timer and calculate frequency of
 * measured waveform.  Clock frequency is DCO/2
 */

#define SAMPLE_CLOCK_FREQUENCY  12000000

#endif /* PRECOMPILE_H_ */
