#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
	char  *str;
	int   len;
	int   is_literal;
} dynstr;

#define _dynstr_lit(s) (dynstr){(char *)s, sizeof(s)-1, 1}

static inline void free_dynstr(dynstr *a){
	assert(a->is_literal != 1717171717);
	// double free detected

	if (a->is_literal != 1) {
		free(a->str);
		a->is_literal = 1717171717;
	}
}

dynstr add(dynstr a, dynstr b){
	int len = a.len + b.len;
	
	dynstr ret = (dynstr) {
		.str = malloc(len + 1),
		.len = len,
		.is_literal = 0
	};

	memcpy(ret.str        , a.str, a.len);
	memcpy(ret.str + a.len, b.str, b.len);

	free_dynstr(&a);
	free_dynstr(&b);

	ret.str[len] = 0;
	return ret;
}

int main(){
	dynstr a = _dynstr_lit("Hello ");
	dynstr b = _dynstr_lit("world!\n");

	dynstr c = add(a,b);

	printf("%s",c);
	printf("%d\n",c.is_literal);

	return 0;
}