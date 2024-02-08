#include "mem_dev.h"
#include "x_malloc.h"
#include "tool.h"

#include "GUI_interface_extern.h"

/*创建一个存储设备*/
HMEM_DEV	MemDevCreate(int16 x, int16 y, uint16 w, uint16 h, uint8 colorDepth) {
	HMEM_DEV hMemDev = xMalloc(sizeof(MEM_DEV));
	if (hMemDev == NULL) { return NULL; }
	hMemDev->mem = (uint8*)xMalloc(w * h * (colorDepth / 8));
	if (hMemDev->mem == NULL) {
		xFree(hMemDev);
	}
	hMemDev->rect.x = x;
	hMemDev->rect.y = y;
	hMemDev->rect.w = w;
	hMemDev->rect.h = h;
	hMemDev->memSize = w * h * (colorDepth / 8);

	hMemDev->rawPoint.x = x;
	hMemDev->rawPoint.y = y;

	hMemDev->colorDepth = colorDepth;

	return hMemDev;
}
/*重新设置MemDev的大小，*/
uint8 MemDevReset(HMEM_DEV hMemDev, int16 x, int16 y, uint16 w, uint16 h) {
	if (hMemDev == NULL) { return FALSE; }
	if (w == 0 || h == 0) {
		return FALSE;
	}
	hMemDev->rect.x = x;
	hMemDev->rect.y = y;
	hMemDev->rect.w = w;
	hMemDev->rect.h = MIN(hMemDev->memSize / w / (hMemDev->colorDepth / 8), h);/*重新设置高度，向下取整*/

	if (hMemDev->rect.h == 0) {
		/*高度为零，设置失败*/
		return FALSE;
	}
	return TRUE;
}
void MemDevClear(HMEM_DEV hMemDev, uintColor color) {
	if (hMemDev == NULL) { return; }
	int16 i, j;
	for (j = hMemDev->rect.y; j < hMemDev->rect.y + hMemDev->rect.h; j++) {
		for (i = hMemDev->rect.x; i < hMemDev->rect.x + hMemDev->rect.w; i++) {
			((uint16*)(hMemDev->mem))[(i - hMemDev->rect.x) + (j - hMemDev->rect.y) * hMemDev->rect.w] = color;
		}
	}
}
void MemDevDrawPT(HMEM_DEV hMemDev, int16 x, int16 y, uintColor color) {
	if (hMemDev == NULL) { return; }
	((uint16*)(hMemDev->mem))[x + y * hMemDev->rect.w] = (uint16)color;
}
uintColor MemDevReadPT(HMEM_DEV hMemDev, int16 x, int16 y) {
	if (hMemDev == NULL) { return 0x0000; }
	return ((uint16*)(hMemDev->mem))[x + y * hMemDev->rect.w];
}

void MemDevCopyToLCD(HMEM_DEV hMemDev) {
	if (hMemDev == NULL) { return; }
	GUIDrawBitmap(hMemDev->rect.x, hMemDev->rect.y, hMemDev->rect.w, hMemDev->rect.h, hMemDev->mem);
}
