#ifndef _MEM_DEV_H__
#define _MEM_DEV_H__

#include "type.h"
#include "widge_define.h"


typedef struct {

	XRECT	rect;		/*存储设备的大小区域*/
	XPOINT	rawPoint;	/*原始起始位置*/
	uint8*	mem;		/*内存空间*/
	uint32	memSize;	/*内存大小*/

	uint8	colorDepth;	/*颜色深度*/

}*HMEM_DEV,MEM_DEV;

HMEM_DEV	MemDevCreate(int16 x, int16 y, uint16 w, uint16 h, uint8 colorDepth);
void		MemDevClear(HMEM_DEV hMemDev, uintColor color);
uint8		MemDevReset(HMEM_DEV hMemDev, int16 x, int16 y, uint16 w, uint16 h);
void		MemDevDrawPT(HMEM_DEV hMemDev, int16 x, int16 y, uintColor color);
uintColor	MemDevReadPT(HMEM_DEV hMemDev, int16 x, int16 y);
void		MemDevCopyToLCD(HMEM_DEV hMemDev);

#endif
