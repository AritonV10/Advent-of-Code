#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define nRed    12
#define nGreen  13
#define nBlue   14

int main(void) {
  
    uint32_t u32FD;
    uint32_t u32Sum;
    
    uint8_t u8Buflen;
    
    uint8_t u8Idx;
    uint8_t u8ID;
    uint8_t u8Num;
    uint8_t u8Valid;
    
            
    
    char u8Buf[256];
    char u8Char;
    
    
    u32FD    = open("input.txt", 0);
    u32Sum   = 0;
    
    u8Buflen = 0;
    u8ID     = 0;
    
    for( ;(read(u32FD, &u8Char, 1u)) != 0; ) {

        if((u8Char == '\n') || u8Char == EOF) {

            u8Buf[u8Buflen] = 0;
         
            u8Num           = 0;
            u8Idx           = 0;
            u8Valid         = 1;
            
            /* Increment line ID */
            ++u8ID;
            
            /* Move over ID since we don't need it yet */
            for(; u8Buf[u8Idx] != ':'; ++u8Idx)
                ;
            ++u8Idx;
            
            for(; u8Buf[u8Idx] != 0 && u8Valid; ++u8Idx) {

                /* Get the number of cubes */
                if( (u8Buf[u8Idx] & 0xF0u) == 0x30u && (u8Buf[u8Idx]) != 0x3Bu)
                    u8Num = (u8Num * 10) + (u8Buf[u8Idx] & 0x0Fu);

                /* Get the color of the cube */
                switch(u8Buf[u8Idx]) {
                    case 'r':
                      
                        u8Valid = (u8Num <= nRed);
                        u8Num   = 0;
                        break;
                        
                    case 'g':
                       
                        u8Valid = (u8Num <= nGreen);
                        u8Num   = 0;
                        break;
                        
                    case 'b':
                     
                        u8Valid = (u8Num <= nBlue);
                        u8Num   = 0;
                        break;
                }
                
                
            }

            if(u8Valid) {
                
                printf("Game ID: [%d] is Valid\n", u8ID);
                u32Sum += u8ID;
                
            }

            u8Buflen = 0;
            
            continue;
            
        }
        
        u8Buf[u8Buflen++] = u8Char;

    }
    
    printf("%d", u32Sum);
    
    return 0;
}
