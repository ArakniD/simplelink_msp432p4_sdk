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

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "dmaTasks.h"
#include "main.h"

const uint8_t forceP1ISR = 0x04;  // P1.2 is high

const uint32_t clearADC14ENC = 0x40E1190;
const uint32_t clearADC14ON = 0x40E1180;
const uint16_t clearREF_AON = 0x0008;
const uint16_t stopTIMER_A0 = 0x0000;  // timer completely reset

/*
 * Create task list in FLASH for the alternate structure
 */
const DMA_ControlTable altTaskList[NUMBER_OF_TASKS] =
{
    DMA_TaskStructEntry(DMA_BLOCK_SIZE, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[0],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(DMA_BLOCK_SIZE, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*2],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*3],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*4],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*5],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*6],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*7],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*8],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*9],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*10],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*11],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*12],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*13],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*14],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*15],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*16],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*17],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*18],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*19],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*20],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*21],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*22],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*23],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*24],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*25],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*26],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*27],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*28],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*29],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*30],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*31],
            UDMA_ARB_32, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
#ifdef SAMPLES_2K
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
             UDMA_SRC_INC_8, (void *)&forceP1ISR,
             UDMA_DST_INC_8, (void *)&P1->OUT,
             UDMA_ARB_1, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*32],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*33],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*34],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*35],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*36],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*37],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*38],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*39],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*40],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*41],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*42],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*43],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*44],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*45],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*46],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*47],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*48],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*49],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*50],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*51],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*52],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*53],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*54],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*55],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*56],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*57],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*58],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*59],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*60],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*61],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*62],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*63],
            UDMA_ARB_32, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
#endif
#ifdef SAMPLES_4K
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
             UDMA_SRC_INC_8, (void *)&forceP1ISR,
             UDMA_DST_INC_8, (void *)&P1->OUT,
             UDMA_ARB_1, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*64],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*65],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*66],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*67],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*68],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*69],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*70],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*71],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*72],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*73],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*74],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*75],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*76],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*77],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*78],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*79],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*80],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*81],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*82],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*83],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*84],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*85],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*86],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*87],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*88],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*89],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*90],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*91],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*92],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*93],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*94],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*95],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
             UDMA_SRC_INC_8, (void *)&forceP1ISR,
             UDMA_DST_INC_8, (void *)&P1->OUT,
             UDMA_ARB_1, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*96],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*97],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*98],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*99],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*100],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*101],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*102],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*103],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*104],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*105],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*106],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*107],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*108],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*109],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*110],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*111],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*112],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*113],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*114],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*115],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*116],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*117],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*118],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*119],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*120],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*121],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*122],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*123],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*124],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*125],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*126],
            UDMA_ARB_32, (UDMA_MODE_PER_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(32, UDMA_SIZE_32,
            UDMA_SRC_INC_32, &ADC14->MEM[0],
            UDMA_DST_INC_32, (void *)&i32Data_array[DMA_BLOCK_SIZE*127],
            UDMA_ARB_32, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
#endif
    /*
     * Add inctructions to power down the ADC and Reference so that
     * last processing is done at lower power.
     * clearADC14ENC
     * clearADC14ON
     * clearREF_AON
     * stopTIMER_A0
     */
    DMA_TaskStructEntry(1, UDMA_SIZE_32,
             UDMA_SRC_INC_NONE, (void *)&clearADC14ENC,
             UDMA_DST_INC_NONE, (void *)&ADC14->CTL0,
             UDMA_ARB_1, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(1, UDMA_SIZE_16,
             UDMA_SRC_INC_NONE, (void *)&clearREF_AON,
             UDMA_DST_INC_NONE, (void *)&REF_A->CTL0,
             UDMA_ARB_1, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(1, UDMA_SIZE_16,
             UDMA_SRC_INC_NONE, (void *)&stopTIMER_A0,
             UDMA_DST_INC_NONE, (void *)&TIMER_A0->CTL,
             UDMA_ARB_1, (UDMA_MODE_MEM_SCATTER_GATHER)
            ),
    DMA_TaskStructEntry(1, UDMA_SIZE_8,
             UDMA_SRC_INC_NONE, (void *)&forceP1ISR,
             UDMA_DST_INC_NONE, (void *)&P1->OUT,
             UDMA_ARB_1, (UDMA_MODE_MEM_SCATTER_GATHER)
            )
};
