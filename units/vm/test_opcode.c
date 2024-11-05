#include "clox/vm/opcode.h"

#include <stdio.h>

int main()
{
    CloxOpCodeInfo_t opCodeInfo;

    if (cloxGetOpCodeInfo(77, &opCodeInfo))
        printf("opcode: %02Xh (%s)\n", opCodeInfo.code, opCodeInfo.name);
    else
        printf("uknown opcode: %02Xh\n", opCodeInfo.code);

    return 0;
}
