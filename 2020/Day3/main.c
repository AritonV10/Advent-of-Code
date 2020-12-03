#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "..\Utils\string\include\string.h"


static uint64_t
MAIN__Day3Part1(STRING_tstString **pstString, uint32_t u32Size, uint32_t u32Right, uint32_t u32Down) {
    
    uint32_t u32CurrentLine;
    uint32_t u32CurrentColumn;

    uint32_t u32EmptySpace;
    uint64_t u64Trees;

    uint8_t u8Index;

    u32CurrentLine   = u32Down;
    u32CurrentColumn = 1;
    u32EmptySpace    = 0;
    u64Trees         = 0;

    for(; u32CurrentLine < u32Size; ) {
        
        u32CurrentColumn += u32Right;
        u32CurrentColumn = (u32CurrentColumn & 31) + (u32CurrentColumn > 31);
        
        if(pstString[u32CurrentLine]->pchBuffer[u32CurrentColumn - 1] == '#') {

            ++u64Trees;

        } else {

            ++u32EmptySpace;
        }

         u32CurrentLine += u32Down;
    }

    return(u64Trees);

}

int main(void) {
    
    uint32_t           u32Strings;
    uint32_t           u32Index;
    STRING_tstString **pstString;


    pstString = STRING_astLoadStrings("C:\\Users\\vio\\Desktop\\VS_C\\viorel.txt", NEW_LINE, &u32Strings);
    
    
    printf("%lu\n", (MAIN__Day3Part1(pstString, u32Strings, 1, 1)));
    printf("%lu\n", (MAIN__Day3Part1(pstString, u32Strings, 3, 1)));
    printf("%lu\n", (MAIN__Day3Part1(pstString, u32Strings, 5, 1)));
    printf("%lu\n", (MAIN__Day3Part1(pstString, u32Strings, 7, 1)));
    printf("%lu\n", (MAIN__Day3Part1(pstString, u32Strings, 1, 2)));
    
    
    return(0);
}
