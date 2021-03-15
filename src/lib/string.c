/**
 * string.c -- string library functions
 * 
 * Authors:
 * 		Charlotte (charpointer)
 * 		Harry (harrego)
 * 	
 * With parts taken from the Linux kernel sources
 */
#include "string.h"

/**
 * strlen - returns the length of the string s
 * @param s: the string to measure
 */
size_t strlen(const char *s) {
	size_t i = 0;
	while (*s++ != '\0')
		i++;

	return i;
}

/**
 * strcat - concatenate two NULL terminated strings together
 * @param dest: the destination string
 * @param src: the source string
 */
char *strcat(char *dest, const char *src) {
	char *rdest = dest;
	while (*dest) dest++;

	while (*dest++ = *src++) ;
	return rdest;
}

/**
 * strcpy - copy a NULL terminated string src into dest
 * @param dest: the destination string
 * @param src: the source string to copy
 */
char *strcpy(char *dest, const char *src) {
	char *saved = dest;

	while (*src)
		*dest++ = *src++;
	*dest = 0;
	return saved;
}

char *strncpy(char *dest, const char *src, size_t count) {
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}

	return dest;
}

/**
 * strrev - reverses a string (in place)
 * 
 */
void strrev(char *s, int len) {
	char *p1 = s;
	char *p2 = s + len - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1++ = *p2;
		*p2-- = tmp;
	}
}

/**
 * memset - fill a region of memory with the given value
 */
void *memset(void *s, int c, size_t count) {
	char *tmp = s;
	while (count--)
		*tmp++ = c;
	return tmp;
}

/**
 * memcpy - copy an area of memory to another
 * @param dest: where to copy to
 * @param src: the source area of memory to copy from
 * @param count: the size of the area
 */
void *memcpy(void *dest, const void *src, size_t count) {
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}

/* non standard conversion functions */

char *itoa(int n, int base) {
	static char text[32];

	int i, sign;
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		text[i++] = n % base + '0';
	} while ((n /= base) > 0);
	if (sign < 0)
		text[i++] = '-';
	text[i] = '\0';
	strrev(text, strlen(text));

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