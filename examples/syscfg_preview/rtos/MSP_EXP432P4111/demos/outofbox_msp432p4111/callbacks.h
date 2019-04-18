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
 *  ======== callbacks.h ========
 */

/* POSIX Header files */
#include <pthread.h>
#include <ti/drivers/Timer.h>
#include <stdbool.h>

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void debounceGPIO(uint_least8_t index);
extern void gpioButton0_Callback(uint_least8_t index);
extern void gpioButton1_Callback(uint_least8_t index);
extern void intervalTimer_Callback(Timer_Handle myHandle);
extern void blinkTimer_Callback(Timer_Handle myHandle);
extern void debounceTimer_Callback(Timer_Handle myHandle);
extern Timer_Handle intervalTimer, blinkTimer, debounceTimer, lcdScrollTimer;
extern uint16_t S1buttonDebounce, S2buttonDebounce;
extern bool counting;
extern bool scrolling;

#ifdef __cplusplus
}
#endif

#endif /* CALLBACKS_H_ */
