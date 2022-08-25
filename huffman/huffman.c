#include "strings.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct HuffmanNode HuffmanNode;
struct HuffmanNode {
	size_t weight;
	uint8_t ch;
	HuffmanNode *left;
	HuffmanNode *right;
};
// using a union + bool instead will be the exact same size.
// a huffman node is a character node when left and right is NULL

#define TREE_CAP 1024
HuffmanNode node_buf[TREE_CAP] = {0};
HuffmanNode *node_odd = NULL;
int node_count = 0;

int cmpfunc(const void* a, const void* b) {
   return ((HuffmanNode*)b)->weight - ((HuffmanNode*)a)->weight;
}

#define BYTE_RANGE 256

typedef struct {
	uint8_t ch;
	size_t weight;
} CharRank;

static inline HuffmanNode *new_node(uint8_t ch, size_t weight){
	node_buf[node_count].ch = ch;
	node_buf[node_count].weight = weight;
	assert(node_count++ <= TREE_CAP);
}

void rank_file(const char *filepath){
	CharRank file_ranking[BYTE_RANGE] = {0};
	uint8_t FILEBUFFER[1024];
	
	FILE *fp = fopen(filepath, "r");
	assert (fp);
	size_t read_amt = fread(FILEBUFFER, 1, 1024, fp);
	uint8_t *p = FILEBUFFER;
	assert(read_amt != 0);
	while (read_amt--){
		file_ranking[*p].ch = *p;
		file_ranking[*p].weight++;
		p++;
	}
	for (int i = 0; i < BYTE_RANGE; i++)
	{
		if (file_ranking[i].weight != 0)
			new_node(file_ranking[i].ch,file_ranking[i].weight);
	}
}

int main(){
	rank_file("huffman/huffman_text.txt");
	size_t size = 0;
	for (int i = 0; i < node_count; i++){
		size += node_buf[i].weight;
	}
	printf("------ %zu, from %d nodes ------\n", size, node_count);

	for (int i = 0; i < node_count; i++){
		if (node_buf[i].left == NULL && node_buf[i].right == NULL) {
			printf("HuffmanNode [ %d ]\n",node_buf[i].weight);
		} else {
			printf("HuffmanNode { '%c', %d }\n",node_buf[i].ch,node_buf[i].weight);
		}
	}
}