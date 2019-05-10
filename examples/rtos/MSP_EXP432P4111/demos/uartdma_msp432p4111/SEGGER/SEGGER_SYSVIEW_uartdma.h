/*
 * SEGGER_SYSVIEW_dma.h
 *
 *  Created on: 4 Apr. 2019
 *      Author: lucas.oldfield
 */

#ifndef RTT_SEGGER_SYSVIEW_UARTDMA_H_
#define RTT_SEGGER_SYSVIEW_UARTDMA_H_

#define SYSVIEW_EID_uartReadAsyncEnter  2000              // uartReadAsyncEnter=%p req=%u rel=%u
#define SYSVIEW_EID_uartReadAsyncExit   2001              // uartReadAsyncExit=%p rtnLen=%u ringLen=%u

#define SYSVIEW_EID_uartReadEnter       2002              // uartReadEnter=%p req=%u
#define SYSVIEW_EID_uartReadExit        2003              // uartReadExit=%p rtnLen=%u ringLen=%u

#define SYSVIEW_EID_uartWriteEnter      2004              // uartWriteEnter=%p req=%u
#define SYSVIEW_EID_uartWriteExit       2005              // uartWriteExit=%p rtnLen=%u ringLen=%u

#define SYSVIEW_EID_uartWriteAlloc      2006              // uartWriteAlloc=%p bfr=%u reserve=%u
#define SYSVIEW_EID_uartWriteFree       2007              // uartWriteFree=%p bfr=%u free=%u

#endif /* RTT_SEGGER_SYSVIEW_UARTDMA_H_ */
