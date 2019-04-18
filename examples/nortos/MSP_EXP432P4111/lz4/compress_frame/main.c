//******************************************************************************
//  lz4_ex1_compress_frame - Compress a block of text to a valid LZ4 file.
//
//  This example demonstrates how to compress a block of text into a valid LZ4
//  file with framing. The compressed data can be saved from memory as a raw
//  binary file and recovered on the PC using the LZ4 command line utility.
//
//  This example is configured with a hash size of 4096 and includes both the
//  content size and a content checksum to verify the original file contents.
//  With these settings the LZ4 compression achieves a 1.51 compression ratio
//  (3283 bytes compressed to 2169 bytes).
//
//  Brent Peterson
//  Texas Instruments Inc.
//  January 2016
//
//  Modified to run on the MSP432P4XX
//******************************************************************************

/* MSP432 Includes */
#include <ti/devices/msp432p4xx/inc/msp432.h>

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

volatile uint32_t contentSize;
volatile uint32_t compressedSize;
volatile uint32_t decompressedSize;

int main(void)
{
    LZ4_status status;
    LZ4_compressParams compressParams;
    LZ4_decompressParams decompressParams;

    // Initialize compression parameters and compress data block
    compressParams.src = (const void *)loremIpsum;
    compressParams.dst = (void *)compressedData;
    compressParams.length = DATA_SIZE;
    compressParams.hashTable = (void *)hashMemory;
    compressParams.hashLog2Size = HASHLOG2SIZE;
    compressParams.addBlockChecksum = false;
    compressParams.addContentChecksum = true;
    compressParams.addContentSize = true;
    compressedSize = LZ4_compress(&compressParams, &status);

    // Read and verify the content size matches the original file size.
    contentSize = LZ4_getContentSize(compressedData, &status);
    if (contentSize != DATA_SIZE) {
        while(1);
    }

    // Initialize decompression parameters and decompress data block
    decompressParams.src = (void *)compressedData;
    decompressParams.dst = (void *)decompressedData;
    decompressParams.dstLength = sizeof(decompressedData);
    decompressParams.verifyBlockChecksum = true;
    decompressParams.verifyContentChecksum = true;
    decompressedSize = LZ4_decompress(&decompressParams, &status);

    // Verify that the decompressed size is identical to the original file size.
    if (decompressedSize != DATA_SIZE) {
        while(1);
    }

    // Check the the decompressed data matches the original file.
    if (memcmp((void *)loremIpsum, (void *)decompressedData, DATA_SIZE)) {
        while(1);
    }

    // Compression/decompression is successful is execution finishes
    return 0;
}
