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

typedef struct {
	size_t len;
	size_t cap;

	HuffmanNode **data;
} MinHeap;

MinHeap *new_heap(size_t cap);
void heap_push(MinHeap *h, HuffmanNode *a);
HuffmanNode *heap_pop(MinHeap *h);
void heapify_all(MinHeap *h);

#endif