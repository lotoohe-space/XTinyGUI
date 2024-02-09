#pragma once

#ifndef _X_MALLOC_H__
#define _X_MALLOC_H__

#include "type.h"

void *xMalloc(uint32_t size);
void xFree(void *mem);

#endif
