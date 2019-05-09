#include "string_utils.h"

char * copy_string(const char * string) {
	size_t size = strlen(string) + 1;
	char * copy = malloc(size*sizeof(char));
	memcpy(copy, string, size);
	return copy;
}