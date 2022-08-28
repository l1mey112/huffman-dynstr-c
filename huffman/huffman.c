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
	assert(buffer);
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

HuffmanNode buffer[1024];
int amt = 4;

HuffmanNode *global_buffer_alloc(){
	int p = amt;
	
	amt++;

	assert( amt < 1024 );

	return &buffer[p];
}

/* RelativeBuffer global_buffer; */

/* void setup_global_buffer(size_t cap){
	/* global_buffer.cap = cap;
	global_buffer.len = 0;

	global_buffer.data = malloc(sizeof(HuffmanNode) * cap);
} 

/* #define Mrel(rptr) (HuffmanNode*)((void*)global_buffer.data + rptr) 
#define Drel(ptr)  (HuffmanNode*)(ptr - (void*)global_buffer.data)
 */
/* HuffmanNode *global_buffer_alloc(){
	uint64_t place = global_buffer.len;
	global_buffer.len++;

	if (global_buffer.len > global_buffer.cap){
		printf("realloc\n");
		global_buffer.cap *= 2;
		global_buffer.data = realloc(global_buffer.data, sizeof(HuffmanNode) * global_buffer.cap);
	}

	return global_buffer.data + place;
}
 */
HuffmanNode *new_huffman_node(uint8_t ch, size_t weight){
	HuffmanNode *n = global_buffer_alloc();

	n->left = NULL; 
	n->right = NULL;
	n->ch = ch;
	n->weight = weight;

	//printf("%zu\n",(void*)n - (void*)global_buffer.data);
	return n;
}

HuffmanNode *join_huffman_node(HuffmanNode *a, HuffmanNode *b){
	HuffmanNode *n = global_buffer_alloc();
	
	n->ch = 0;
	n->weight = a->weight + b->weight;
	n->left = a;
	n->right = b;

	return n;
}

#define BYTE_RANGE 256

MinHeap huffman_rank(Buffer b){
	size_t chars[BYTE_RANGE] = {0};

	uint8_t *p = b.data;
	size_t pos = b.len; 
	// len can never be zero, is checked when read
	while (pos--) chars[*p++]++;

	MinHeap h = new_heap(50);

	// measure weights
	for (int i = 0; i < BYTE_RANGE; i++)
	{
		if (chars[i]){
			heap_push(&h, new_huffman_node(i, chars[i]));
		}
	}

	return h;
}

void print_bits(HuffmanNode *root, uint8_t *buf, uint8_t top) {
 
    if (root->left) {
        buf[top] = 0;
        print_bits(root->left, buf, top + 1);
    }
 
    if (root->right) {
        buf[top] = 1;
        print_bits(root->right, buf, top + 1);
    }

    if (!(root->left) && !(root->right)) {
        printf("'%c': ", root->ch == '\n' ? '_' : root->ch);
        
		for (int i = 0; i < top; i++){
        	printf("%d", buf[i]);
		}

		printf("\n");
	}
}

/* void SPACES(int level){
	while(level != 0){
		printf("-");
		level--;
	}
}

void print_tree(HuffmanNode *root, int level) {
	if (root->left) {
		SPACES(level); printf("%d - %d\n",root->weight,level/2);
        print_tree(root->left, level+2);
    }
 
    if (root->right) {
		SPACES(level); printf("%d - %d\n",root->weight,level/2);
        print_tree(root->right, level+2);
    }
} */

// 256, 128, 64, 32, 16, 8, 4, 2, 1
// 9 possible left or right branches, store this in a u16.

typedef struct {
	uint16_t data;
	uint8_t len;
} HuffmanMapEntry;

void walk_huffman(HuffmanMapEntry map[], HuffmanNode *root, uint16_t data, uint8_t len) {
    if (root->left) {
        walk_huffman(map, root->left, data, len + 1);
    }
 
    if (root->right) {
		data |= 1 << len;
        walk_huffman(map, root->right, data, len + 1);
    }

	if (!(root->left) && !(root->right)) {
		map[root->ch].data = data;
		map[root->ch].len = len;

		// uint8_t mask = 0;
		// while (len--)
		//     putchar(data & 1 << mask++ ? '1' : '0');
		// putchar('\n');
	}
}

void encode_huffman(HuffmanMapEntry map[], Buffer file, BitArray *b){
	for (size_t i = 0; i < file.len; i++)
	{
		HuffmanMapEntry m = map[file.data[i]];
		uint8_t mask = 0;
		while (m.len--)
			bitarray_write(b, m.data & 1 << mask++);
	}
}

void decode_huffman(HuffmanNode *root, BitArray *b){
	HuffmanNode *node = root;

	size_t idx = 0;
	uint8_t mask = 1;
	for (size_t bit = 0; bit <= b->bitlen; bit++)
	{
		if (bit && bit % 8 == 0) {
			idx++;
			mask = 1;
		}

		if (!(b->data[idx] & mask)){
			node = node->left;
		} else {
			node = node->right;
		}

		if (!node->left && !node->right) {
			putchar(node->ch);
			node = root;
		}

		mask <<= 1;
	}
	putchar('\n');
}

/* file.txt
   file.txt.hcoding
   file.txt.hkey    */

int main(){
	/* setup_global_buffer(100); */

	Buffer file = open_and_read_bytes("huffman/huffman_text.txt");
	MinHeap h = huffman_rank(file);

	fwrite(file.data, 1, file.len, stdout);

	printf("\n---------------------\n\n");

	while(h.len != 1) {
		heap_push(&h, join_huffman_node(heap_pop(&h), heap_pop(&h)));
	}
	HuffmanNode *root = heap_pop(&h);
	free(h.data);
	
	BitArray b = bitarray_new(50);
	HuffmanMapEntry map[256];
	walk_huffman(map, root, 0, 0);

	encode_huffman(map, file, &b);
	bitarray_print(&b);

	//printf("\n%zu bytes saved excluding overhead (uncompressed %zu, compressed %zu)\n",file.len - b->idx, file.len, b->idx);

	printf("\n---------------------\n\n");

	decode_huffman(root, &b);
	free(file.data);
	free(b.data);
}