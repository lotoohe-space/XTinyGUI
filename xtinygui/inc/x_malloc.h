#pragma once

#ifndef _X_MALLOC_H__
#define _X_MALLOC_H__

#include "x_types.h"

void *XMalloc(uint32_t size);
void XFree(void *mem);

#endif
