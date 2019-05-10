/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/utils/RingBuf.h>
#include <string.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

/*
 *  ======== RingBuf_construct ========
 */
void RingBuf_construct(RingBuf_Handle object, unsigned char *bufPtr,
                       size_t bufSize)
{
    memset(object, 0, sizeof(RingBuf_Object));
    object->buffer = bufPtr;
    object->length = bufSize;

    /* Set the buffer to a known value of 0xFE for debugging purposes */
    memset(bufPtr, 0xFE, bufSize);
}

/*
 *  ======== RingBuf_get ========
 */
int RingBuf_get(RingBuf_Handle object, unsigned char *data)
{
    unsigned int key;

    key = HwiP_disable();

    if (!object->count)
    {
        HwiP_restore(key);
        return -1;
    }

    *data = object->buffer[object->tail];
    object->tail = (object->tail + 1) % object->length;
    object->count--;

    HwiP_restore(key);

    return (object->count);
}

/*
 *  ======== RingBuf_getCount ========
 */
int RingBuf_getCount(RingBuf_Handle object)
{
    return (object->count);
}
int RingBuf_getAllocated(RingBuf_Handle object)
{
    return (object->allocated);
}
int RingBuf_getReserved(RingBuf_Handle object)
{
    return (object->reserved);
}

/*
 *  ======== RingBuf_isFull ========
 */
bool RingBuf_isFull(RingBuf_Handle object)
{
    return (object->count == object->length);
}

/*
 *  ======== RingBuf_getFree ========
 */
int RingBuf_getFree(RingBuf_Handle object)
{
    return (object->length - object->count);
}

/*
 *  ======== RingBuf_getUnallocated ========
 *
 * Get the number fo bytes not yet allocated to a DMA writer. This lets the RingBuf track the alloc
 * functions and
 */
size_t RingBuf_getUnallocated(RingBuf_Handle object)
{
    return (object->length - object->count - object->allocated);
}

/*
 *  ======== RingBuf_clear ========
 */
void RingBuf_clear(RingBuf_Handle object)
{
    object->count = 0;
    object->head = 0;
    object->tail = 0;
    object->allocated = 0;
    object->reserved = 0;
}

/*
 *  ======== RingBuf_peek ========
 */
int RingBuf_peek(RingBuf_Handle object, unsigned char *data)
{
    unsigned int key;
    int retCount;

    key = HwiP_disable();

    *data = object->buffer[object->tail];
    retCount = object->count;

    HwiP_restore(key);

    return (retCount);
}

/*
 *  ======== RingBuf_put ========
 */
int RingBuf_put(RingBuf_Handle object, unsigned char data)
{
    unsigned int key;

    key = HwiP_disable();

    if (object->count != object->length)
    {
        object->buffer[object->head] = data;
        object->head = (object->head + 1) % object->length;
        object->count++;
    }
    else
    {

        HwiP_restore(key);
        return (-1);
    }

    HwiP_restore(key);

    return (object->count);
}

/*
 * ======== RingBuf_put_dma ========
 *
 * Data has already been written into the buffer via a DMA
 * routine. This updates the ringBuf object with that
 * addition.
 *
 * The number of bytes exceeding the ring buffer capacity is
 * returned in case of an overflow of the data structure
 */
size_t RingBuf_put_dma(RingBuf_Handle object, size_t n)
{
    if ( (object->count + n) <= object->length) {
        /* Put into the buffer size n because we have space */
        object->head = (object->head + n )% object->length;
        object->count += n;

        return 0;
    } else {
        /* Buffer overflow situation here. The data is already in
         * the ring buffer and the old data is no longer valid
         */
        size_t overflow = n - (object->length - object->count);

        /* Head moved forward and the tail follows by the overflow
         * amount
         */
        object->head = (object->head + n )% object->length;
        object->tail = (object->tail + overflow) % object->length;
        /* We have a full buffer */
        object->count = object->length;

        /* Return the amount of overflowed bytes so
         * the caller can deal with how many bytes they just
         * lost to the buffer overflow
         */
        return overflow;
    }
}

/*
 * ======== RingBuf_get_dma ========
 *
 * Increment the RingBug handle variables to reflect data
 * consumed (ie Tx) via a DMA transfer process. Data has
 * consumed from the tail.
 *
 * Return the size number of bytes still held in the ring
 * buffer
 */
size_t RingBuf_get_dma(RingBuf_Handle object, size_t n)
{
    /* Quick sanity check.. We should never get into
     * a less than zero situation.. otherwise something
     * bad happened
     */
    if (((int)object->count - (int)n) < 0)
    {
        object->tail = object->head;
        object->count = 0;
    }
    else
    {
        object->tail = (object->tail + n) % object->length;
        object->count -= n;
    }

    return object->count;
}


/*
 *  ======== RingBuf_put_buffer ========
 *
 *  Write data into the ring buffer given a buffer and size
 */
size_t RingBuf_put_buffer(RingBuf_Handle object, const unsigned char *data, size_t cnt)
{
    const int f = (object->length - object->count);
    int key;
    int putCnt = cnt;

    if (f < cnt)
    {
        putCnt = cnt = f;
    }

    while (cnt)
    {
        int copySize = min(object->length - object->head, cnt);

        for (int i = 0; i < copySize; ++i, ++object->head, ++data)
            object->buffer[object->head] = *data;

        object->head = object->head % object->length;
        cnt -= copySize;

        /* Count is the only lock concern object here */
        key = HwiP_disable();

        object->count += copySize;
        HwiP_restore(key);
    }

    return putCnt;
}

/*
 *  ======== RingBuf_peek_contiguous ========
 * Called within a hardware interrupt context...
 */
size_t RingBuf_peek_contiguous(RingBuf_Handle object, unsigned char **data)
{
    int blockSize = min(object->count, object->length - object->tail);

    if (blockSize)
        *data = &object->buffer[object->tail];
    else
        *data = NULL;

    return blockSize;
}

/*
 *  ======== RingBuf_alloc_put_contiguous ========
 *
 * Allocate a contiguous block within the RingBuffer for an asynchronous writer, ie DMA (Rx)
 *
 * Request a size, and the pointer will be returned via **ringPointer and the actual allocation
 * size returned by the function
 *
 * The ringBuffer is then considered "put" after calling RingBuf_put_dma()
 *
 */
size_t RingBuf_alloc_put_contiguous(RingBuf_Handle object, size_t size, unsigned char **ringPointer)
{
    int blockSize = min(object->length - object->count - object->allocated,
                        object->length - ( (object->head + object->allocated) % object->length ));
    int allocSize = min(blockSize, size);

    if (allocSize) {
        *ringPointer = &object->buffer[ (object->head + object->allocated ) % object->length ];
        object->allocated += allocSize;
    } else {
        *ringPointer = NULL;
    }

    return allocSize;
}

/*
 * ======== RingBuf_free_put_contiguous ========
 *
 * Data has already been written into the buffer via a DMA
 * routine. This updates the ringBuf object with that
 * addition.
 *
 * The number of bytes exceeding the ring buffer capacity is
 * returned in case of an overflow of the data structure
 *
 * All calls to put_dma must have an origin from allocate_cont
 * otherwise the allocated number will not line up
 */
size_t RingBuf_free_put_contiguous(RingBuf_Handle object, size_t n)
{
    if ( (object->count + n) <= object->length) {
        /* Put into the buffer size n because we have space */
        object->head = (object->head + n )% object->length;
        object->count += n;
        object->allocated -= n;

        return 0;
    } else {
        /* Buffer overflow situation here. The data is already in
         * the ring buffer and the old data is no longer valid
         */
        size_t overflow = n - (object->length - object->count);

        /* Head moved forward and the tail follows by the overflow
         * amount
         */
        object->head = (object->head + n )% object->length;
        object->tail = (object->tail + overflow) % object->length;
        /* We have a full buffer */
        object->count = object->length;
        object->allocated -= n;

        /* Return the amount of overflowed bytes so
         * the caller can deal with how many bytes they just
         * lost to the buffer overflow
         */
        return overflow;
    }
}

/*
 *  ======== RingBuf_alloc_get_contiguous ========
 *
 * Reserve a contiguous block within the RingBuffer for an asynchronous reader, ie DMA (Tx)
 *
 * Request a size, and the pointer will be returned via **ringPointer and the actual allocation
 * size returned by the function
 *
 * The buffer is then released from RingBuf_get_dma() which allows more data to be written to the
 * ring buffer
 *
 */
size_t RingBuf_alloc_get_contiguous(RingBuf_Handle object, size_t size, unsigned char **ringPointer)
{
    /* Can't reserve more than is IN the ring buffer */
    int reserveSize = min(object->count - object->reserved, size);

    /* Check if the reservation wraps or is continuous, in which case get the 'to end of buffer' size */
    if ( ( (object->tail + object->reserved ) % object->length ) + reserveSize > object->length ) {
        reserveSize = object->length - ( (object->tail + object->reserved) % object->length );
    }

    if (reserveSize) {
        *ringPointer = &object->buffer[(object->tail + object->reserved) % object->length];
        object->reserved += reserveSize;
    } else {
        *ringPointer = NULL;
    }

    return reserveSize;
}

/*
 * ======== RingBuf_free_get_contiguous ========
 *
 * Increment the RingBug handle variables to reflect data
 * consumed (ie Tx) via a DMA transfer process. Data has
 * consumed from the tail.
 *
 * Return the size number of bytes still held in the ring
 * buffer and decrements the number of reserved bytes by
 * the number consumed
 */
size_t RingBuf_free_get_contiguous(RingBuf_Handle object, size_t n)
{
    /* Quick sanity check.. We should never get into
     * a less than zero situation.. otherwise something
     * bad happened
     */
    if (((int)object->count - (int)n) < 0)
    {
        object->tail = object->head;
        object->count = 0;
        object->reserved = 0;
    }
    else
    {
        object->tail = (object->tail + n) % object->length;
        object->count -= n;
        object->reserved -= n;
    }

    return object->count;
}
