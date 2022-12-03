#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define RESET(x)                                                               \
	{                                                                          \
		au32Vct[x][0] = 0;                                                     \
		au32Vct[x][1] = 0;                                                     \
		au32Vct[x][2] = 0;                                                     \
		au32Vct[x][3] = 0;                                                     \
	}

uint32_t au32Vct[3][4];

int main(void) {
	uint8_t au8Buf[32];

	uint8_t u8Char;
	uint8_t u8Len;
	uint8_t u8Idx;
	uint8_t u8Group;
	uint8_t u8Bucket;
	uint8_t u8Offset;

	uint32_t u32FD;
	uint32_t u32Sum;
	uint32_t u32Sum2;
	/* Init */
	u32Sum = 0;
	u32Sum2 = 0;
	u8Idx = 0;
	u8Group = 0;
	/* Get the data */
	u32FD = open("untitled.txt", O_RDONLY);

	for (; (read(u32FD, &u8Char, 1u)) != 0;) {
		if (u8Char == '\n') {
			u8Idx = 0;
			/* First half */
			for (; u8Idx < u8Len; ++u8Idx) {
				/* Find which variable to use [0 = upper, 1 = lower] */
				u8Bucket = ((au8Buf[u8Idx] >> 5u) & 0x01u) +
					(u8Idx >= (u8Len >> 1)) * 2;

				/*
				 * If lower => 0101 | [0101, 0111] => 0x7A
				 * If upper => 0101 | [0100, 0101] => 0x5A
				 */
				u8Offset = ((0x5Au | (au8Buf[u8Idx] & 0xF0u)));
				printf(
					"%d %c %d\n",
					u8Bucket,
					au8Buf[u8Idx],
					26u - (u8Offset - au8Buf[u8Idx]));
				au32Vct[u8Group][u8Bucket] |=
					(1u << (26u - (u8Offset - au8Buf[u8Idx])));
			}

#define AND(x, y) (x[u8Group][y] & x[u8Group][y + 2])

			for (u8Idx = 0; u8Idx < (sizeof(uint32_t) * 8u); ++u8Idx) {
				u32Sum +=
					(u8Idx + 26u) * ((AND(au32Vct, 0) & (1u << u8Idx)) > 0);
				u32Sum += (u8Idx) * ((AND(au32Vct, 1) & (1u << u8Idx)) > 0);
			}

			u8Len = 0;
			u8Idx = 0;

			u8Group = (u8Group + 1) % 3u;

			/* 3 groups */
			if (u8Group == 0) {
				/* Merge the uppercases */
				au32Vct[0][0] |= au32Vct[0][2];
				au32Vct[1][0] |= au32Vct[1][2];
				au32Vct[2][0] |= au32Vct[2][2];
				/* Merge the lower cases */
				au32Vct[0][1] |= au32Vct[0][3];
				au32Vct[1][1] |= au32Vct[1][3];
				au32Vct[2][1] |= au32Vct[2][3];

				au32Vct[0][0] &= (au32Vct[1][0] & au32Vct[2][0]);
				au32Vct[0][1] &= (au32Vct[1][1] & au32Vct[2][1]);

				/* Get the letter position */
				u8Idx = 0;

				for (; u8Idx < 32u; ++u8Idx) {
					u32Sum2 +=
						(u8Idx + 26u) * ((au32Vct[0][0] & (1u << u8Idx)) > 0);
					u32Sum2 += (u8Idx) * ((au32Vct[0][1] & (1u << u8Idx)) > 0);
				}

				u8Idx = 0;
				/* Reset for the next groups */
				RESET(0);
				RESET(1);
				RESET(2);
			}

		} else {
			au8Buf[u8Len++] = u8Char;
		}
	}

	printf("%u\n", u32Sum);
	printf("%u", u32Sum2);
	return 0;
}
