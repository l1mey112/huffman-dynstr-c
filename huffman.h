#ifndef _H_HUFFMAN
#define _H_HUFFMAN

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct HuffmanNode HuffmanNode;
struct HuffmanNode {
	size_t weight;
	uint8_t ch;

	HuffmanNode *left, *right;
};
// using a union + bool instead will be the exact same size.
// a huffman node is a character/leaf node when left and right is NULL

/* typedef struct {
	size_t len;
	size_t cap;

	HuffmanNode *data;
} RelativeBuffer; */
// for serialisied data

typedef struct {
	size_t len;
	size_t cap;

	HuffmanNode **data;
} MinHeap;

MinHeap new_heap(size_t cap);
void heap_push(MinHeap *h, HuffmanNode *a);
HuffmanNode *heap_pop(MinHeap *h);
void heapify_all(MinHeap *h);

typedef struct {
	size_t bitlen;  // bits written, not bytes

	size_t  idx;
	size_t  cap;    // in bytes

	uint8_t *data;
} BitArray;

typedef _Bool Bit;

BitArray bitarray_new(size_t bytes);
void bitarray_write(BitArray * b, Bit w);

typedef void (*bitarray_iter_cb)(Bit, void*);
void bitarray_iter(BitArray * b, bitarray_iter_cb cb, void *user_data);
void bitarray_print(BitArray * b);

typedef struct {
	uint8_t *data;
	size_t len;
} Buffer;

typedef struct {
	uint16_t data;
	uint8_t len;
} HuffmanMapEntry;

Buffer open_and_read_bytes(const char *filepath);
HuffmanNode *huffman_rank(Buffer b);
void walk_huffman(HuffmanMapEntry map[], HuffmanNode *root, uint16_t data, uint8_t len);

void encode_huffman(HuffmanMapEntry map[], Buffer file, BitArray *b);
void decode_huffman(HuffmanNode *root, BitArray *b);

#endif