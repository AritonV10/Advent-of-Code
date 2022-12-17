#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




static FILE    *MAIN__pstFile;

static int32_t  MAIN__i32Register;


int main(void) {

  /* (X, Y) received */
  int16_t  i16Inc;
  int16_t  i16Delta;
  /* File */
  int32_t  i32CRT;
  uint64_t u64Size;

  char     u8Parse[16u];

  MAIN__pstFile = fopen("data.txt", "r");

  char *MAIN__pcDir;

  MAIN__pcDir = NULL;

  
  MAIN__i32Register = 1;
  i32CRT            = 1;
  /* Get the data */
  for (; (getline((char **)&MAIN__pcDir, &u64Size, MAIN__pstFile)) != -1;) {

    i16Inc = 0;
    (void)sscanf(MAIN__pcDir, "%s %d\n", u8Parse, (int *)&i16Inc);
    
    /* First clock cycle */
    i16Delta = MAIN__i32Register - i32CRT;
    
    printf("%c ", ('@' * (i16Delta <= 0 && i16Delta >= -2)) + (' ' * (i16Delta > 0 || i16Delta < -2)));
    printf("%c", ('\n' * ((i32CRT % 40) == 0)));
    i32CRT *= !((i32CRT % 40) == 0);
    ++i32CRT;
   
    /* noop just one clock cycle */
    if(*MAIN__pcDir == 'n')
      continue;
  
    i16Delta = MAIN__i32Register - i32CRT;
    printf("%c ", ('@' * (i16Delta <= 0 && i16Delta >= -2)) + (' ' * (i16Delta > 0 || i16Delta < -2)));
    printf("%c", ('\n' * ((i32CRT % 40) == 0)));
    i32CRT *= !((i32CRT % 40) == 0);
    ++i32CRT;
      /* Second clock cycle */
    MAIN__i32Register = (MAIN__i32Register + i16Inc) % 40;

    free(MAIN__pcDir);
    MAIN__pcDir = NULL;
    
    
  }

  return(0);
}
