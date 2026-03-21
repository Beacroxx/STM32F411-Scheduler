#ifndef _KMALLOC_H
#define _KMALLOC_H

#include <stddef.h>

int kmalloc_init();

void* kmalloc(size_t size);
void kfree(void* addr);

#endif