//******************************************************************************
//  lz4_ex3_stream_decompress - Decompress data chunks using stream functions.
//
//  This example demonstrates how to decompress LZ4 data in chunks of compressed
//  data. The advantage of the stream decompress functions is less data usage
//  due to only needing to buffer small chunks, one at a time. The performance
//  is slightly worse than the non-streaming API's.
//
//  Brent Peterson
//  Texas Instruments Inc.
//  June 2016
//
//  Modified to run on the MSP432P4XX
//******************************************************************************

/* MSP423 Includes */
#include <ti/devices/msp432p4xx/inc/msp432.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* LZ4 Includes */
#include <ti/compression/lz4/lz4.h>
#include <ti/compression/lz4/lz4_stream.h>

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
#define CHUNK_SIZE      100

#define HASHSIZE        4096
#define HASHLOG2SIZE    12

uint8_t hashMemory[HASHSIZE] = {0};
uint8_t compressedData[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};
uint8_t decompressedData[DATA_SIZE] = {0};

volatile uint32_t compressedSize;
volatile uint32_t decompressedSize;

int main(void)
{
    uint8_t *streamPtr;
    uint16_t streamLength;
    LZ4_status status;
    LZ4_compressBlockParams compressParams;
    LZ4_streamDecompressBlockState streamDecompressState;
    LZ4_streamDecompressBlockParams streamDecompressParams;

    // Compress data using lz4
    compressParams.src = loremIpsum;
    compressParams.dst = compressedData;
    compressParams.length = DATA_SIZE;
    compressParams.hashTable = (void *)hashMemory;
    compressParams.hashLog2Size = HASHLOG2SIZE;
    compressParams.addBlockChecksum = false;
    compressedSize = LZ4_compressBlock(&compressParams, &status);

    // Initialize stream decompress API
    streamDecompressParams.dst = decompressedData;
    streamDecompressParams.dstLength = DATA_SIZE;
    streamDecompressParams.containsBlockSize = true;
    LZ4_streamDecompressBlockInit(&streamDecompressParams, &streamDecompressState, &status);

    // Initialize local pointers
    streamPtr = compressedData;
    streamLength = compressedSize;

    // Decompress CHUNK_SIZE blocks of data using streaming API
    while (streamLength > CHUNK_SIZE) {
        decompressedSize = LZ4_streamDecompressBlock(&streamDecompressState, streamPtr, CHUNK_SIZE, &status);
        streamPtr += CHUNK_SIZE;
        streamLength -= CHUNK_SIZE;

        // Check that the operation completed successfully
        if (status != LZ4_SUCCESS) {
            while(1);
        }
    }

    // Decompress remaining bytes
    decompressedSize = LZ4_streamDecompressBlock(&streamDecompressState, streamPtr, streamLength, &status);

    // Check that the operation completed successfully
    if (status != LZ4_SUCCESS) {
        while(1);
    }

    // Check the the decompressed data matches the original file
    if (memcmp((void *)loremIpsum, (void *)decompressedData, sizeof(loremIpsum))) {
        while(1);
    }

    // Compression/decompression is successful is execution finishes
    return 0;
}
