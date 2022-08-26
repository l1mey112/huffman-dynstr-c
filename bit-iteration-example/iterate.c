#include <stdint.h>
#include <stdio.h>

// https://godbolt.org/z/PjcvcE7r7

int main(){

	uint8_t bytes[] = {
		0b11111111,
		0b00011000,
		0b00011000,
		0b11111111,
	};

	uint8_t val, mask;
	for (int i = 0; i < sizeof(bytes); i++)
	{
		val = bytes[i];
		for (mask = 1 << 7; mask != 0; mask >>= 1)
		{
			if (val & mask) {
				putchar('1');
			} else {
				putchar('0');
			}
		}
		putchar('\n');
	}
}