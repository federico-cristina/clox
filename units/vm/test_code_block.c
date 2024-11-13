#include "clox/vm/debug.h"
#include "clox/vm/opcode.h"

int main()
{
    byte_t program[] = {
        CLOX_OP_CODE_NOP,
        CLOX_OP_CODE_LDBYTE,
        54,
        CLOX_OP_CODE_LDBYTE,
        64,
    };

    CloxCodeBlock_t block;

    cloxInitCodeBlock(&block, 0);
    cloxCodeBlockWrite(&block, (const byte_t *)program, countof(program));
    cloxDisassembleCodeBlock(stdout, &block);
}
