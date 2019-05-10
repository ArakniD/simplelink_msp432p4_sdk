/*
 * dma_thread.h
 *
 *  Created on: 17 Apr. 2019
 *      Author: lucas.oldfield
 */

#ifndef DMA_THREAD_H_
#define DMA_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* POSIX Header files */
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include <ti/drivers/UART.h>

/* Global Variables */
extern pthread_t dma_master_thread;
extern pthread_t dma_loopback_thread;
extern sem_t semDMAMaster;
extern sem_t semDMALoopback;

typedef struct DmaMsg {
    int   cmd;
    void *buffer;
    UART_Handle terminalHandle;
} DmaMsg;

#define DMA_MSG_SIZE sizeof(DmaMsg)
#define DMA_MSG_NUM  6

extern mqd_t  mqDMALoopback;
extern mqd_t  mqDMAMaster;

typedef enum DmaCmd {
    dmaCmd_ChangePortSettings = 0,      // valueIntArray=<Baud>,<Timeout>
    dmaCmd_PauseReceiving,              // valueInt=<Master or Loop>
    dmaCmd_ResumeReceiving,             // valueInt=<Master or Loop>
    dmaCmd_SendStream                   // valueInt=<number Of Bytes>
} DmaCmd;

/* The master thread accepts messages from others parts of the application
 * and performs actions and tests as per the request. The actions are to
 * send data to the loopback. and then receive back exactly what is sent,
 * plus record any edge cases, errors or other problems that occur
 */
void *dmaMasterThread(void *arg0);

/* The loop-back thread takes what is read and immediately sends it back out
 * again unchanged. The record of how much as read per iteration and how much
 * is written. along with specific timings of interrupts is recorded in this
 * thread
 */
void *dmaLoopbackThread(void *arg0);

#ifdef __cplusplus
}
#endif

#endif /* DMA_THREAD_H_ */
