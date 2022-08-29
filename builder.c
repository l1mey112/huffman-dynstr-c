#include "strings.h"

str_builder builder_make(uint32_t size){
	MSG("created str builder with size %u",size);
	return (str_builder){
		.data = malloc(size),
		.len  = 0,
		.cap  = size
	};
}

void builder_ensure_cap(str_builder *builder, uint32_t len){
	if (builder->len + len > builder->cap) {
		builder->cap *= 2;
		if (builder->cap < builder->len + len)
			builder->cap = builder->len + len;
		builder->data = realloc(builder->data, builder->cap);
		MSG("additional %u bytes (new cap: %u)",len, builder->cap);
	}
}

void builder_append(str_builder *builder, string a){
	MSG("appending str (old len: %u, new len: %u)", builder->len, builder->len + a.len);
	builder_ensure_cap(builder, a.len);
	memcpy(builder->data + builder->len, a.cstr, a.len);
	builder->len += a.len;
}

void builder_append_cstr(str_builder *builder, const char *a){
	MSG("appending str (old len: %u, new len: %u)", builder->len, builder->len + a.len);
	uint32_t len = strlen(a);
	builder_ensure_cap(builder, len);
	memcpy(builder->data + builder->len, a, len);
	builder->len += len;
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