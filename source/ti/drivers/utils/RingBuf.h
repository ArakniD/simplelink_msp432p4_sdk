/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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

#ifndef ti_drivers_uart_RingBuf__include
#define ti_drivers_uart_RingBuf__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct RingBuf_Object {
    unsigned char      *buffer;
    size_t              length;
    size_t              count;
    size_t              head; // last used entry index
    size_t              tail; // first used entry index
    size_t              allocated;  // count of bytes allocated to an external DMA writer
    size_t              reserved;   // count of bytes allocated to an external DMA reader
} RingBuf_Object, *RingBuf_Handle;

/*!
 *  @brief  Initialize circular buffer
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *
 *  @param  bufPtr  Pointer to data buffer to be used for the circular buffer.
 *                  The buffer is NOT stored in RingBuf_Object.
 *
 *  @param  bufSize The size of bufPtr in number of unsigned chars.
 */
void RingBuf_construct(RingBuf_Handle object, unsigned char *bufPtr,
    size_t bufSize);

/*!
 *  @brief  Get an unsigned char from the end of the circular buffer and remove
 *          it.
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *
 *  @param  data    Pointer to an unsigned char to be filled with the data from
 *                  the front of the circular buffer.
 *
 *  @return         Number of unsigned chars on the buffer after taking it out
 *                  of the circular buffer. If it returns -1, the circular
 *                  buffer was already empty and data is invalid.
 */
int RingBuf_get(RingBuf_Handle object, unsigned char *data);

/*!
 *  @brief  Get the number of unsigned chars currently stored on the circular
 *          buffer.
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *          variables to operate a circular buffer.
 *
 *  @return         Number of unsigned chars on the circular buffer.
 */
int RingBuf_getCount(RingBuf_Handle object);

/*!
 *  @brief  Function to determine if the circular buffer is full or not.
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *
 *  @return         true if circular buffer is full, else false.
 */
bool RingBuf_isFull(RingBuf_Handle object);

/*!
 *  @brief  Get the unallocated size of the ringBuffer. This is the bytes free, less the bytes already allocated via calls
 *  to allocContigious
 */
size_t RingBuf_getUnallocated(RingBuf_Handle object);
/*!
 *  @brief  Get the allocated size of the ringBuffer. This is the bytes that are being pre-allocated for a Rx operation to
 *  write into, from an interrupt or DMA transaction. The allocated buffer is in a volatile state and may be written to as data
 *  read from a port or device.
 */
int RingBuf_getAllocated(RingBuf_Handle object);
/*!
 *  @brief  Get the reserved size of the ringBuffer. This is the bytes that are being reserved for a Tx operation which will stream
 *  the buffered data via an interrupt or DMA transaction. The reserved buffer will be released back to the ring buffer once
 *  transmission is complete so more data can be buffered for sending.
 */
int RingBuf_getReserved(RingBuf_Handle object);
/*!
 *  @brief  Get an unsigned char from the end of the circular buffer without
 *          removing it.
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *
 *  @param  data    Pointer to an unsigned char to be filled with the data from
 *                  the front of the circular buffer. This function does not
 *                  remove the data from the circular buffer. Do not evaluate
 *                  data if the count returned is equal to 0.
 *
 *  @return         Number of unsigned chars on the circular buffer. If the
 *                  number != 0, then data will contain the unsigned char at the
 *                  end of the circular buffer.
 */
int RingBuf_peek(RingBuf_Handle object, unsigned char *data);

/*!
 *  @brief  Put an unsigned char into the end of the circular buffer.
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *
 *  @param  data    unsigned char to be placed at the end of the circular
 *                  buffer.
 *
 *  @return         Number of unsigned chars on the buffer after it was added,
 *                  or -1 if it's already full.
 */
int RingBuf_put(RingBuf_Handle object, unsigned char data);

/*!
 *  @brief  Clear the ring buffer.
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 */
void RingBuf_clear(RingBuf_Handle object);

/*!
 *  @brief  Get free elements in the ring buffer
 *
 *  @param  object  Pointer to a RingBuf Object that contains the member
 *                  variables to operate a circular buffer.
 *
 *  @return         Number of free unsigned chars on the buffer
 */
int RingBuf_getFree(RingBuf_Handle object);

/*!
 *  @brief  Get the next head pointer with requested max size in the ringBuffer. Allows the buffer
 *  to handle any wrapping customizations
 */
size_t RingBuf_alloc_put_contiguous(RingBuf_Handle object, size_t size, unsigned char **ringPointer);
/*!
 *  @brief  Reserve the tail onwards with max size in the ringBuffer. Allows the buffer to know where
 *  data is being removed from.
 */
size_t RingBuf_alloc_get_contiguous(RingBuf_Handle object, size_t size, unsigned char **ringPointer);
/*!
 *  @brief  Free the bytes allocated by get_contiguous and advance the pointers forward as data has now
 *  been consumed from the tail
 */
size_t RingBuf_free_get_contiguous(RingBuf_Handle object, size_t n);
/*!
 *  @brief  Free bytes into the ring buffer for consumtion as data has now been placed at the head
 */
size_t RingBuf_free_put_contiguous(RingBuf_Handle object, size_t n);
/*!
 *  @brief  Peek at the ring buffer and get a pointer to the tail plus the size of bytes available
 *  that are continuously used.
 */
size_t RingBuf_peek_contiguous(RingBuf_Handle object, unsigned char **data);
/*!
 *  @brief  Put an entire buffer of size into the ring buffer
 */
size_t RingBuf_put_buffer(RingBuf_Handle object, const unsigned char *data, size_t cnt);
/*!
 *  @brief  Consume the RingBuffer (Get) by size, given consumption by bulk reader with direct memory access
 */
size_t RingBuf_get_dma(RingBuf_Handle object, size_t n);
/*!
 *  @brief  Fill the RingBuffer (put) by size, given additions by a direct memory access function
 */
size_t RingBuf_put_dma(RingBuf_Handle object, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_uart_RingBuf__include */
