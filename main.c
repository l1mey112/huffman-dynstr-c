#include "huffman.h"
#include "strings.h"

extern HuffmanNode buffer[1024];
extern unsigned int amt;

void global_buffer_make_relative(){
	for (int i = 0; i < amt; i++)
	{
		if (buffer[i].left)
			buffer[i].left = (HuffmanNode*)((size_t)buffer[i].left - (size_t)buffer);

		if (buffer[i].right)
			buffer[i].right = (HuffmanNode*)((size_t)buffer[i].right - (size_t)buffer);
	}
}

void global_buffer_make_absolute(){
	for (int i = 0; i < amt; i++)
	{
		if (buffer[i].left)
			buffer[i].left = (HuffmanNode*)((size_t)buffer[i].left + (size_t)buffer);

		if (buffer[i].right)
			buffer[i].right = (HuffmanNode*)((size_t)buffer[i].right + (size_t)buffer);
	}
}

/* file.txt
   file.txt.hcoding
   file.txt.hkey    */

void serialise(string file_p){
    Buffer file = open_and_read_bytes(file_p.cstr);
    HuffmanNode *root = huffman_rank(file);

    BitArray b = bitarray_new(200);
	HuffmanMapEntry map[256];
	walk_huffman(map, root, 0, 0);
    encode_huffman(map, file, &b);

    FILE *h_key = fopen(string_concat(file_p,slit(".hkey")).cstr, "w");
	assert(h_key && "failed to open file!");
    FILE *h_coding = fopen(string_concat(file_p,slit(".hcoding")).cstr, "w");
    assert(h_coding && "failed to open file!");

    global_buffer_make_relative();

    unsigned int whereis = (unsigned int)((void*)root - (void*)buffer);
    fwrite(&amt,1,sizeof(amt),h_key);
    fwrite(&whereis,1,sizeof(unsigned int),h_key);
    fwrite(buffer,amt,sizeof(HuffmanNode),h_key);

    fwrite(&(b.bitlen),sizeof(size_t),1,h_coding);
    fwrite(b.data,sizeof(uint8_t),b.idx,h_coding);

    fclose(h_key);
    fclose(h_coding);

    free(file.data);
	free(b.data);
}

void walk_huffman_print(HuffmanNode *root, uint16_t data, uint8_t len) {
    if (root->left) {
        walk_huffman_print(root->left, data, len + 1);
    }
 
    if (root->right) {
		data |= 1 << len;
        walk_huffman_print(root->right, data, len + 1);
    }

	if (!(root->left) && !(root->right)) {
		uint8_t mask = 0;
		while (len--)
		    putchar(data & 1 << mask++ ? '1' : '0');
            printf(" - %c",root->ch == '\n' ? '_' : root->ch);
		putchar('\n');
	}
}

#define roundup8(x) ((x + 7) & (-8))

void deserialise(string file_name){
    FILE *h_key = fopen(string_concat(file_name,slit(".hkey")).cstr, "r");
	assert(h_key && "failed to open file!");
    FILE *h_coding = fopen(string_concat(file_name,slit(".hcoding")).cstr, "r");
    assert(h_coding && "failed to open file!");

    unsigned int rel_root;

    fread(&amt,sizeof(unsigned int),1,h_key);
    fread(&rel_root,sizeof(unsigned int),1,h_key);
    fread(buffer,sizeof(HuffmanNode),amt,h_key);
    global_buffer_make_absolute();

    HuffmanNode *root = (HuffmanNode*)(((void*)buffer) + rel_root);

    size_t bitlen;
    fread(&bitlen,sizeof(size_t),1,h_coding);
    size_t bytes = bitlen / 8;

    BitArray b;
    b.data = malloc(bytes);
	assert(b.data);
    
    b.bitlen = bitlen;
    b.cap = b.idx = bytes;
    fread(b.data,sizeof(uint8_t),b.idx,h_coding);

    FILE *h_file = fopen(file_name.cstr, "w");
	assert(h_file && "failed to open file!");

    decode_huffman(root, &b, h_file);

    fclose(h_key);
    fclose(h_coding);
    fclose(h_file);
    free(b.data);
}

int main1(){
    serialise(slit("huffman_text.txt"));
    deserialise(slit("huffman_text.txt"));
}

int main(){
    string hello = slit("Hello");
    hello = string_concat(hello, slit(" world!\n"));

    Buffer file;
    file.data = hello.cstr;
    file.len = hello.len;

    HuffmanNode *root = huffman_rank(file);

    BitArray b = bitarray_new(50);
	HuffmanMapEntry map[256];
	walk_huffman(map, root, 0, 0);
    encode_huffman(map, file, &b);

    printf("string: %s",hello.cstr);
    walk_huffman_print(root,0,0);
    bitarray_print(&b);
}