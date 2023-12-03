#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>

#define isSymbol(n) ( (!(isdigit((n)))) & ((n) != '.') )
#define LEN 
int au32Cache[300];
char Buf[2][256] = {0};

/* Symbol (x, y) coords */
static uint32_t u8X;
static uint32_t u8Y;
static uint32_t u32Sum;

void
vOccurance(uint32_t u32Num) {
    
    switch(Buf[u8X][u8Y]) {
		      
		case '*':
		/* Set as visited*/ 
		Buf[u8X][u8Y] = 'x';
	    au32Cache[u8Y] = u32Num;
        break;
		      
		/* Already visited -> fetch the previous value who visited it */
		case 'x':
        u32Sum += (au32Cache[u8Y] * u32Num);
        Buf[u8X][u8Y] = '.';
        break;
    }
}

uint32_t 
u32Find(uint32_t u8Line, uint32_t u8Jdx) {
    
        uint32_t u32K;
        uint32_t u32Num = 0;
        /* Get to the beggining of the number */
        for(u32K = u8Jdx; isdigit(Buf[u8Line][u32K]); --u32K)
            ;
        /* To integer */
        ++u32K;
       
        for(; u32K <= u8Jdx || isdigit(Buf[u8Line][u32K]); ++u32K) {
            
            u32Num = (u32Num * 10) + (Buf[u8Line][u32K] & 0x0Fu);
            Buf[u8Line][u32K] = '.';
        }
        
        
        
    return(u32Num);
    
     
}

uint32_t
main (void)
{
 

  uint32_t u32FD;

  uint32_t u8Char;
  
  uint32_t u8Jdx         = 1;
  uint32_t u8Line        = 0;
  uint32_t u8PrevLine    = 1;

 
  uint32_t u32Num        = 0;
  u32FD                  = open ("input.txt", 0);

  /* Fill the initial buffer */
  for(uint32_t n = 0; n < 2; ++n) {
      for(uint32_t m = 0; m < 256; ++m) {
          Buf[n][m] = '.';
      }
      
  }
  
  
  for (; (read (u32FD, &u8Char, 1)) != 0;) {
        
      if(u8Char == '\n' || u8Jdx > 141)  {
   
          
          u8Line ^= 1;
          u8PrevLine ^= 1;
          u8Jdx = 1;
          
          if(u8Char == '\n')
            continue;
          
      }
        
      Buf[u8Line][u8Jdx] = u8Char;

      if (u8Char != '.') {

	    if (isdigit(u8Char)) {
            
            if (isSymbol (Buf[u8Line][u8Jdx - 1]) || isSymbol (Buf[u8PrevLine][u8Jdx - 1])
		  || isSymbol (Buf[u8PrevLine][u8Jdx]) || isSymbol (Buf[u8PrevLine][u8Jdx + 1]) ) {
		      
              
                /* Get where the symbol is */
                if( (isSymbol(Buf[u8Line][u8Jdx - 1]))) {
                    u8X = u8Line;
                    u8Y = u8Jdx - 1;
                } else if( (isSymbol(Buf[u8PrevLine][u8Jdx - 1]))) {
                    u8X = u8PrevLine;
                    u8Y = u8Jdx - 1;
                } else if( (isSymbol(Buf[u8PrevLine][u8Jdx]))) {
                    u8X = u8PrevLine;
                    u8Y = u8Jdx;
                } else if( (isSymbol(Buf[u8PrevLine][u8Jdx + 1]))) {
                    u8X = u8PrevLine;
                    u8Y = u8Jdx + 1;
                }


                for(; u8Jdx < 141; read(u32FD, &u8Char, 1), ++u8Jdx) {
                    if(isdigit(u8Char)) 
                        Buf[u8Line][u8Jdx] = u8Char;
                    else
                        break;
                    
                }
                if(u8Char != '\n')
                    Buf[u8Line][u8Jdx] = u8Char;
                    
                /* Decrement file index because it is ahead by one */
                lseek(u32FD, -1, SEEK_CUR);
                
                --u8Jdx;
               
                u32Num = u32Find(u8Line, u8Jdx);
                
                vOccurance(u32Num);
                
		    }
                
	    } else {
	      
	      u8X = u8Line;
	      u8Y = u8Jdx;
	      
	      if (isdigit (Buf[u8Line][u8Jdx - 1]) ) {
	          u32Num =u32Find(u8Line, u8Jdx - 1);
	          vOccurance(u32Num);
		  
		
	      }
	      
	      if(isdigit(Buf[u8PrevLine][u8Jdx - 1])) {
	          u32Num = u32Find(u8PrevLine, u8Jdx - 1);
	          vOccurance(u32Num);

	      }
		  
		  if(isdigit(Buf[u8PrevLine][u8Jdx])) {
		    
		    u32Num = u32Find(u8PrevLine, u8Jdx);
		    vOccurance(u32Num);

		  }
		  
		  if(isdigit(Buf[u8PrevLine][u8Jdx + 1]) ) {
		    
		      u32Num = u32Find(u8PrevLine, u8Jdx + 1);
		      vOccurance(u32Num);

		  }
 
	    }
		  
		  /* Reset the coords */
		  u8X = 0;
		  u8Y = 0;

	}
	
      ++u8Jdx;

    }
    
  printf("%ld", (long)u32Sum);
  return 0;
}
