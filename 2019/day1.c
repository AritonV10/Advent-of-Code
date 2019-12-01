#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>


#define IS_DIGIT(c) ((c) >= 0x30 && (c) <= 0x39)

typedef int FD;


int32_t  stoi                                 (const char *);
uint64_t compute_fuel_requirements            (FD *); 
uint64_t compute_fuel_requirements_second_star(FD *fd)


int main() {
    
    FD fd;
    
    fd = open("inst.txt", 0x0000);
    
    if(fd < 0)
        exit(1);
        
    
    
    printf("%lu", compute_fuel_requirements(&fd));

    close(fd);
    
    return(0);
}


int32_t
stoi(const char *number) {

    int32_t n;

    n = 0;

    for(; *number != 0; ++number)
        n = (n * 10) + (*number & 0x0F);

    return(n);

}

uint64_t 
compute_fuel_requirements(FD *fd) {
    
    char    ch;
    char    buf[31];
    
    int32_t  index;
    uint64_t fuel_requirements;
    
    fuel_requirements = 0;
    index             = 0;
    
    for(; (read(*fd, &ch, 1)) > 0; ) {
        
        if(ch == '\n' || ch == 0) {
            
            buf[index] = 0;
            
            fuel_requirements += ((uint32_t)(stoi(&buf[0]) / 3) - 2);
            
            index = 0;
            
            if(ch == 0)
                break;
            
        } else {
            buf[index++] = ch;
        }
        
    }
    
    
  return(fuel_requirements);
  
}

/* the second part of the exercise */
uint64_t 
compute_fuel_requirements_second_star(FD *fd) {
    
    char    ch;
    char    buf[31];
    int32_t fuel;
    
    int32_t  index;
    uint64_t fuel_requirements;
    
    fuel_requirements = 0;
    index             = 0;
    fuel              = 0;
    
    for(; (read(*fd, &ch, 1)) > 0; ) {
        
        if(ch == '\n' || ch == 0) {
            
            buf[index] = 0;
            
            fuel       = ((int32_t) stoi(&buf[0])/3) - 2;
            
            for(; fuel > 0; fuel = ((int32_t)fuel/3) - 2)
                fuel_requirements += fuel;
            
            index = 0;
            
            if(ch == 0)
                break;
            
        } else {
            buf[index++] = ch;
        }
        
    }
    
    
  return fuel_requirements;
  
}

