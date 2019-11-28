#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define GET_OPERAND(op) \
    for(; *(op) != ' '; ++op) \
            ;

#define NUM_REGISTERS 0x4
#define NUM_OPCODES   0x4

#define UPPER(c) (c & 0x4F)

typedef enum {
    MOV,
    INC,
    JNZ,
    DEC,
    INVALID_OPCODE
} opcode_e;

typedef struct {
    uint8_t bits[32];
} register32_t;

typedef struct {
    
    #define REGISTER_A   registers[0]
    #define REGISTER_B   registers[1]
    #define REGISTER_C   registers[2]
    #define REGISTER_D   registers[3]
    #define REGISTER(c)  registers[(c & 0x0F) - 1]
        
    register32_t registers[NUM_REGISTERS];
    
    const char   *IP;
    
} cpu_t;


typedef struct {
    
    opcode_e    e;
    
    const char  *opcode;
    
} opcode_t;

typedef int FD;



cpu_t        g_cpu;
opcode_t     g_opcodes[NUM_OPCODES];


void     pack32_signed             (register32_t *, const int32_t *);
int32_t  unpack32_signed           (const register32_t *);

opcode_e get_opcode                (const char *);

void     init_opcodes              (void);
char *   make_string               (const char *, const uint8_t);
int32_t  stoi                      (const char *, const uint8_t);


int32_t  get_numberof_instructions (FD *);
void     get_instructions          (FD *, char **);
void     exec_instructions         (char **, int32_t *);


int main(void){
    
    FD            fd;
    int32_t       i;
    int32_t       ninstr;
    char          **instr;
    
    init_opcodes();
    fd = open("inst.txt", (const char *)0x0000);
    
    
    ninstr = get_numberof_instructions(&fd);
    
    instr  = malloc(sizeof(char *) * ninstr);
    
    if(instr == NULL)
        fprintf(stderr, "ERR: Could not allocate memory for the instruction buffer");
    
    get_instructions(&fd, instr);
    
    --ninstr;
    exec_instructions(instr, &ninstr);
    
    for(i = 0; i < 32; ++i)
        printf("%d ", g_cpu.REGISTER_A.bits[i]);
    
    printf("\n%d", unpack32_signed(&g_cpu.REGISTER_A));
    
    close(fd);
    

    return 0;
}


int32_t
unpack32_signed(const register32_t *reg) {
    
    int32_t i;
    int32_t n;
    
    n = 0;
    
    n |= (reg->bits[0]);
    
    for(i = 1; i < 32; ++i)
        n |= (reg->bits[i] << i);
    
    return(n);
    
}

void
pack32_signed(register32_t *reg, const int32_t *val) {
    
    int32_t i;
    
    for(i = 0; i < 32; ++i)
        reg->bits[i] = ((*val) & (1UL << (i)) ? 1 : 0);
        
}

void
exec_instructions(char **instr, int32_t *n) {
    
    int i;
    int j;
    
    const char * FIRST_OP;
    const char * SECOND_OP;
    
    for(i = 0; i < *n; ) {
        
        g_cpu.IP  = instr[i];
        FIRST_OP  = g_cpu.IP;
        
        for(; *(FIRST_OP - 1) != ' '; ++FIRST_OP)
            ;
            
        SECOND_OP = FIRST_OP + 1;
        
        for(; *(SECOND_OP - 1) != ' ' && *SECOND_OP != 0; ++SECOND_OP)
            ;
            
        
        switch(get_opcode(g_cpu.IP)) {
            case MOV:
                
                switch(*FIRST_OP) {
                        
                    /* register to register */
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                        
                        printf("INFO: Moving value from REG %c to REG %c\n", *FIRST_OP, *SECOND_OP);
                        
                        for(j = 0; j < 31; ++j) 
                           g_cpu.REGISTER(*SECOND_OP).bits[j] = g_cpu.REGISTER(*FIRST_OP).bits[j];
                        
                        ++i;
                        
                        break;
                            
                    /* Integer to register */
                    default:
                        {
                            int32_t n;
                            
                            n = stoi(FIRST_OP, ' ');
                            
                            printf("INFO: Moving %d INTO REG %c\n", n, *SECOND_OP);
                            
                            pack32_signed(&g_cpu.REGISTER(*SECOND_OP), &n);
                            
                            ++i;
                                
                        }
                        
                }
                break;
            case INC:
                {
                    int32_t n;
                    
                    n = unpack32_signed(&g_cpu.REGISTER(*FIRST_OP));
                    
                    ++n;
                    
                    printf("INFO: Incrementing REG %c\n", *FIRST_OP);
                    
                    pack32_signed(&g_cpu.REGISTER(*FIRST_OP), &n);
                    
                    ++i;
                    
                    
                }
                break;
            case DEC:
                {
                    int32_t n;
                    
                    n = unpack32_signed(&g_cpu.REGISTER(*FIRST_OP));
                    
                    --n;
                    
                    printf("INFO: Decrementing REG %c\n", *FIRST_OP);
                    
                    pack32_signed(&g_cpu.REGISTER(*FIRST_OP), &n);
                    
                    ++i;
                    
                    
                }
                break;
            case JNZ:
                {
                    int32_t reg_val;
                    
                    reg_val = unpack32_signed(&g_cpu.REGISTER(*FIRST_OP));
                    
                    
                    i += ((stoi(SECOND_OP, 0) * !!reg_val) + !reg_val);
                        
                    if(i < *n) 
                        printf("INFO: Jumping to instruction: %s\n", (instr[i]));
                    else
                        printf("INFO: Ending program\n");
                }
                break;
            
        }
    }
    
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



opcode_e
get_opcode(const char *inst) {
    
    uint8_t     i;
    uint8_t     j;
    
    uint8_t     buf[5];
    
    const char *ptr;
    
    ptr = inst;
    
    /* get the opcode of the instruction */
    for(i = 0; *ptr != ' '; ++i, ++ptr)
        buf[i] = *ptr;
        
    buf[++i] = 0;
    
    for(i = 0; i < NUM_OPCODES; ++i) {
        
        if(strcmp(buf, g_opcodes[i].opcode) == 0)
            return g_opcodes[i].e;
        
    }
    
    return INVALID_OPCODE;
    
    
}


int32_t
stoi(const char * str, const uint8_t end) {
    
    int32_t x;
    int8_t sign;
    
    x    = 0;
    sign = -1;
    
    if(*str == '-') {
        ++str;
        sign = 1;
    }
        
    
    while(*str != end) {
        x = (x * 10) + (*str & 0x0F);
        ++str;
    }
    
    return(x * -sign);
    
}



char *
make_string(const char *inst, const uint8_t instlen) {
    
    char *ret;
    char *ptr;
    
    
    ret = malloc(sizeof(char) * instlen);
    
    if(ret == NULL)
        fprintf(stderr, "ERR: Could not allocate memory for instruction %s", inst);
        
    ptr = ret;
    
    for(; *inst != 0; ++inst, ++ptr)
        *ptr = *inst;
        
    *ptr = 0;
    
    return(ret);
    
}


void
init_opcodes() {
    
    g_opcodes[0].opcode = make_string("cpy", 4);
    g_opcodes[0].e = MOV;
    
    g_opcodes[1].opcode = make_string("inc", 4);
    g_opcodes[1].e = INC;
    
    g_opcodes[2].opcode = make_string("dec", 4);
    g_opcodes[2].e = DEC;
    
    g_opcodes[3].opcode = make_string("jnz", 4);
    g_opcodes[3].e = JNZ;
}

