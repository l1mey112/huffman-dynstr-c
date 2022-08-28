#include "huffman.h"

BitArray bitarray_new(size_t bytes){
	BitArray b;
	b.data = malloc(bytes);
	b.bitlen = 0;
	b.idx = 0;
	b.cap = bytes;

	return b;
}

void bitarray_write(BitArray *b, Bit w){
	if (w) b->data[b->idx] |= 1 << ( b->bitlen % 8 );

	b->bitlen++;

	if (b->bitlen && b->bitlen % 8 == 0) {
		b->idx++;
		if (b->idx >= b->cap) {
			b->cap *= 2;
			b->data = realloc(b->data, b->cap /* * sizeof(uint8_t) */);
		}
	}
}

void bitarray_iter(BitArray * b, bitarray_iter_cb cb, void *user_data){
	size_t idx = 0;
	uint8_t mask = 1;
	for (size_t bit = 0; bit <= b->bitlen; bit++)
	{
		if (bit && bit % 8 == 0) {
			idx++;
			mask = 1;
		} else {
			cb(b->data[idx] & mask, user_data);
			mask <<= 1;
		}
	}
}

static void bitarray_print_cb(Bit a, void *_){
	if (a) {
		putchar('1');
	} else {
		putchar('0');
	}
}

void bitarray_print(BitArray * b){
	bitarray_iter(b, bitarray_print_cb, NULL);
	putchar('\n');
}