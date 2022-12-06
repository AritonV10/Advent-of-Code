#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define vSetBit(x, y) y |= (1u << (x - 'a'))
#define vUnsetBit(x, y) y &= ~(1u << (x - 'a'))
#define nIsSet(x, y) ((y & (1u << (x - 'a'))) > 0)

uint8_t  au8Char[14];
uint32_t u32Occurances;
uint32_t u32Decrements;

int main(void) {

  uint32_t u32FD;
  uint32_t u32BytesRead;

  uint8_t u8Chars;
  uint8_t u8Idx;
  uint8_t u8I;

  u8Idx        = 0;
  u8Chars      = 0;
  u32BytesRead = 0;
  u32FD        = open("data.txt", 0);

  /* Do the initial read of the default 4 characters */

  for (;;) {

    if (u8Idx >= 14u) {
      
      /* Linear approach - we could speed it up by not checking the letters
       * every time, but just checking the last letter added since we already
       * have the occurances of the previous ones */
      for (u8I = 0; u8I < 14u; ++u8I) {

        if (nIsSet(au8Char[u8I], u32Occurances))
          break;
        else
          ++u8Chars;

        vSetBit(au8Char[u8I], u32Occurances);
      }

      if (u8Chars == 14u)
        break;

      for(u8I = 0; u8I < 13u; ++u8I)
        au8Char[u8I] = au8Char[u8I + 1];
    
      /* Clamp the index */
      u8Idx = 13u;
    }

    /* Read char */
    if ((read(u32FD, &au8Char[u8Idx], 1u)) == 0)
      break;

    ++u8Idx;
    ++u32BytesRead;

    /* Reset for next iteration */
    u32Occurances = 0;
    u8Chars       = 0;
  }

  printf("%d", u32BytesRead);

  return 0;
}
