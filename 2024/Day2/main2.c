#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(void) {
    
    int32_t  safe;
    int32_t x1;
    int32_t x2;
    int32_t x3;
    
    
    uint8_t  nl;
    uint8_t  removed;
    int8_t  sign;
    
    FILE *file;

    removed = 0;
    safe    = 0;
    x1      = -1;
    x2      = -1;
    x3      = -1;
    sign    = 0;
    
    file = fopen("data.txt", "r");
    
    /* Get the first 2 to determine the sign */
    for(; (fscanf(file, "%d%c", &x1, &nl)) != EOF ;) {
        
        if(x2 < 0) {
            x2 = x1;
            continue;
        }
        
        sign = (x1 - x2) >> sizeof(int32_t);
        
        
        /* Read the rest */
        for(; ; (void)fscanf(file, "%d%c", &x1, &nl)){
            
            
            if(x1 == x2 || abs(x1 - x2) > 3 || sign != ((x1 - x2) >> sizeof(int32_t))) {
                
                /* Must be last element, so we can remove it */
                if(nl == '\n' && removed == 0)
                    break;
                
                /* First 2 aren't good, so we must do another read */
                if(x3 < 0) {
                    
                    x3 = x2;
                    
                    x2 = x1;
                    
                    (void)fscanf(file, "%d%c", &x1, &nl);
                }
                
                /* Can we remove the digit? */
                if( x1 != x3 && abs(x1 - x3) <= 3 && removed == 0) {
                    
                    
                    printf("[%d %d %d] Safe1\n", x3, x2, x1);
                    
                    removed = 1;
                    sign = ((x1 - x3) >> sizeof(int32_t));
                    x3 = x2;
                    x2 = x1;
                    
                    continue;
                    
                    
                }
                    
                --safe;
                printf("[%d %d] Are not good\n", x1, x2);
                
                for(; nl != '\n'; (void)fscanf(file, "%d%c", &x1, &nl))
                    ;
                    
            }
            
            
            x3 = x2;
            
            x2 = x1;
            
            
            if(nl == '\n')
                break;
        }
        
        removed = 0;
        
        ++safe;
        
        x2 = -1;
        x3 = -1;
    }
        
    fclose(file);

    printf("%d", safe);
    return 0;
}
