#ifndef LIB_KERNEL_H
#define LIB_KERNEL_H
#include <common.h>

/* should all output be logged to port 0xE9? */
#define LOG_TO_E9 1

enum klog_level {
	KLOG_NONE,
	KLOG_INFO,
	KLOG_WARN,
	KLOG_ERROR
};

void kputs(char *s);
void printk(const char *fmt, ...);
void klog(enum klog_level level, const char *fmt, ...);

#endif