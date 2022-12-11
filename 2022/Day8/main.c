#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <fcntl.h>
#include <unistd.h> 


#define nLINES (99)
#define nCOLUMNS (nLINES)

static FILE *MAIN__pstFile;
static char *MAIN__pcLine;

static uint32_t MAIN__au32Matrix[nLINES][nCOLUMNS];

int main(void) {

  /* Line & Column idx */
  uint8_t  u8L;
  uint8_t  u8C;
  uint8_t  u8K;
  uint32_t u32Rel;
  uint32_t u32Max;
  uint32_t u32NoTrees; 
  uint64_t u64Sz;
  
  u8L         = 0;
  u8C         = 0;
  u32Rel      = 1u;
  u32Max      = 0;

  MAIN__pstFile = fopen("data.txt", "r");
  /* Let getline malloc it */
  MAIN__pcLine = NULL;

  for(; (getline((char **)&MAIN__pcLine, &u64Sz, MAIN__pstFile)) != -1 ; ) {
    
    for(; *MAIN__pcLine != '\n'; ++MAIN__pcLine)
      MAIN__au32Matrix[u8L][u8C++] = (*MAIN__pcLine & 0x0Fu);

    ++u8L;
    u8C = 0;
    
     MAIN__pcLine = NULL;     
  }

  for(u8L = 0; u8L < nLINES; ++u8L){
    for(u8C = 0; u8C < nCOLUMNS; ++u8C) {

      /* Left */
   
      for(u8K = u8C - 1; u8K >= 0; --u8K) {
        if(MAIN__au32Matrix[u8L][u8K] >= MAIN__au32Matrix[u8L][u8C]) {
          pruint32_tf("[%d] is >= than [%d]\n", MAIN__au32Matrix[u8L][u8K], MAIN__au32Matrix[u8L][u8C]);
          ++u32NoTrees;
          break;
        } 
         pruint32_tf("[%d] is <= than [%d]\n", MAIN__au32Matrix[u8L][u8K], MAIN__au32Matrix[u8L][u8C]);
        ++u32NoTrees;
            
      }
      
      u32Rel *= u32NoTrees;
      u32NoTrees = 0;

      pruint32_tf("Checking right of [%d]\n", MAIN__au32Matrix[u8L][u8C]);
      /* Right */
      for(u8K = u8C + 1; u8K <= (nCOLUMNS - 1u); ++u8K) {
        if(MAIN__au32Matrix[u8L][u8K] >= MAIN__au32Matrix[u8L][u8C]) {
          pruint32_tf("[%d] is >= than [%d]\n", MAIN__au32Matrix[u8L][u8K], MAIN__au32Matrix[u8L][u8C]);
          ++u32NoTrees;
          break;
        } 
        pruint32_tf("[%d] is < than [%d]\n", MAIN__au32Matrix[u8L][u8K], MAIN__au32Matrix[u8L][u8C]);
        ++u32NoTrees;
      }
      
      u32Rel *= u32NoTrees;
      u32NoTrees = 0;
      pruint32_tf("Checking down of [%d]\n", MAIN__au32Matrix[u8L][u8C]);
      /* Down */
      for(u8K = u8L + 1; u8K <= (nLINES - 1); ++u8K) {
        if(MAIN__au32Matrix[u8K][u8C] >= MAIN__au32Matrix[u8L][u8C]) {
          pruint32_tf("[%d] is >= than [%d]\n", MAIN__au32Matrix[u8K][u8C], MAIN__au32Matrix[u8L][u8C]);
          ++u32NoTrees;
          break;
        } 
        pruint32_tf("[%d] is < than [%d]\n", MAIN__au32Matrix[u8K][u8C], MAIN__au32Matrix[u8L][u8C]);
        ++u32NoTrees;
      }
        
      u32Rel *= u32NoTrees;
      u32NoTrees = 0;
      pruint32_tf("Checking up of [%d]\n", MAIN__au32Matrix[u8L][u8C]);
      /* Up */
      for(u8K = u8L - 1; u8K >= 0; --u8K) {
       if(MAIN__au32Matrix[u8K][u8C] >= MAIN__au32Matrix[u8L][u8C]) {
         
        pruint32_tf("[%d] is >= than [%d]\n", MAIN__au32Matrix[u8K][u8C], MAIN__au32Matrix[u8L][u8C]);
          ++u32NoTrees;
          break;
        } 
        pruint32_tf("[%d] is < than [%d]\n", MAIN__au32Matrix[u8K][u8C], MAIN__au32Matrix[u8L][u8C]); 
        ++u32NoTrees;
      }
      
      u32Rel *= u32NoTrees;
     
    pruint32_tf("%d%s", u32Rel, "\n\n");
    if(u32Rel > u32Max)
      u32Max = u32Rel;

      u32Rel = 1;
      u32NoTrees = 0;
    }

  }
   printf("%d", u32Max);
    
  return(0);
}


