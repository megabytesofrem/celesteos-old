#include <common.h>
#include "string.h"

size_t strlen(const char *s) {
	size_t i = 0;
	while (*s != '\0') {
		s++; i++;
	}

	return i;
}

char *strcpy(char *dest, const char *src) {
	char *saved = dest;

	while (*src)
		*dest++ = *src++;
	*dest = 0;
	return saved;
}

char *strcat(char *dest, const char *src) {
	char *rdest = dest;
	while (*dest) dest++;

	while (*dest++ = *src++) ;
	return rdest;
}

void *memset(void *s, int c, size_t n) {
	unsigned char *p = s;
	for (size_t i = 0; i < n; i++)
		p[i] = c;

	// shit memset
	return p;
}