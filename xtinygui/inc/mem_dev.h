#ifndef _MEM_DEV_H__
#define _MEM_DEV_H__

#include "x_types.h"
#include "widget_define.h"

typedef struct
{
	xrect_t rect;		 /*存储设备的大小区域*/
	xpoint_t rawPoint; /*原始起始位置*/
	uint8_t *mem;		 /*内存空间*/
	uint32_t memSize;	 /*内存大小*/

	uint8_t colorDepth; /*颜色深度*/
} *p_mem_dev_t, mem_dev_t;

p_mem_dev_t MemDevCreate(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t colorDepth);
void MemDevClear(p_mem_dev_t hMemDev, uintColor color);
uint8_t MemDevReset(p_mem_dev_t hMemDev, int16_t x, int16_t y, uint16_t w, uint16_t h);
void MemDevDrawPT(p_mem_dev_t hMemDev, int16_t x, int16_t y, uintColor color);
uintColor MemDevReadPT(p_mem_dev_t hMemDev, int16_t x, int16_t y);
void MemDevCopyToLCD(p_mem_dev_t hMemDev);

#endif
