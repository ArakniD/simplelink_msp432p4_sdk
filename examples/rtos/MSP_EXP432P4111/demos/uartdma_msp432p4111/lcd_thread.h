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
 *  ======== lcd_thread.h ========
 */

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include <ti/drivers/PWM.h>

#ifndef __LCDTHREAD_H
#define __LCDTHREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_MSG_SIZE sizeof(LcdMsg)
#define LCD_MSG_NUM  1

typedef struct LcdMsg {
    int   cmd;
    void *buffer;
} LcdMsg;

typedef enum LcdCmd {
    LcdCmd_SHOW_STRING = 0,
    LcdCmd_SCROLL_STRING,
    LcdCmd_SHOW_TEMPERATURE,
    LcdCmd_UPDATE_MEMORY,
    LcdCmd_UPDATE_ANIMATION_MEMORY,
    LcdCmd_START_ANIMATION,
    LcdCmd_CLEAR_MEMORY
} LcdCmd;

extern sem_t semLCD;
extern mqd_t  mqLCD;

extern pthread_t lcdthread_handler;
extern void  *lcdThread(void *arg0);

#ifdef __cplusplus
}
#endif

#endif /* __LEDTHREAD_H */
