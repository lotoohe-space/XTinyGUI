
#include "x_malloc.h"
#include <stdlib.h>

void *XMalloc(uint32_t size)
{
	return malloc(size);
}

void XFree(void *mem)
{
	free(mem);
}