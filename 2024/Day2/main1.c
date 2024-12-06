#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(void) {
    
    int32_t  safe;
    uint8_t  nl;
    int8_t  sign;
    int32_t x1;
    int32_t x2;
    
    FILE *file;


    safe = 0;
    x1   = -1;
    x2   = -1;
    sign = 0;
    
    file = fopen("data.txt", "r");
    
    /* Get the first 2 to determine the sign */
    for(; (fscanf(file, "%d%c", &x1, &nl)) != EOF; ) {
        
        if(x2 < 0) {
            x2 = x1;
            continue;
        }
        
        sign = (x1 - x2) >> sizeof(int32_t);
        
        
        /* Read the rest */
        for(; ; fscanf(file, "%d%c", &x1, &nl)){
            /* Skip the next line */
            if(x1 - x2 == 0 || abs(x1 - x2) > 3 || sign != ((x1 - x2) >> sizeof(int32_t))) {
                --safe;
                printf("[%d %d] Are not good\n", x1, x2);
                for(; nl != '\n'; fscanf(file, "%d%c", &x1, &nl))
                    ;
                break;
            }
            
            x2 = x1;
            
            if(nl == '\n')
                break;
        }
         
        ++safe;
        x2 = -1;
    }
        
    fclose(file);

    printf("%d", safe);
    return 0;
}
