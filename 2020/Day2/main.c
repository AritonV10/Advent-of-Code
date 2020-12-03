#include <stdio.h>
#include <unistd.h>
#include "..\Utils\string\include\string.h"

#define MAIN__Compare(a, b, s) (*(a + (b - 1)) s acInteger[0])

static uint32_t
MAIN__u32Stoi(const char *pcInteger) {
    uint32_t u32Integer;

    for(u32Integer = 0; *pcInteger != 0; ++pcInteger)
        u32Integer = ((u32Integer * 10) + (*pcInteger & 0x0F));

    return(u32Integer);
}

static MAIN__Day2Part1(STRING_tstString **pstString, uint32_t u32Size) {
    uint32_t u32Index;
    
    uint32_t u32Occurance;
    uint32_t u32Result;
    uint32_t u32ResultPart2;
    uint32_t au32Range[2];

    uint8_t  u8Part2Index;
    uint8_t  u8RangeIndex;
    uint8_t  u8CharIndex;
    uint8_t  u8Flag;
    char     acInteger[10];
    char     *pcHead;

    u32Result      = 0;
    u32ResultPart2 = 0;

    for(u32Index = 0, u8CharIndex = 0, u8RangeIndex = 0; u32Index < u32Size; ++u32Index) {
        pcHead = pstString[u32Index]->pchBuffer;

        for(u8CharIndex = 0, u8RangeIndex = 0; *pcHead != ':'; ++pcHead) {
            if((*pcHead >> 4) == 0x03) {
                acInteger[u8CharIndex++] = *pcHead;
                u8Flag = 1;
            } else {

                if(u8Flag == 1) {

                    acInteger[u8CharIndex]    = 0;

                    au32Range[u8RangeIndex++] = MAIN__u32Stoi((const char *)&acInteger);
                    u8CharIndex = 0;
                    u8Flag       = 0;

                }
            }
        }

        /* get past the : */
        acInteger[0] = *(pcHead - 1);
        
        pcHead += 2;
        printf("%s\n", pcHead);
        /* x == c && y != c, x != c && y == c */
        /* x == c && y == c, x != c && y != c */
        u32ResultPart2 += (( MAIN__Compare(pcHead, au32Range[0], ==) && MAIN__Compare(pcHead, au32Range[1], !=)) 
        || MAIN__Compare(pcHead, au32Range[1], ==) && MAIN__Compare(pcHead, au32Range[0], !=));

        for(u32Occurance = 0; *pcHead != 0; ++pcHead) {
            u32Occurance += (*pcHead == acInteger[0]);
            
        }

        u32Result += ((u32Occurance >= au32Range[0]) && (u32Occurance <= au32Range[1]));

       
         
    }
    printf("Part 1: %d\nPart 2: %d", u32Result, u32ResultPart2);
}



int main(void) {
    
    uint32_t           u32Strings;
    uint32_t           u32Index;
    STRING_tstString **pstString;



    pstString = STRING_astLoadStrings("C:\\Users\\vio\\Desktop\\VS_C\\viorel.txt", NEW_LINE, &u32Strings);
    
    MAIN__Day2Part1(pstString, u32Strings);
    
    return(0);
}
