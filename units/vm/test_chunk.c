#include "clox/vm/chunk.h"

#include <stdio.h>

int main()
{
    CloxChunk_t *b = cloxCreateChunk(0);

    byte_t a[] = {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20
    };

    cloxChunkWrite(b, a, countof(a));

    cloxChunkPush(b, 1);
    cloxChunkPush(b, 2);
    cloxChunkPush(b, 3);
    cloxChunkPush(b, 4);
    cloxChunkPush(b, 5);
    cloxChunkPush(b, 6);
    cloxChunkPush(b, 7);
    cloxChunkPush(b, 8);
    cloxChunkPush(b, 9);
    cloxChunkPush(b, 10);
    cloxChunkPush(b, 11);
    cloxChunkPush(b, 12);
    cloxChunkPush(b, 13);
    cloxChunkPush(b, 14);
    cloxChunkPush(b, 15);
    cloxChunkPush(b, 16);
    cloxChunkPush(b, 17);
    cloxChunkPush(b, 18);

    CloxChunkReader_t *r = cloxCreateChunkReader(b);

    while (!cloxChunkReaderIsAtEnd(r))
        printf("%02X ", cloxChunkReaderGet(r));

    cloxDeleteChunkReader(r, FALSE);
    cloxDeleteChunk(b);
}
