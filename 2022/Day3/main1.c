#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint32_t au32Vct[4];

int main(void) {
	uint8_t au8Buf[32];
	uint8_t u8Char;
	uint8_t u8Len;
	uint8_t u8Idx;

	uint8_t u8Bucket;

	uint8_t u8Offset;

	uint32_t u32FD;

	uint32_t u32Sum;

	u32FD = open("untitled.txt", O_RDONLY);

	u32Sum = 0;
	u8Idx = 0;

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
				au32Vct[u8Bucket] |= (1u << (26u - (u8Offset - au8Buf[u8Idx])));
			}
			/* Upper */
			au32Vct[0] &= au32Vct[2];
			/* Lower */
			au32Vct[1] &= au32Vct[3];
			printf("%d %d\n", au32Vct[0], au32Vct[1]);
			for (u8Idx = 0; u8Idx < (sizeof(uint32_t) * 8u); ++u8Idx) {
				u32Sum += (u8Idx + 26u) * ((au32Vct[0] & (1u << u8Idx)) > 0);
				u32Sum += (u8Idx) * ((au32Vct[1] & (1u << u8Idx)) > 0);
			}

			au32Vct[0] = 0;
			au32Vct[1] = 0;
			au32Vct[2] = 0;
			au32Vct[3] = 0;

			u8Len = 0;
			u8Idx = 0;

		} else {
			au8Buf[u8Len++] = u8Char;
		}
	}

	printf("%u", u32Sum);
	return 0;
}
