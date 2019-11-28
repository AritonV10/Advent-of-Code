#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int FD;

uint8_t pad[3][3];

void
follow_instr(char **, int32_t *);

int32_t
get_numberof_instructions(FD *);

void    
get_instructions(FD *, char **);

int main(void){
    
    uint8_t i;
    uint8_t j;
    uint8_t counter;

    int32_t ninstr;
    
    FD      fd;
    
    char **instr;
    
    counter = 1;
    
    for(i = 0; i < 3; ++i) {
        for(j = 0; j < 3; ++j) {
            pad[i][j] = counter++;
        }
    }
    
    fd = open("inst.txt", (const char *)0x0000);
    
    ninstr = get_numberof_instructions(&fd);
    
    instr  = malloc(sizeof(char*) * ninstr);
    
    --ninstr;
    
    get_instructions(&fd, instr);
    follow_instr(instr, &ninstr);
    
    if(instr == NULL)
        fprintf(stderr, "ERR: Could not allocate mem for the instructions");
    
    /*follow_instr(instr, &ninstr); */
     printf("%s", instr[1]);
    return 0;
}

void
follow_instr(char **inst, int32_t *ninst) {
    
    int32_t i;
    int32_t direction_x;
    int32_t direction_y;
    
    direction_x = 1;
    direction_y = 1;
    
    for(i = 0; i < *ninst; ++i) {
        
        printf("Executing: %s ", inst[i]);
        
        for(; *(inst[i]) != 0; (inst[i])++) {
            switch(*(inst[i])) {
                case 'U':
                    direction_y -= (direction_y != 0);
                    break;
                case 'D':
                    direction_y += (direction_y != 2);
                    break;
                case 'L':
                    direction_x -= (direction_x != 0);
                    break;
                case 'R':
                    direction_x += (direction_x != 2);
                    break;
            }
        }
        
        printf("%d\n", pad[direction_y][direction_x]);
    }
    
}

int32_t
get_numberof_instructions(FD *fd) {
    
    char    ch;
    int32_t n;
    int32_t chread;
    
    n      = 0;
    chread = 0;
    
    for(; (read(*fd, &ch, 1)) > 0; ++chread) {
        
        if(ch == 0xA)
            ++n;
    }
    
    
    lseek(*fd, -chread, SEEK_END);
    
    return(n + 1);
}

void    
get_instructions(FD *fd, char **instr) {
    
    char    ch;
    char    buf[31];
    int32_t chread;
    
    int32_t i;
    int32_t j;
    
    char    *ptr;
    
    for(chread = 0; (read(*fd, &ch, 1)) > 0;) {
        
        if(ch == 0xA) {
            
            *instr = malloc(sizeof(char) * (chread));
            
            if(*instr == NULL)
                fprintf(stderr, "ERR: Could not allocate memory for instruction");
            
            ptr = *instr;
            
            for(j = 0; j < chread; ++j, ++ptr)
                *ptr = buf[j];
            
            
            *ptr = 0;

            ++instr;
            chread = 0;
            
        } else {
            
            buf[chread++] = ch;
        
        }
    }
    
}




