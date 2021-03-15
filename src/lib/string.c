/**
 * string.c -- libc string implementation
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 */

#include "string.h"

size_t strlen(const char *s) {
	size_t i = 0;
	while (*s++ != '\0')
		i++;

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

void reverse(char *s, int len) {
	char *p1 = s;
	char *p2 = s + len - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1++ = *p2;
		*p2-- = tmp;
	}
}

char *itoa(int n) {
	static char text[32];

	int i, sign;
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		text[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		text[i++] = '-';
	text[i] = '\0';
	reverse(text, strlen(text));

	return text;
}

char *utoa(uint64_t n) {
	static char text[20];
	int i = 0;

	text[19] = 0;	/* null terminator */
	do {
		--i;
		text[i] = (n % 10)+'0';
		n /= 10;
	} while (n);

	return &text[i];
}