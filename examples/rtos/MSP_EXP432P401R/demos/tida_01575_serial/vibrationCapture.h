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

#ifndef VIBRATIONCAPTURE_H_
#define VIBRATIONCAPTURE_H_
#include <stdbool.h>
#include <semaphore.h>
#include "arm_math.h"

/* ADC Sample Size */
#ifdef DeviceFamily_MSP432P4x1xI
	#define ADCBUFFERSIZE    4096
#else
	#define ADCBUFFERSIZE    1024
#endif
#define FFTSIZE          ADCBUFFERSIZE/2

#if AVG_LOOP_COUNT == 0
#error "AVG_LOOP_COUNT cannot be 0"
#endif
/* -------------------------------------------------------------------
 * Structure to store the iteration count, range of bin of interest,
 * value to monitor in the range of bin, alarm condition to monitor
 * across the spectrum and energy value of each bin.
 * ------------------------------------------------------------------- */
typedef struct _Energy_Bin
{
    uint8_t  iteration;
    float32_t energyValue[FFTSIZE];
    float32_t maxValue;
    uint16_t  maxValueBin;
    bool      dataReady;
} Energy_Bin;

extern Energy_Bin XAxisEnergyBin;
extern Energy_Bin YAxisEnergyBin;
extern Energy_Bin ZAxisEnergyBin;

/* Task configuration */
#define VIBRATION_TASK_PRIORITY        1
#define VIBRATION_TASK_STACK_SIZE      1024

#define ADC_NO_OF_CH                   3
#define ADC_CH_SAMPLING_FREQ           24000

/* Task creation function for the Application Processor. */
void vibrationTask(void);

#endif /* VIBRATIONCAPTURE_H_ */
