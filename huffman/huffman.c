#include "strings.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct HuffmanNode HuffmanNode;
struct HuffmanNode {
	bool is_internal_node;
	size_t weight;
	union {
		uint8_t ch;
		// HuffmanNode *parent;
	};
	union {
		HuffmanNode *left;
		HuffmanNode *right;
		// HuffmanNode *parent;
	};
};

#define TREE_CAP 1024
HuffmanNode node_buf[TREE_CAP] = {0};
HuffmanNode *node_odd = NULL;
int node_count;

int cmpfunc(const void* a, const void* b) {
   return ((HuffmanNode*)a)->weight - ((HuffmanNode*)b)->weight;
}

#define BYTE_RANGE 256

void rank_file(const char *filepath){
	HuffmanNode file_ranking[BYTE_RANGE] = {0};
	uint8_t FILEBUFFER[1024];
	
	FILE *fp = fopen(filepath, "r");
	assert (fp);
	fread(FILEBUFFER, 1024, 1, fp);

	uint8_t *p = FILEBUFFER;
	for (;;){
		uint8_t ch = *p;
		if (ch == 0){
			break;
		}
		file_ranking[ch].ch = ch;
		file_ranking[ch].weight++;
		p++;
	}
	qsort(file_ranking, BYTE_RANGE, sizeof(HuffmanNode), cmpfunc);
	int stop = BYTE_RANGE;
	while (file_ranking[stop].weight > 0) stop--;

	memcpy(node_buf,file_ranking + stop + 1, stop * sizeof(HuffmanNode));
	node_count = BYTE_RANGE - stop - 1;
	
	/* for (int i = 0; i < BYTE_RANGE; i++){
		printf("HuffmanByte { '%c', %d }\n",file_ranking[i].ch,file_ranking[i].weight);
	}
	printf("\n--------------\n\n");
	for (int i = 0; i < node_count; i++){
		printf("HuffmanByte { '%c', %d }\n",node_buf[i].ch,node_buf[i].weight);
	} */
}

// multiple passes to coalesce these nodes into one root
void parse_tree(int left, int right){
	assert(left >= 0);
	assert(right < node_count);

	printf("parsing tree %d to %d\n",left,right);
	for (;;) {
		if (left + 2 < right) {
			HuffmanNode *r = &node_buf[left++];
			HuffmanNode *l = &node_buf[left++]; 

			HuffmanNode mix;
			mix.is_internal_node = true;
			mix.weight = l->weight + r->weight;
			mix.left = l;
			mix.right = r;

			node_buf[node_count++] = mix;
			assert(node_count < TREE_CAP);
		} else {
			break;
		}
	}
}

HuffmanNode *resolve_root(){
	HuffmanNode *largest = &node_buf[0];
	for (int i = 0; i < node_count; i++){
		if (node_buf[i].weight > largest->weight){
			largest = &node_buf[i];
		}
	}
	if (node_odd != NULL){
		assert(false);
		// copy of above, macro them?
		/* HuffmanNode *l = largest;
		HuffmanNode *r = node_odd;

		HuffmanNode mix;
		mix.is_internal_node = true;
		mix.weight = r->weight + l->weight;
		mix.left = l;
		mix.right = r;

		node_buf[node_count] = mix;
		largest = &node_buf[node_count];
		node_count++;
		assert(node_count < TREE_CAP); */
	}
	return largest;
}

#define SPACES(count) printf("%*s", count, "")

void traverse_print(HuffmanNode *root, int level){
	if (root->left->is_internal_node){
		SPACES(level);
		printf("[ %d ]\n",root->left->weight);
		traverse_print(root->left,level+2);
	} else {
		SPACES(level);
		printf("{ '%c' - %d }\n",root->left->ch,root->left->weight);
	}

	if (root->right->is_internal_node){
		SPACES(level);
		printf("[ %d ]\n",root->right->weight);
		traverse_print(root->right,level+2);
	} else {
		SPACES(level);
		printf("{ '%c' - %d }\n",root->right->ch,root->right->weight);
	}
}

int main(){
	rank_file("huffman/huffman_text.txt");
	size_t size = 0;
	for (int i = 0; i < node_count; i++){
		size += node_buf[i].weight;
	}
	printf("------ %zu, from %d nodes ------\n", size, node_count);
	int c1 = 0;
	int c2 = node_count - 1;

	if (node_count % 2 != 0) {
		node_odd = &node_buf[0];
		c1++;
	}

	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;
	parse_tree(c1, c2);
	c1 = c2;
	c2 = node_count - 1;

	for (int i = 0; i < node_count; i++){
		if (!node_buf[i].is_internal_node) {
			printf("HuffmanNode { '%c', %d }\n",node_buf[i].ch,node_buf[i].weight);
		} else {
			printf("HuffmanNode [ %d ]\n",node_buf[i].weight);
		}
	}

	printf("\n--------------\n\n");
	
	HuffmanNode *root = resolve_root();

	printf("[ %d ]\n",root->weight);
	traverse_print(root,2);
}