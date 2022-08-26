#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct {
	size_t bitlen; // bits written, not bytes
	size_t bitcap; // grows by 8 bits

	uint8_t *data;
} BitArray;

#define roundup8(x) ((x + 7) & (-8))
#define rounddown8(x) x & (-8)

typedef _Bool Bit;

BitArray *bitarray_new(size_t bytes){
	BitArray *b = malloc(sizeof(BitArray) + bytes);
	b->data = (void*)b + sizeof(BitArray);

	b->bitlen = 0;
	b->bitcap = bytes * 8;

	return b;
}

// TODO, build up buffer in static variable of a byte,
//       then flush when needed.

void *bitarray_writeone(BitArray * b, Bit w){
	size_t bnd = rounddown8(b->bitlen);
	size_t bit = b->bitlen - bnd;
	size_t idx = bnd / 8; 

	printf("%d | %zu - %zu - %zu\n", w, bnd, bit, idx);

	b->bitlen++;
	assert(b->bitlen <= b->bitcap);
}

int main(){
	BitArray *b = bitarray_new(100);

	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);
	bitarray_writeone(b, 0);
	bitarray_writeone(b, 1);
}