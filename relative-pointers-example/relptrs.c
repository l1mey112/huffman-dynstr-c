#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

typedef struct Node Node;
struct Node {
	Node *next;
	uint16_t data;
};

static Node node_pool[1024];
static int len = 0;

Node *new_node(uint16_t data){
	Node *place = node_pool + len;
	place->data = data;
	len++; assert(len < 1024);
	return place;
}

void iter_node(Node *root){
	do    printf("%zu -> %u\n",(void*)root - (void*)node_pool, root->data);
	while (root = root->next);
}

/* convert to relative pointers only when writing by making it
   relative to a buffer instead of messing with relative 
   pointers the entire time. it's simpler this way             */

int main(){
	Node *n1 = new_node(15);
	Node *n2 = new_node(10);
	Node *n3 = new_node(5);

	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;

	iter_node(n1);
}