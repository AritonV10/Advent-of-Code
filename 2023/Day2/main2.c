#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#define nRed    12
#define nGreen  13
#define nBlue   14


int main(void)
{
    

    uint32_t u32FD;
    uint32_t u32Sum;
    
    /* Colors */
    uint16_t u16Red;
    uint16_t u16Green;
    uint16_t u16Blue;
    
    uint8_t u8Buflen;
    
    uint8_t u8Idx;
    uint8_t u8Num;
            
    char u8Buf[256];
    char u8Char;
    
    
    u32FD                 = open("input.txt", 0);
    u32Sum                = 0;
    u8Buflen              = 0;
    u16Red                = 0;
    u16Green              = 0;
    u16Blue               = 0;
    
    for( ;(read(u32FD, &u8Char, 1u)) != 0; ) {
        
        
        if((u8Char == '\n') || u8Char == EOF) {
            
            

            u8Buf[u8Buflen] = 0;
            u8Num           = 0;
            u8Idx           = 0;

            
            /* Move over ID since we don't need */
            for(; u8Buf[u8Idx] != ':'; ++u8Idx)
                ;
            ++u8Idx;
            
            for(; u8Buf[u8Idx] != 0; ++u8Idx) {
                
                if( (u8Buf[u8Idx] & 0xF0u) == 0x30u && (u8Buf[u8Idx]) != 0x3Bu) {
                    
                    u8Num = (u8Num * 10) + (u8Buf[u8Idx] & 0x0Fu);
                   
                }
                
                switch(u8Buf[u8Idx]) {
                    case 'r':
                      
                        if(u8Num > u16Red)
                            u16Red = u8Num;
                        u8Num   = 0;
                        break;
                        
                    case 'g':
                       
                        if(u8Num > u16Green)
                            u16Green = u8Num;
                        u8Num   = 0;
                        break;
                        
                    case 'b':
                     
                        if(u8Num > u16Blue)
                            u16Blue = u8Num;
                        u8Num   = 0;
                        break;
                }
                
                
            }
            
            u32Sum += u16Red * u16Green * u16Blue;
            
            u16Red   = 0;
            u16Green = 0;
            u16Blue  = 0;
           
            u8Buflen = 0;
            
            continue;
            
        }
        
        u8Buf[u8Buflen++] = u8Char;
        
        
    }
    
    printf("%d", u32Sum);
    
    
    return(0);
}
