
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




static FILE *MAIN__pstFile;

static int64_t MAIN__u64Sum;
static int32_t  MAIN__i32Register;
static uint32_t MAIN__u32Cycles;


int main(void) {

  /* (X, Y) received */
  char     u8Dir;
  uint16_t u16Quo;
  int16_t  i16Inc;
  /* File */
  int32_t  i32BytesRead;
  uint64_t u64Size;

  char     u8Parse[16u];

  MAIN__pstFile = fopen("data.txt", "r");

  char *MAIN__pcDir;

  MAIN__pcDir = NULL;


  MAIN__i32Register = 1;
  
  /* Get the data */
  for (; (getline((char **)&MAIN__pcDir, &u64Size, MAIN__pstFile)) != -1;) {

    i16Inc = 0;
    (void)sscanf(MAIN__pcDir, "%s %d\n", u8Parse, (int *)&i16Inc);

    /* First clock cycle */
    MAIN__u32Cycles += 1u;
    u16Quo = ((MAIN__u32Cycles + 20) % 40);
    
    if(u16Quo == 0) {
      printf("Line %sCycles %d\nRegister %d\n\n\n", MAIN__pcDir, MAIN__u32Cycles, MAIN__i32Register);
    }

    /* Are we at n + 40? */
    MAIN__u64Sum += (u16Quo == 0) * (MAIN__u32Cycles * MAIN__i32Register);

    /* noop just one clock cycle */
    if(*MAIN__pcDir == 'n')
      continue;
    
    /* Second clock cycle */
    ++MAIN__u32Cycles;
   
    
    u16Quo = ((MAIN__u32Cycles + 20) % 40);
    
    if(u16Quo == 0) {
      printf("Line %sCycles %d\nRegister %d\n\n\n", MAIN__pcDir, MAIN__u32Cycles, MAIN__i32Register);
    }
    MAIN__u64Sum += (u16Quo == 0) * (MAIN__u32Cycles * MAIN__i32Register);
    
    
    MAIN__i32Register += i16Inc;

    
    free(MAIN__pcDir);
    MAIN__pcDir = NULL;
    
    
  }

  printf("%d\n", MAIN__u64Sum);
  

  return(0);
}
