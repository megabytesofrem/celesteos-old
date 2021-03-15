#ifndef LIB_STRING_H
#define LIB_STRING_H
#include <common.h>

size_t strlen(const char *s);
char* strcpy(char *dest, const char *src);
char* strcat(char *dest, const char *src);

void* memset(void *s, int c, size_t n);

char *itoa(int n);
char *utoa(uint64_t n);
void reverse(char *s, int len);

#endif