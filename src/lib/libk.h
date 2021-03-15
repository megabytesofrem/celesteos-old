#ifndef LIB_KERNEL_H
#define LIB_KERNEL_H
#include <common.h>

/* should all output be logged to port 0xE9? */
#define LOG_TO_E9 1

typedef enum {
	KLOG_INFO,
	KLOG_WARN,
	KLOG_ERROR
} klog_level;

void kputs(char *s);
void klog(klog_level level, const char *s);

#endif