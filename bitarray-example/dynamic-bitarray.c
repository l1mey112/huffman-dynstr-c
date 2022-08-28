#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
	size_t bitlen;  // bits written, not bytes

	size_t  idx;
	size_t  cap;    // in bytes

	uint8_t *data;
} BitArray;

#define roundup8(x) ((x + 7) & (-8))
#define rounddown8(x) x & (-8)

typedef _Bool Bit;

BitArray *bitarray_new(size_t bytes){
	BitArray *b = malloc(sizeof(BitArray) + bytes);
	b->data = (void*)b + sizeof(BitArray);

	b->bitlen = 0;
	b->cap = bytes;

	b->idx = 0;

	return b;
}

void *bitarray_writeone(BitArray * b, Bit w){
	if (w) b->data[b->idx] |= 1 << ( b->bitlen % 8 );

	b->bitlen++;

	printf("wrote data");
	if (b->bitlen && b->bitlen % 8 == 0) {
		printf(" - flushed data\n");
		b->idx++;
		if (b->idx >= b->cap) {
			printf("realloc!\n");
			b->cap *= 2;
			abort();
			// TODO: THIS REALLOC THING IS ABSOLUTELY WRONG
			//       YOU DONT ACTUALLY RETURN THE PTR, HOW
			//       CAN FUTURE CALLS GET THE CHANGED PTR?
			/* b = realloc(b, b->cap + sizeof(BitArray));
			// makes it easier to free the entire thing:
			// free(bitarray); */
		}
	} else {
		printf("\n");
	}
}

typedef void (*bitarray_iter_cb)(Bit);

void *bitarray_iter(BitArray * b, bitarray_iter_cb cb){
	size_t idx = 0;
	uint8_t mask = 1;
	for (size_t bit = 0; bit <= b->bitlen; bit++)
	{
		if (bit && bit % 8 == 0) {
			idx++;
			mask = 1;
		} else {
			cb(b->data[idx] & mask);
			mask <<= 1;
		}
	}
}

void bitarray_print_cb(Bit a){
	if (a) {
		putchar('1');
	} else {
		putchar('0');
	}
}

void *bitarray_print(BitArray * b){
	bitarray_iter(b, bitarray_print_cb);
	putchar('\n');
}

int main(){
	BitArray *b = bitarray_new(1);

	bitarray_writeone(b, 1);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);

	bitarray_print(b);
}