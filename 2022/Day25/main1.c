#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAIN__nMatrixLen (10000u)

#define MAIN__nStartPosition (MAIN__nMatrixLen >> 1u)

#define MAIN__nRotateY90(x, y) (-x)

#define MAIN__nRotateX90(x, y) (y)

#define MAIN__nHeadX (MAIN__i32HeadPos[0])
#define MAIN__nHeadY (MAIN__i32HeadPos[1u])

#define MAIN__nTailX (MAIN__i32TailPos[0])
#define MAIN__nTailY (MAIN__i32TailPos[1u])

#define MAIN__nXDiff (MAIN__nHeadX - MAIN__nTailX)
#define MAIN__nYDiff (MAIN__nHeadY - MAIN__nTailY)



static void MAIN__vUpdate(int32_t *, uint8_t, uint8_t, int16_t);

static void *MAIN__vpCalloc(uint32_t, uint32_t);

/*************************************************/

static char *MAIN__pcDir;
static FILE *MAIN__pstFile;

static const int8_t MAIN__ai8Map[5u][5u] = {{0, 1, 1, 1, 0},
                                            {1, 0, 0, 0, 1},
                                            {0, 0, 0, 0, 0},
                                            {-1, 0, 0, 0, -1},
                                            {0, -1, -1, -1, 0}};

static int32_t MAIN__i32HeadPos[2u];
static int32_t MAIN__i32TailPos[2u];

static uint32_t MAIN__u32Part1;

static uint8_t **MAIN__au8Map;

int main(void) {

  /* (X, Y) received */
  char     u8Dir;
  int16_t  i16Inc;
  /* File */
  int32_t  i32BytesRead;
  uint64_t u64Size;

  MAIN__pstFile = fopen("data.txt", "r");

  MAIN__nTailX = MAIN__nStartPosition;
  MAIN__nTailY = MAIN__nStartPosition;
  MAIN__nHeadX = MAIN__nTailX;
  MAIN__nHeadY = MAIN__nTailY;

  /* Initialize the map */
  MAIN__au8Map = malloc(MAIN__nMatrixLen * sizeof(uint8_t *));

  for (i16Inc = 0; i16Inc < MAIN__nMatrixLen; ++i16Inc)
    MAIN__au8Map[i16Inc] = MAIN__vpCalloc(MAIN__nMatrixLen, sizeof(uint8_t));

  /* Get the data */
  for (; (fscanf(MAIN__pstFile, "%c %hd\n", &u8Dir, &i16Inc)) != EOF;) {
    
    switch (u8Dir) {
      case 'L':
      case 'R':

        /* Update X coordinates */
        MAIN__vUpdate(&MAIN__nHeadY, ~u8Dir, u8Dir, i16Inc);
  
        break;
  
      case 'U':
      case 'D':

        /* Update Y coordinates */
        MAIN__vUpdate(&MAIN__nHeadX, u8Dir, u8Dir, i16Inc);
        
        break;
      }
  }

  printf("%d\n", MAIN__u32Part1);
  

  return 0;
}


static void 
MAIN__vUpdate(int32_t *Position, uint8_t u8Condition, uint8_t u8Char, int16_t i16Moves) {

    int16_t i16XDiff;
    int16_t i16YDiff;
    int16_t i16Idx;
      
      for (i16Idx = 1; i16Idx <= i16Moves; ++i16Idx) {

        *Position += (1 - 2 * ((u8Condition & 0x10) > 0));
        /* Save the previous values before they are changed */
        i16XDiff = MAIN__ai8Map[2 - MAIN__nXDiff][2 - MAIN__nYDiff];
        /* Do a 90deg rotation of the coordinates */
        i16YDiff = MAIN__ai8Map[2 - MAIN__nYDiff][2 + MAIN__nXDiff];
      
        MAIN__nTailX += i16XDiff;
        MAIN__nTailY += i16YDiff;

        MAIN__u32Part1 += (MAIN__au8Map[MAIN__nTailX][MAIN__nTailY] != 1u);
        
        MAIN__au8Map[MAIN__nTailX][MAIN__nTailY] = 1u;
                
        
      } 
  
}

static void *MAIN__vpCalloc(uint32_t u32NoElem, uint32_t u32Size) {

  void *pvBlock;

  pvBlock = calloc(u32NoElem, u32Size);

  if (pvBlock == NULL)
    exit(EXIT_FAILURE);

  return (pvBlock);
}
