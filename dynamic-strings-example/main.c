#include "strings.h"

int main(){
	str_builder str = builder_make(4);
	builder_append(&str, slit("Hello "));
	builder_append(&str, slit("world!\n"));
	
	string ret = builder_tostr(&str);
	printf("string = %s",ret);

	if (string_contains(ret, slit("world"))){
		puts("string contains the substring 'world'");
	}

	if (string_equals(ret, slit("Hello world!\n"))){
		puts("string equals the string 'Hello world!\\n'");
	}

	string whsp_gone = string_trim_all_whitespace(slit("\n hello\n "));
	printf("whitespace trim '%s'\n",whsp_gone.cstr);

	free(ret.cstr);
	return 0;
}