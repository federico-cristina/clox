#include "clox/vm/debug.h"
#include "clox/vm/code.h"

int main()
{
    byte_t program[] = {
        CLOX_OP_CODE_NOP,
    };

    CloxCodeBlock_t block;

    cloxInitCodeBlock(&block, 0);
    cloxCodeBlockWrite(&block, (const byte_t *)program, countof(program));
    cloxDisassembleCodeBlock(stdout, &block);
}
