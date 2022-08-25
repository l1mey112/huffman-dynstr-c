#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// this is a min heap example using integers...

typedef struct {
	size_t len;
	size_t cap;

	int *data;
} MinHeap;

MinHeap *new_heap(size_t cap)
{
	MinHeap *h = malloc(sizeof(MinHeap));
	h->len = 0;
	h->cap = cap;
	h->data = malloc(sizeof(int) * cap);

	return h;
}

void heap_init_data(MinHeap *h, int src[], size_t n)
{
	assert(h->cap <= n);
	memcpy(h->data,src, sizeof(int) * n);
	h->len = n;
}

void heapify(MinHeap *h, int i)
{
	int smallest = i;
	int leftChild = 2 * i + 1;
	int rightChild = leftChild + 1;

	if (leftChild < h->len && h->data[leftChild] < h->data[smallest])
		smallest = leftChild;
	
	if (rightChild < h->len && h->data[rightChild] < h->data[smallest])
		smallest = rightChild;
	
	if (smallest != i)
	{
		// swap
		int tmp = h->data[i];
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

static inline void heap_grow(MinHeap *h, size_t grow){
	h->len += grow;
	if (h->len > h->cap){
		printf("[] heap grew! - cap was %zu, is now %zu\n",h->cap, h->cap * (size_t)2);
		h->cap *= 2;
		h->data = realloc(h->data, h->cap * sizeof(int));
	}
}

void heap_push(MinHeap *h, int a)
{
    heap_grow(h, 1);
    int i = h->len - 1;
 
    while (i != 0 && a < h->data[(i - 1) / 2]) 
	{
        h->data[i] = h->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    h->data[i] = a;
}

int heap_pop(MinHeap *h)
{
    int tmp = h->data[0];
    h->data[0] = h->data[h->len - 1];
 
    h->len--;
    heapify(h, 0);
 
    return tmp;
}

#define SPACES(count) printf("%*s", count, "")

static void heap_print_recurse(MinHeap *h, int i, int level)
{
	int leftChild = 2 * i + 1;
	int rightChild = leftChild + 1;

	if (leftChild < h->len){
		SPACES(level); 
		printf("%d\n",h->data[leftChild]);
		heap_print_recurse(h, leftChild, level+2);
	}
	if (rightChild < h->len){
		SPACES(level); 
		printf("%d\n",h->data[rightChild]);
		heap_print_recurse(h, rightChild, level+2);
	}
}

void heap_print(MinHeap *h){
	printf("%d\n",h->data[0]);
	heap_print_recurse(h, 0, 2);
}

int main(){
	int arr[] = {4, 18, 17, 10, 19, 20, 14, 8, 3, 12};
	int len = sizeof(arr) / sizeof(int);

	MinHeap *h = new_heap(len);
	heap_init_data(h, arr, len);
	heapify_all(h);

	heap_push(h, 111);
	heap_push(h, 112);
	heap_push(h, 113);
	heap_push(h, 114);

	heap_print(h);

	printf("data popped! %d\n",heap_pop(h));
	printf("data popped! %d\n",heap_pop(h));
	printf("data popped! %d\n",heap_pop(h));

	heap_print(h);
}