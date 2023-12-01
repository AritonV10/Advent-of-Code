#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>

int main(void)
{
    
    int8_t  u8FirstDigit;
    uint8_t u8Char;
    uint8_t u8LastDigit;
    
    uint32_t u32FD;
    uint32_t u32Sum;
    
    
    u8FirstDigit = 0;
    u8LastDigit  = 0;
    u32Sum       = 0;
    u32FD        = open("input.txt", O_RDONLY);
    
    for(; (read(u32FD, &u8Char, 1u) != 0) ;) {
        
        
        if(u8Char == '\n') {
            
            u32Sum += ( (10 * u8FirstDigit) + u8LastDigit );
            
            u8FirstDigit = 0;
            u8LastDigit  = 0;
            
            continue;
        }
        
        /* Check if digit */
        if((u8Char & 0xF0u) == 0x30u) {
            
            
            u8FirstDigit += (u8LastDigit == 0) * (u8Char & 0x0Fu);
            
            u8LastDigit = (u8Char & 0x0Fu);
            
        }
        
        
    }
 
    u32Sum += ( (10 * u8FirstDigit) + u8LastDigit );
    
    printf("%d", u32Sum);

    return(0);
}

