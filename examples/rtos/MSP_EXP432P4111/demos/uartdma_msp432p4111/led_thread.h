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
 *  ======== led_thread.h ========
 */

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include <ti/drivers/PWM.h>

#ifndef __LEDTHREAD_H
#define __LEDTHREAD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LedMsg {
    int   cmd;
    void *buffer;
} LedMsg;

#define LED_MSG_SIZE sizeof(LedMsg)
#define LED_MSG_NUM  6

extern mqd_t  mqLED;

extern pthread_t ledthread_handler;
extern void  *ledThread(void *arg0);

extern enum PwmColors {
    PWM_RED = 0,
    PWM_GREEN,
    PWM_BLUE,

    PWM_COLOR_COUNT
} PwmColors;

typedef enum LedColorModes {
    LED_RED_MODE = 0,
    LED_GREEN_MODE,
    LED_BLUE_MODE,
    LED_RANDOM_MODE,
    LED_PC_CONTROLLED_MODE,
    
    LED_COLOR_MODE_COUNT
} LedColors;

typedef enum LedCmd {
    LedCmd_COLOR_MODE = 0,
    LedCmd_PC_UPDATE,
    LedCmd_BLINK_RATE,
    LedCmd_TOGGLE
} LedCmd;

extern uint16_t   pwmPeriod;
extern PWM_Handle pwms[PWM_COLOR_COUNT];
extern uint16_t   dutys[PWM_COLOR_COUNT];
extern uint32_t   ledTogglePeriods[LED_COLOR_MODE_COUNT];
extern sem_t semLED;
extern bool ledActive;
extern uint16_t numIntervals;

#ifdef __cplusplus
}
#endif

#endif /* __LEDTHREAD_H */
