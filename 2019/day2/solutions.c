#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define NREGISTERS      (0x4)
#define INTEGER_TO_FIND (19690720)
#define OPERATION(a, op) a[a[i + 3]] = a[a[i + 1]] op a[a[i + 2]]
                    

typedef enum {
    
    ADD  = 0x1,
    MULT = 0x2,
    END  = 0x63,

} opcode_e;

typedef int FD;

int32_t
get_num_instructions(FD *);

void
get_instr(FD *, int32_t *);

int32_t
stoi(const char *, uint8_t);


void
exec(int32_t *, int32_t *);

void
copy_integers(int32_t *, int32_t *, int32_t *);


int main(void) {
    
    
    FD       fd;
    int32_t  len;
    int32_t *integers;
    
    fd = open("instr.txt", 0x0000);
    
    if(fd < 0)
        exit(1);
    
    
    len = get_num_instructions(&fd);
    
    integers = malloc(sizeof(int32_t) * len);
    
    if(integers == NULL)
        exit(1);
    
    get_instr(&fd, integers);
    exec(integers, &len);    

    return(0);
}


int32_t
get_num_instructions(FD *fd) {

    char    ch;
    int32_t n;
    int32_t chread;

    n      = 0;
    chread = 0;

    for(; (read(*fd, &ch, 1)) > 0; ++chread)
        n += (ch == ',');

    lseek(*fd, -chread, SEEK_END);

    return(n + 1);
}

void
get_instr(FD *fd, int32_t *integers) {
    
    
    char    ch;
    char    buf[1024];
    
    uint8_t index;
    int32_t integer_idx;
    
    index       = 0;
    integer_idx = 0;
    
    for(; (read(*fd, &ch, 1)) > 0; ) {
        
        if(ch == ',' || ch == ' ') {
            
            buf[index] = 0;
            
            integers[integer_idx++] = stoi(&buf[0], 0);
            
            if(ch == 0)
                break;
                
            index = 0;
            
        } else {
            
            buf[index++] = ch;
        
            
        }
        
    }
    
}

int32_t
stoi(const char *num, uint8_t delimiter) {
    
    
    int32_t ret_value;
    
    
    ret_value = 0;
    
    for(; *num != delimiter; ++num)
        ret_value = (ret_value * 10) + ((int32_t) *num & 0x0F);
        
    return(ret_value);
}

void
exec(int32_t *integers, int32_t *len) {
    
    int32_t i;
    int32_t first_value;
    int32_t second_value;
    
    first_value  = 0;
    second_value = 0;
    
    int32_t *integers_copy;
    
    integers_copy = malloc(sizeof(int32_t) * (*len));
    
    if(integers_copy == NULL)
        exit(1);
    
    
    for(;;) {
        
        copy_integers(integers, integers_copy, len);
        
        integers_copy[1] = first_value;
        integers_copy[2] = second_value;
         
        for(i = 0; i < *len; i += 4) {
        
            switch(integers_copy[i]) {
            
                case ADD:
                    OPERATION(integers_copy, +);
                    break;
                case MULT:
                    OPERATION(integers_copy, *);
                    break;
                case END:
                    goto end; 
            
            }
    
        }
        
        end:
            if(integers_copy[0] == INTEGER_TO_FIND) {
                printf("%d", 100 * first_value + second_value);
                return;
            }
            
            first_value = (first_value + 1 % 99);
            
            second_value += !first_value;
    }
    
}

void
copy_integers(int32_t *src, int32_t *dest, int32_t *len) {
    
    int32_t i;
    
    for(i = 0; i < *len; ++i)
        dest[i] = src[i];
        
}
