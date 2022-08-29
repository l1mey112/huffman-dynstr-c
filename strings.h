#ifndef _H_DYNAMIC_STRINGS
#define _H_DYNAMIC_STRINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

//#define MSG(a, args...) fprintf(stderr, "[%s:%d: %s]: \x1b[92m"a"\x1b[39m\n",__FILE__,__LINE__,__func__,args)
#define MSG(a, args...)

typedef struct {
	char *data;
	uint32_t len;
	uint32_t cap;
} str_builder;

typedef struct {
	char *cstr;
	uint32_t len;
	int is_literal;
} string;

#define slit(s) (string){(char *)s, sizeof(s)-1, 1}

void string_free(string *a);
string string_concat(string a, string b);
bool string_contains(string str, string substr);
bool string_equals(string a, string b);
string string_trim_all_whitespace(string a);

str_builder builder_make(uint32_t size);
void builder_ensure_cap(str_builder *builder, uint32_t len);
void builder_append(str_builder *builder, string a);
void builder_append_cstr(str_builder *builder, const char *a);
string builder_tostr(str_builder *builder);

#endif