/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>

static uint32_t
MAIN__u32Stoi(const char *);


int
main(void)
{
    
    uint8_t au8Buf[16];
    uint8_t u8Idx;
    uint8_t u8Stat;
    
    uint16_t u16Char;
    uint16_t u16LastChar;
    
    uint32_t u32FD;
    uint32_t u32Sum;
    uint32_t u32First;
    uint32_t u32Second;
    uint32_t u32Third;
    
    
    u8Idx       = 0;
    u16LastChar = 0;
    
    u32Sum      = 0;
    u32First    = 0;
    u32Second   = 0;
    u32Third    = 0;
    
    
    u32FD       = open("Data.txt", O_RDONLY);
    
    
    for(;;) {
        
        u8Stat = read(u32FD, &u16Char, 1u);
        
        u16Char &= 0xFFu;
        
        /* New elf */
        if(((u16LastChar  == 0x0Au) && (u16Char  == 0x0Au)) || u8Stat == 0) {
            
            if(u32Sum >= u32First) {
                u32Third  = u32Second;
                u32Second = u32First;
                u32First  = u32Sum;
            } else if(u32Sum >= u32Second) {
                u32Third  = u32Second;
                u32Second = u32Sum;
            } else if (u32Sum >= u32Third) {
                u32Third = u32Sum;
            }
           
            u32Sum = 0;
            
            if(u8Stat == 0)
                break;
            
            
        } else {
            
            if(u16Char == 0x0Au) {
                au8Buf[u8Idx] = 0;
                
                u32Sum += MAIN__u32Stoi((const char *)&au8Buf);
                
                u8Idx = 0;
            } else {
                au8Buf[u8Idx++] = u16Char;
            }
            
        }
        
        /* Add to the sum */
        u16LastChar = u16Char;
        
    }
    
    printf("%d\n", MAIN__u32Stoi("143215"));
    printf("%u\n", u32First + u32Second + u32Third);
    
   
    return 0;
}

static uint32_t
MAIN__u32Stoi(const char *pcNumber) {
    
    uint32_t u32Number;
    
    u32Number = 0;
    
    
    for(; *pcNumber != 0; ++pcNumber) {
        
        u32Number = (u32Number * 10u) + (*pcNumber & 0x0Fu);
    }
    
    
    return(u32Number);
    
}
