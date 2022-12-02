#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>


#define DIFF(x) ((x[2]) - (x[0]))
#define F(x) (-3 * (24 - (x)) + 6)

#define CLAMP(x) do { \
    if(x == 21)     \
        x = 24;    \
    else if(x == 25) \
        x = 22;     \
}while(0);

int
main(void)
{

    int8_t  ai8Buf[16u];
    int8_t  i8Diff;
    
    uint32_t u32Sum;
    uint32_t u32Sum2;
    
    uint32_t u32FD;
  
    u32Sum  = 0;
    u32Sum2 = 0;
    u32FD   = open("untitled", O_RDONLY);
    
    for(; (read(u32FD, &ai8Buf, 4u)) != 0 ;) {
              
        i8Diff = DIFF(ai8Buf);
        CLAMP(i8Diff);
        
        u32Sum += F(i8Diff) + ai8Buf[2] - 87;
        
        i8Diff = ((ai8Buf[0] - 65));
       
        switch(ai8Buf[2]) {
            case 'X':
            
                /* The round is lost for X - 1 so we map [65, 66, 67] -> [0, 1, 2] to get the letter position */
                u32Sum2 += ( ((i8Diff - 1) & 3) - ((i8Diff - 1) < 0) + 1);
                
                break;
            case 'Y':
                u32Sum2 += ((ai8Buf[0] - 65) + 4);
                break;
            case 'Z':
                /* The round is won for X + 1 so we map [65, 66, 67] -> [0, 1, 2] to get the letter position */
                u32Sum2 += ( ((i8Diff + 1) % 3) + 7); 
                break;
        }
       
    }
    
    printf("%d %d", u32Sum, u32Sum2);
    
    return 0;
}


