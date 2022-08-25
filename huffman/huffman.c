#include "huffman.h"
#include <math.h>

typedef struct {
	uint8_t *data;
	size_t len;
} Buffer;

Buffer open_and_read_bytes(const char *filepath){
	FILE *fp = fopen(filepath, "rb");
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

HuffmanNode *new_huffman_node(uint8_t ch, size_t weight){
	HuffmanNode *n = malloc(sizeof(HuffmanNode));
	// use preallocated buffer later

	n->left = NULL; 
	n->right = NULL;
	n->ch = ch;
	n->weight = weight;

	return n;
}

HuffmanNode *dupe_huffman_node(HuffmanNode *a){
	HuffmanNode *n = malloc(sizeof(HuffmanNode));
	memcpy(n, a, sizeof(HuffmanNode));

	return n;
}

HuffmanNode *join_huffman_node(HuffmanNode *a, HuffmanNode *b){
	HuffmanNode *n = malloc(sizeof(HuffmanNode));
	
	n->ch = 0;
	n->weight = a->weight + b->weight;
	n->left = a;
	n->right = b;

	return n;
}

#define BYTE_RANGE 256

MinHeap *huffman_rank(Buffer b){
	size_t chars[BYTE_RANGE] = {0};

	uint8_t *p = b.data;
	size_t pos = b.len; 
	// len can never be zero, is checked when read
	while (pos--) chars[*p++]++;

	MinHeap *h = new_heap(50);

	// measure weights
	for (int i = 0; i < BYTE_RANGE; i++)
	{
		if (chars[i]){
			heap_push(h, new_huffman_node(i, chars[i]));
		}
	}

	return h;
}

size_t size = 0;

void printCodes(HuffmanNode *root, uint8_t *buf, uint8_t top) {
 
    if (root->left) {
        buf[top] = 0;
        printCodes(root->left, buf, top + 1);
    }
 
    if (root->right) {
        buf[top] = 1;
        printCodes(root->right, buf, top + 1);
    }

    if (!(root->left) && !(root->right)) {
        printf("'%c': ", root->ch == '\n' ? '_' : root->ch);
        
		for (int i = 0; i < top; i++){
        	printf("%d", buf[i]);
		}

		size += top;

		printf("\n");
	}
}

int main(){
	Buffer file = open_and_read_bytes("huffman/huffman_text.txt");
	MinHeap *h = huffman_rank(file);

	while(h->len > 1) {
		heap_push(h, join_huffman_node(heap_pop(h), heap_pop(h)));
	}

	uint8_t arr[100] = {0};

	printCodes(h->data[0], arr, 0);

	printf("bytes without compression %zu\n",file.len);
	printf("bytes after compression %zu\n",size / 8 + (size % 8));
}