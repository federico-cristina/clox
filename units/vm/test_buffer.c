#include "clox/vm/buffer.h"

int main()
{
    CloxBuffer_t *b = cloxCreateBuffer(0);

    cloxBufferPush(b, 1);
    cloxBufferPush(b, 2);
    cloxBufferPush(b, 3);
    cloxBufferPush(b, 4);
    cloxBufferPush(b, 5);
    cloxBufferPush(b, 6);
    cloxBufferPush(b, 7);
    cloxBufferPush(b, 8);
    cloxBufferPush(b, 9);
    cloxBufferPush(b, 10);
    cloxBufferPush(b, 11);
    cloxBufferPush(b, 12);
    cloxBufferPush(b, 13);
    cloxBufferPush(b, 14);
    cloxBufferPush(b, 15);
    cloxBufferPush(b, 16);
    cloxBufferPush(b, 17);
    cloxBufferPush(b, 18);

    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
    cloxBufferPop(b);
}
