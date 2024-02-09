
#include "x_malloc.h"
#include <stdlib.h>

void *xMalloc(uint32_t size)
{
	return malloc(size);
}

void xFree(void *mem)
{
	free(mem);
}