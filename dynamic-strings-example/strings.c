#include "strings.h"

void string_free(string *a){
	if (a->is_literal) return;

	assert(a->is_literal != 1717171717);
	free(a->cstr);
	a->is_literal = 1717171717;
}

string string_concat(string a, string b){
	int len = a.len + b.len;
	
	string ret = (string) {
		.cstr = malloc(len + 1),
		.len = len,
		.is_literal = 0
	};

	memcpy(ret.cstr        , a.cstr, a.len);
	memcpy(ret.cstr + a.len, b.cstr, b.len);

//	string_free(&a);
//	string_free(&b);

	ret.cstr[len] = 0;
	return ret;
}

/* string string_split_first_on(string a, string delimit){
	
} */

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
string string_substr(string a, int start, int end){
	assert(start < end); // also makes sure len > 0
	start = MAX(start, 0);
	end = MIN(end, a.len);
	int len = end - start;
	string ret = (string) {
		.cstr = malloc(len + 1),
		.len = len,
		.is_literal = 0
	};
	memcpy(ret.cstr, a.cstr + start, len);
	ret.cstr[len] = 0;
	return ret;
}

static inline bool is_whitespace(char c){
	return c == '\t' || c == ' ' || (c > 8 && c < 14) || (c == 0x85) || (c == 0xa0);
}

string string_trim_all_whitespace(string a){
	int left = 0;
	int right = a.len;

	for (char *ptr = a.cstr; is_whitespace(*ptr); ptr++)
		left++;
	for (char *ptr = a.cstr + a.len - 1; is_whitespace(*ptr); ptr--)
		right--;

	return string_substr(a, left, right);
}

bool string_equals(string a, string b){
	if (a.len != b.len) return false;

	for (int i = 0; i < a.len; i++){
		if (a.cstr[i] != b.cstr[i]) return false;
	}
	return true;
}

bool string_contains(string str, string substr){
	if (substr.len == 0) return true;

	int i = 0;
	while (i < str.len)
	{
		int j = 0;
		// march along the substr for every letter on the string
		while (j < substr.len && str.cstr[i + j] == substr.cstr[j])
			j++;
		
		if (j == substr.len) {
			return true;
		}
		i++;
	}
	return false;
}