#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int
comp(const void *a, const void *b) {
    
    return *((uint32_t *)a) > *((uint32_t *)b);
}

int main()
{
    
    uint32_t len;
    uint32_t result;
    uint32_t list1[1000];
    uint32_t list2[1000];
    
    uint32_t idx;
    uint32_t jdx;
    uint32_t occurances;
    
    FILE *file;


    len    = 0;
    idx    = 0;
    jdx    = 0;
    result = 0;
    occurances = 0;
    
    file = fopen("data.txt", "r");
    
    for(; (fscanf(file, "%d %d", &list1[len], &list2[len])) != EOF; ++len)
        ;
        
    fclose(file);
      
    qsort(list1, len, sizeof(uint32_t), &comp);
    qsort(list2, len, sizeof(uint32_t), &comp);
    
    for(int i = 0; i < len; ++i)
        result += abs(list1[i] - list2[i]);
      
    printf("Result 1: %d\n", result);
    
    result = 0;
    
    for(; jdx < len; ) {
        
       
        if(idx > 0 && list1[idx] == list1[idx -1]) {
            result += (list1[idx] * occurances);
            ++idx;
            continue;
        }
        if(list2[jdx] > list1[idx]) {
            ++idx;
            continue;
        }
        
        for(;; ++jdx) {
            
            /* If bigger, no point in continuing */
            if(list2[jdx] > list1[idx])
                break;
            
            occurances = 0;
            if(list2[jdx] == list1[idx]) {
                
                for(; (list2[jdx] == list1[idx]) && jdx < len; ++jdx, ++occurances)
                    ;

                result += (list1[idx] * occurances);
                ++idx;
                --jdx;
                
            }
                
        }
        
    }
    
    printf("Result 2: %d", result);

    return 0;
}
