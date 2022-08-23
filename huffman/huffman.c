#include "strings.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	uint8_t  data;
	uint32_t count;
} HuffmanByte;

typedef struct {
	HuffmanByte *buffer;
	size_t size;
} HuffmanRanking;

#define BYTE_RANGE 256
HuffmanByte file_ranking[BYTE_RANGE] = {0};

void print_ranks(){
	for (int i = 0; i < BYTE_RANGE; i++){
		printf("HuffmanByte { '%c', %d }\n",file_ranking[i].data,file_ranking[i].count);
	}
}

int cmpfunc (const HuffmanByte * a, const HuffmanByte * b) {
   return b->count - a->count;
}

HuffmanRanking rank_file(const char *filepath){
	uint8_t FILEBUFFER[1024];
	
	FILE *fp = fopen("huffman/huffman_text.txt", "r");
	assert (fp);
	fread(FILEBUFFER, 1024, 1, fp);

	uint8_t *p = FILEBUFFER;
	for (;;){
		uint8_t ch = *p;
		if (ch == 0){
			break;
		}
		file_ranking[ch].data = ch;
		file_ranking[ch].count++;
		p++;
	}
	qsort(file_ranking, BYTE_RANGE, sizeof(HuffmanByte), cmpfunc);
	int stop = 0;
	while 
	return (HuffmanRanking){
		file_ranking,

	}
}

int main(){
	
	print_ranks();
}