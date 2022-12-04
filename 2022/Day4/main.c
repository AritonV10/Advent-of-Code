#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
	int8_t ai8Buf[4];

	uint8_t u8Char;
	uint8_t u8Idx;

	uint32_t u32FD;
	uint32_t u32Sum;
	uint32_t u32Nr;
	uint32_t u32Lines;
	uint32_t u32Sum2;
	/* Init */
	u32Sum = 0;
	u32Nr = 0;
	u32Lines = 0;
	u32Sum2 = 0;

	u8Idx = 0;

	/* Get the data */
	u32FD = open("untitled.txt", O_RDONLY);

	for (; (read(u32FD, &u8Char, 1u)) != 0;) {
		if ((u8Char & 0xF0) == 0x30) {
			u32Nr = (u32Nr * 10) + (u8Char & 0x0F);
		} else {
			ai8Buf[u8Idx++] = u32Nr;

			u32Nr = 0;

			if (u8Char == '\n') {
				++u32Lines;
				/* Within each other */
				u32Sum += ((ai8Buf[0] - ai8Buf[2]) <= 0 &&
						   (ai8Buf[1] - ai8Buf[3]) >= 0) |
					((ai8Buf[0] - ai8Buf[2]) >= 0 &&
					 (ai8Buf[1] - ai8Buf[3]) <= 0);
				/* Those who don't overlap at all */
				u32Sum2 += ((ai8Buf[0] > ai8Buf[3]) || (ai8Buf[1] < ai8Buf[2]));

				
				u8Idx = 0;
			}
		}
	}

	printf("%d %d", u32Sum, (u32Lines - u32Sum2));
	return(0);
}
