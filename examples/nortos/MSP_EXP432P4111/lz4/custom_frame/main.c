//******************************************************************************
//  lz4_ex2_custom_frame - Compress a block of text with CRC32 checksum.
//
//  This example demonstrates how to compress a block of text into a custom
//  frame with CRC32 checksum. The advantage is a significantly faster checksum
//  calculation using the hardware CRC module compared to the software xxHash
//  implementation.
//
//    +-------------------+-----------------+-----------------------+
//    | 32-bit block size | compressed data | 32-bit CRC32 checksum |
//    +-------------------+-----------------+-----------------------+
//
//  Brent Peterson
//  Texas Instruments Inc.
//  January 2016
//
//  Modified to run on the MSP432P4XX
//******************************************************************************

/* MSP432 Includes */
#include <ti/devices/msp432p4xx/inc/msp432.h>

/* Driverlib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* LZ4 Includes */
#include <ti/compression/lz4/lz4.h>

const char loremIpsum[] = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi eget dui at turp\
is blandit condimentum. Sed a nisl turpis. Sed iaculis urna id blandit tristiqu\
e. Ut sagittis sapien sit amet justo pretium tincidunt. Nulla ac hendrerit libe\
ro. Aenean in eros velit. Nulla vel nibh elit. Curabitur mollis quis dolor eget\
 vehicula. Curabitur vitae turpis viverra, mollis mauris tempor, vehicula odio.\
Vivamus feugiat justo dui, id auctor nunc faucibus in. In at tellus id elit sce\
lerisque ornare. Donec at ultricies felis. Sed sed ultrices nibh, sit amet mole\
stie justo. Suspendisse nec eleifend risus. Sed consectetur ullamcorper nisi in\
 tempor. Sed accumsan tortor et enim consectetur, ac vulputate tortor fringilla\
. Aliquam at condimentum massa. Suspendisse eget ante venenatis, facilisis sem \
eu, tempor ligula. Praesent orci diam, aliquam nec velit in, malesuada porttito\
r orci. Sed nibh nulla, molestie id congue vitae, elementum et nisi. Duis matti\
s vel enim vitae vestibulum. Curabitur posuere augue sit amet lacus aliquam, la\
cinia commodo mauris dignissim. Curabitur id posuere nisi. Phasellus bibendum, \
mauris quis maximus mollis, urna nisi pharetra orci, eget pulvinar arcu enim eg\
et mi.\
Fusce sed nibh hendrerit, finibus nunc vestibulum, sagittis purus. Maecenas ut \
tellus lacinia, sodales nisl sit amet, scelerisque erat. Ut et iaculis nulla, s\
it amet lacinia sem. Mauris ut eros scelerisque, sodales quam ac, vestibulum nu\
lla. Curabitur tristique molestie pretium. Pellentesque et ornare est, at ultri\
cies dolor. Ut eu est erat. Aenean vel massa placerat, rutrum ipsum non, tincid\
unt purus. Curabitur gravida gravida pellentesque. In imperdiet, tellus ut inte\
rdum aliquam, orci nisi commodo mauris, vitae dignissim ipsum neque ac nisl. Cr\
as vitae finibus justo. Ut quis congue augue. Nulla ac porta dolor. Proin moles\
tie maximus lacus at mattis. Proin sodales at nibh sit amet lacinia.\
Praesent porta justo libero, faucibus ornare dui aliquam in. Vivamus maximus ma\
ssa quis lacus consequat mattis. Maecenas et luctus sapien. Duis rutrum nisl au\
gue, non finibus ligula bibendum sit amet. Cum sociis natoque penatibus et magn\
is dis parturient montes, nascetur ridiculus mus. Aenean eros libero, sagittis \
bibendum massa eu, convallis iaculis nulla. Integer sit amet ultrices lacus, eg\
estas dapibus felis. Cras iaculis vel nisi a iaculis. In rhoncus nunc ac mauris\
 posuere, et lobortis lacus ultricies. Nulla hendrerit pellentesque sapien vel \
mollis. Praesent congue odio sed pulvinar pharetra. Phasellus vehicula malesuad\
a ligula, consequat iaculis nulla porta vitae. Nulla quam mauris, laoreet nec u\
ltricies efficitur, ullamcorper et est. In congue metus ante, ut sagittis metus\
 ultricies sodales. In fermentum congue accumsan.\
Proin sagittis id eros semper venenatis. Nunc eget metus euismod, ornare nunc e\
get, pellentesque lacus. Etiam efficitur, sem eget pulvinar maximus, magna elit\
 eleifend urna, in commodo risus massa sed tellus. Aenean in dui ante. Phasellu\
s a vehicula velit. Maecenas eget laoreet est, suscipit tristique odio. Nulla i\
d sapien et felis venenatis imperdiet. Phasellus rutrum nisl at leo auctor vulp\
utate. Ut quam urna, facilisis in lectus sed, interdum convallis est. Integer f\
aucibus nulla neque, nec auctor eros fringilla at. Nulla elementum lorem nibh.";

#define DATA_SIZE       (sizeof(loremIpsum))

#define HASHSIZE        4096
#define HASHLOG2SIZE    12

uint8_t hashMemory[HASHSIZE] = {0};
uint8_t compressedData[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};
uint8_t decompressedData[DATA_SIZE] = {0};

volatile uint32_t compressedSize;
volatile uint32_t decompressedSize;

uint32_t msp_compress(const void *src, void *dst, uint32_t length);
uint32_t msp_decompress(const void *src, void *dst, uint32_t length);

int main(void)
{
    // Compress text to custom frame using CRC32 for checksum
    compressedSize = msp_compress((const void *)loremIpsum, (void *)compressedData, sizeof(loremIpsum));

    // Verify that the data was successfully compressed.
    if (!compressedSize) {
        while(1);
    }

    // Decompress data to original text.
    decompressedSize = msp_decompress((const void *)compressedData, (void *)decompressedData, sizeof(decompressedData));

    // Verify that the data was successfully decompressed.
    if (!decompressedSize) {
        while(1);
    }

    // Check the the decompressed data matches the original file.
    if (memcmp((void *)loremIpsum, (void *)decompressedData, sizeof(loremIpsum))) {
        while(1);
    }

    // Compression/decompression is successful if execution finishes
    return 0;
}

uint32_t msp_compress(const void *src, void *dst, uint32_t length)
{
    uint8_t *crcPtr;
    uint8_t *crcEnd;
    uint32_t blockLength;
    LZ4_status status;
    LZ4_compressBlockParams params;

    // Initialize compression parameters and compress data block
    params.src = src;
    params.dst = dst;
    params.length = length;
    params.hashTable = (void *)hashMemory;
    params.hashLog2Size = HASHLOG2SIZE;
    params.addBlockChecksum = false;
    LZ4_compressBlock(&params, &status);
    blockLength = *(uint32_t *)dst;

    // Check status for success, if not return length of zero.
    if (status != LZ4_SUCCESS) {
        return 0;
    }

     // Calculate CRC32 with seed of 0xFFFFFFFF
    crcPtr = (uint8_t *)dst + sizeof(uint32_t);
    crcEnd = crcPtr + blockLength;
    MAP_CRC32_setSeed(0xFFFFFFFF, CRC32_MODE);
    while (crcPtr < crcEnd) {
        MAP_CRC32_set8BitDataReversed(*crcPtr++, CRC32_MODE);
    }

    // Append CRC32 checksum to the end of the data block
    *(uint32_t *)crcPtr = MAP_CRC32_getResultReversed(CRC32_MODE);

    // Return total frame length (block length + 32-bit length + 32-bit CRC).
    return blockLength+sizeof(uint32_t)+sizeof(uint32_t);
}

uint32_t msp_decompress(const void *src, void *dst, uint32_t length)
{
    uint32_t crcValue;
    const uint8_t *crcPtr;
    const uint8_t *crcEnd;
    uint32_t blockLength;
    LZ4_status status;
    LZ4_decompressBlockParams params;

    // Verify CRC32 with seed of 0xFFFFFFFF
    blockLength = *(const uint32_t *)src;
    crcPtr = (const uint8_t *)src + sizeof(uint32_t);
    crcEnd = crcPtr + blockLength;
    MAP_CRC32_setSeed(0xFFFFFFFF, CRC32_MODE);
    while (crcPtr < crcEnd) {
        MAP_CRC32_set8BitDataReversed(*crcPtr++, CRC32_MODE);
    }

    // Read CRC32 and check for match
    crcValue = *(uint32_t *)crcPtr;
    if (crcValue != MAP_CRC32_getResultReversed(CRC32_MODE)) {
        // CRC32 mismatch, return length of zero.
        return 0;
    }

    // Initialize decompression parameters and decompress data block
    params.src = src;
    params.dst = dst;
    params.dstLength = length;
    params.verifyBlockChecksum = false;
    length = LZ4_decompressBlock(&params, &status);

    // Check status for success, if not return length of zero.
    if (status != LZ4_SUCCESS) {
        return 0;
    }

    // Return decompressed length.
    return length;
}
