#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 

typedef struct {

  int8_t i8Top;
  uint8_t au8Stack[64];

} tstStack;

/*
            [J] [Z] [G]            
            [Z] [T] [S] [P] [R]    
[R]         [Q] [V] [B] [G] [J]    
[W] [W]     [N] [L] [V] [W] [C]    
[F] [Q]     [T] [G] [C] [T] [T] [W]
[H] [D] [W] [W] [H] [T] [R] [M] [B]
[T] [G] [T] [R] [B] [P] [B] [G] [G]
[S] [S] [B] [D] [F] [L] [Z] [N] [L]
 1   2   3   4   5   6   7   8   9 
*/
static tstStack astStacks[9] =
{ 
  {5u, {'S', 'T', 'H', 'F', 'W', 'R'}},
  {4u, {'S', 'G', 'D', 'Q', 'W'}},
  {2u, {'B', 'T', 'W'}},
  {7u, {'D', 'R', 'W', 'T', 'N', 'Q', 'Z', 'J'}},
  {7u, {'F', 'B', 'H', 'G', 'L', 'V', 'T', 'Z'}},
  {7u, {'L', 'P', 'T', 'C', 'V', 'B', 'S', 'G'}},
  {6u, {'Z', 'B', 'R', 'T', 'W', 'G', 'P'}},
  {6u, {'N', 'G', 'M', 'T', 'C', 'J', 'R'}},
  {3u, {'L', 'G', 'B', 'W'}}
};

/* First part solution */
static void
MAIN__vMove(uint8_t, uint8_t, uint8_t);

/* Second part solution */
static void
MAIN__vMove9001(uint8_t, uint8_t, uint8_t);

int main(void) {

  FILE *pstFile;
  char *pStr;

  uint8_t u8Moves;
  uint8_t u8From;
  uint8_t u8To;
  
  size_t u32Size;

  u32Size = 32u;

  pStr    = NULL;

  pstFile = fopen("untitled", "r");
 
  for(; (getline((char **)&pStr, &u32Size, pstFile)) != -1 ;) {
    
    sscanf(pStr, "move %d from %d to %d", (int *)&u8Moves, (int *)&u8From, (int *)&u8To);
    
    MAIN__vMove9001(u8Moves, u8From, u8To);
    
  }
  
  
  u8To = 0;
  
  for(; u8To < 9u; ++i) {
    printf("%c", astStacks[u8To].au8Stack[astStacks[u8To].i8Top]);
    
  }

  return 0;
}

static void
MAIN__vMove9001(uint8_t u8Moves, uint8_t u8From, uint8_t u8To) {

  uint8_t u8I;

  /* Nothing to move */
  if(astStacks[u8From - 1].au8Stack[0] == 0)
    return;

  /* Not enough crates */
  if((u8Moves - 1) > astStacks[u8From - 1].i8Top)
    return;
  
  
  for(u8I = 0; u8I < u8Moves; ++u8I) {
    
    /* Move box */
    astStacks[u8To - 1].au8Stack[(astStacks[u8To - 1].i8Top + u8Moves) - u8I]
      = astStacks[u8From - 1].au8Stack[astStacks[u8From - 1].i8Top];

    /* Set it to 0 */
    astStacks[u8From - 1].au8Stack[astStacks[u8From - 1].i8Top] = 0;
    
    --astStacks[u8From - 1].i8Top;
    
    
  }
  
  astStacks[u8To - 1].i8Top += u8Moves;
  
}


static void
MAIN__vMove(uint8_t u8Moves, uint8_t u8From, uint8_t u8To, ) {

  uint8_t u8I;

  /* Nothing to move */
  if(astStacks[u8From - 1].au8Stack[0] == 0)
    return;

  /* Not enough crates */
  if((u8Moves - 1) > astStacks[u8From - 1].i8Top)
    return;
  
  
  for(u8I = 0; u8I < u8Moves; ++u8I) {
    
    /* Move box */
    astStacks[u8To - 1].au8Stack[++astStacks[u8To - 1].i8Top]
      = astStacks[u8From - 1].au8Stack[astStacks[u8From - 1].i8Top];

    /* Set it to 0 */
    astStacks[u8From - 1].au8Stack[astStacks[u8From - 1].i8Top] = 0;
    
    --astStacks[u8From - 1].i8Top;
  }
  
}
