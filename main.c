#include <stdio.h>

typedef struct {
	char  *str;
	int   len;
	int   is_literal;
} dynstr;

#define _dynstr(s) (dynstr){(char *)s, sizeof(s)-1, 1}

int main(){
	dynstr a = _dynstr("HELLO!");

	printf("sizeof(dynstr) = %u\n"   , sizeof(dynstr));
	printf("           str = '%s'\n" , a.str);
	printf("           len = %d\n"   , a.len);
	printf("    is literal = %s\n"   , a.is_literal ? "true" : "false");

	return 0;
}