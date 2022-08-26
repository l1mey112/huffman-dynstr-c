#include "huffman.h"

MinHeap *new_heap(size_t cap)
{
	MinHeap *h = malloc(sizeof(MinHeap) + sizeof(HuffmanNode*) * cap);
	h->data = (void*)h + sizeof(MinHeap);
	
	// free(minheap);
	// instead of...
	// free(minheap->data); free(minheap); 

	h->len = 0;
	h->cap = cap;

	return h;
}

static void heapify(MinHeap *h, int i)
{
	int smallest = i;
	int leftChild = 2 * i + 1;
	int rightChild = leftChild + 1;

	if (leftChild < h->len && h->data[leftChild]->weight < h->data[smallest]->weight)
		smallest = leftChild;
	
	if (rightChild < h->len && h->data[rightChild]->weight < h->data[smallest]->weight)
		smallest = rightChild;
	
	if (smallest != i)
	{
		// swap
		HuffmanNode *tmp = h->data[i];
		h->data[i] = h->data[smallest];
		h->data[smallest] = tmp;

		heapify(h, smallest);
	}
}

void heapify_all(MinHeap *h)
{
	int n = h->len - 1;

	// save computation time by only executing what is necessary
	for (int i = (n - 1) / 2; i >= 0; i--)
	{
		heapify(h, i);
	}
}

static inline void heap_ensure_cap(MinHeap *h, size_t cap){
	if (h->len + cap > h->cap){
		printf("[] heap grew! - cap was %zu, is now %zu\n",h->cap, h->cap * (size_t)2);
		h->cap *= 2;
		h = realloc(h, h->cap * sizeof(HuffmanNode*) + sizeof(MinHeap));
	}
}

void heap_push(MinHeap *h, HuffmanNode *a)
{
    heap_ensure_cap(h, 1);
	h->len++;
    int i = h->len - 1;
 
    while (i != 0 && a->weight < h->data[(i - 1) / 2]->weight) 
	{
        h->data[i] = h->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    h->data[i] = a;
}

HuffmanNode *heap_pop(MinHeap *h)
{
    HuffmanNode *tmp = h->data[0];
    h->data[0] = h->data[h->len - 1];
 
    h->len--;
    heapify(h, 0);
 
    return tmp;
}