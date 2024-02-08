
#include "x_malloc.h"
#include <stdlib.h>

void* xMalloc(uint32 size) {
	return malloc(size);
}

void xFree(void* mem) {
	free(mem);
}