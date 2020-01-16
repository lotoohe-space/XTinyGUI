
#include "x_malloc.h"
#include "z_malloc.h"
#include <stdlib.h>
void* xMalloc(uint32 size) {
	return t_malloc(0,size);
}

void xFree(void* mem) {
	t_free(0, mem);
}