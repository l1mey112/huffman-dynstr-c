#include "strings.h"

str_builder builder_make(int size){
	MSG("created str builder with size %d",size);
	return (str_builder){
		.data = malloc(size),
		.len  = 0,
		.cap  = size
	};
}

void builder_ensure_cap(str_builder *builder, int len){
	if (builder->len + len > builder->cap) {
		builder->data = realloc(builder->data, builder->len + len);
		MSG("additional %d bytes (old cap: %d, new cap: %d)",len, builder->cap, builder->len + len);
		builder->cap  = builder->len + len;
	}
}

void builder_append(str_builder *builder, string a){
	MSG("appending str (old len: %d, new len: %d)", builder->len, builder->len + a.len);
	builder_ensure_cap(builder, a.len);
	memcpy(builder->data + builder->len, a.cstr, a.len);
	builder->len += a.len;
}

string builder_tostr(str_builder *builder){
	string ret = {0};
	ret.len  = builder->len;
	ret.cstr = malloc(builder->len + 1);
	assert(ret.cstr);
	ret.cstr[builder->len] = 0;
	
	memcpy(ret.cstr, builder->data, builder->len);
	free(builder->data);
	
	MSG("returned a string from a builder, freed builder",0);
	return ret;
}