#include "huffman.h"

typedef struct {
	uint8_t *data;
	size_t len;
} Buffer;

Buffer open_and_read_bytes(const char *filepath){
	FILE *fp = fopen(filepath, "r");
	assert(fp != NULL && "failed to open file!");
	assert(fseek(fp, 0, SEEK_END) == 0);
	
	int64_t len = ftell(fp);
	assert(len > 0);

	rewind(fp);

	uint8_t *buffer = malloc(len);
	size_t elms = fread(buffer, 1, len, fp);

	fclose(fp);

	if (elms == 0 && len > 0) {
		assert(0);
	}

	assert(elms != 0 && "cannot read empty file!");

	return (Buffer){
		.data = buffer,
		.len = elms
	};
}

#define BYTE_RANGE 256

HuffmanNode *new_huffman_node(uint8_t ch, size_t weight){
	HuffmanNode *n = malloc(sizeof(HuffmanNode));
	// use preallocated buffer later

	n->left = n->right = NULL; 
	n->ch = ch;
	n->weight = weight;

	return n;
}

MinHeap *huffman_rank(Buffer b){
	size_t chars[256] = {0};

	uint8_t *p = b.data;
	size_t pos = b.len; 
	// len can never be zero, is checked when read
	while (pos--) chars[*p++]++;

	MinHeap *h = new_heap(50);

	// measure weights
	for (int i = 0; i < BYTE_RANGE; i++)
	{
		if (chars[i])
			heap_push(h,new_huffman_node((uint8_t)i, chars[i]));
	}

	return h;
}

int main(){
	Buffer file = open_and_read_bytes("huffman/huffman_text.txt");
	MinHeap *h = huffman_rank(file);

	HuffmanNode *n;
	do {
		n = heap_pop(h);
		printf("{ '%c' %zu }\n", n->ch, n->weight);
	} while(h->len);
}