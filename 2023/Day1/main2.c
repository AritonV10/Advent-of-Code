#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#define Clamp(x, y) ( (x + 4) - ( ((x + 4) > (y)) * (x + 4 - (y))) )

typedef struct {
    
    char *str;
    char  val;
    
} tstToken;


void
vFind(uint8_t, uint8_t, uint8_t, uint8_t);

uint8_t 
u8Comp(char *, char *, uint8_t, uint8_t);


static tstToken Tokens[9] = {
    {"one", '1'},
    {"two", '2'},
    {"six", '6'},
    {"four", '4'},
    {"five", '5'},
    {"nine", '9'},
    {"three", '3'},
    {"seven", '7'},
    {"eight", '8'},
};

static uint8_t u8Buf[120];

int 
main(void)
{
    


    /* Part 2 */
    uint8_t u8Char;
    
    uint8_t u8Len;
    
    uint8_t u8FirstDigit;
    uint8_t u8LastDigit;
    
    uint32_t u32Sum;
    int fd;
    
    u8FirstDigit = 0;
    u8LastDigit  = 0;
    u32Sum       = 0;
    fd           = open("input.txt", O_RDONLY);
    
    for(; (read(fd, &u8Char, 1)) != 0; ){
        
        u8Buf[u8Len++] = u8Char;
            
        if(u8Char == '\n') {
            
            u8Buf[u8Len - 1] = 0;
            
    
            uint8_t i;
            uint8_t u8Upper;
            uint8_t u8Lower;
            uint8_t u8Diff;
            
            for(i = 0; i < u8Len - 2; ++i) {
                
                u8Upper = Clamp(i, u8Len);
                u8Lower = i;
                
                u8Diff = u8Upper - u8Lower;
                
                /* Length 5 */
                if((u8Diff + 1) >= 5) 
                    vFind(6, 4, u8Lower, u8Upper);

                
                /* Length 4 */
                if((u8Diff + 1) >= 4)
                    vFind(3, 3, u8Lower, u8Upper);

                
                /* Length 3 */
                if((u8Diff + 1) >= 3) 
                    vFind(0, 2, u8Lower, u8Upper);

                
            }
            
            
            printf("%s\n", u8Buf);
            for(int i = 0; i < u8Len; ++i) {
            
                if((u8Buf[i] & 0xF0) == 0x30) {
                    
                    if((u8Buf[i] & 0x0F) != 0) {
                        u8FirstDigit += (u8LastDigit == 0) * (u8Buf[i] & 0x0Fu);
                        u8LastDigit = (u8Buf[i] & 0x0Fu);
                    }
                
                }
                
            }
            
            printf("%d %d\n", u8FirstDigit, u8LastDigit);
            u32Sum += ((u8FirstDigit * 10) + u8LastDigit);
            
            u8FirstDigit = 0;
            u8LastDigit = 0;
            u8Len = 0;
                
        }
            
    }
    
    printf("%d\n", u32Sum);

    return(0);
}


void
vFind(uint8_t u8ArrOffset, uint8_t u8Lim, uint8_t u8Lower, uint8_t u8Upper) {
    
    uint8_t u8K;
    uint8_t u8J; 
    
    for(u8K = 0; u8K < 3u; ++u8K) {
                        
                        
        char *str = Tokens[u8K + u8ArrOffset].str;
                    
        for(u8J = u8Lower; (u8J + u8Lim) <= u8Upper; ++u8J) {
                            
            if(u8Comp((char *)&u8Buf, str, u8J, u8J + u8Lim)) {
                                
                u8Buf[u8J + 1] = Tokens[u8K + u8ArrOffset].val;
                                
                break;
                              
            }
                            
        }
    }
    
}

uint8_t 
u8Comp(char *a, char *b, uint8_t u8Lower, uint8_t u8Upper) {
    
    uint8_t i;
    for(i = u8Lower; i <= u8Upper; ++i) {
        
        if(*(a + i) != *b++)
            return (0);
    }
    
    return (1u);
}
