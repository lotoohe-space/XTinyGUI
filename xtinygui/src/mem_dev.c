#include "mem_dev.h"
#include "x_malloc.h"
#include "x_tool.h"

#include "GUI_interface_extern.h"

/*创建一个存储设备*/
p_mem_dev_t MemDevCreate(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t colorDepth)
{
	p_mem_dev_t hMemDev = xMalloc(sizeof(mem_dev_t));
	if (hMemDev == NULL)
	{
		return NULL;
	}
	hMemDev->mem = (uint8_t *)xMalloc(w * h * (colorDepth / 8));
	if (hMemDev->mem == NULL)
	{
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
uint8_t MemDevReset(p_mem_dev_t hMemDev, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	if (hMemDev == NULL)
	{
		return FALSE;
	}
	if (w == 0 || h == 0)
	{
		return FALSE;
	}
	hMemDev->rect.x = x;
	hMemDev->rect.y = y;
	hMemDev->rect.w = w;
	hMemDev->rect.h = MIN(hMemDev->memSize / w / (hMemDev->colorDepth / 8), h); /*重新设置高度，向下取整*/

	if (hMemDev->rect.h == 0)
	{
		/*高度为零，设置失败*/
		return FALSE;
	}
	return TRUE;
}
void MemDevClear(p_mem_dev_t hMemDev, uintColor color)
{
	if (hMemDev == NULL)
	{
		return;
	}
	int16_t i, j;
	for (j = hMemDev->rect.y; j < hMemDev->rect.y + hMemDev->rect.h; j++)
	{
		for (i = hMemDev->rect.x; i < hMemDev->rect.x + hMemDev->rect.w; i++)
		{
			((uint16_t *)(hMemDev->mem))[(i - hMemDev->rect.x) + (j - hMemDev->rect.y) * hMemDev->rect.w] = color;
		}
	}
}
void MemDevDrawPT(p_mem_dev_t hMemDev, int16_t x, int16_t y, uintColor color)
{
	if (hMemDev == NULL)
	{
		return;
	}
	((uint16_t *)(hMemDev->mem))[x + y * hMemDev->rect.w] = (uint16_t)color;
}
uintColor MemDevReadPT(p_mem_dev_t hMemDev, int16_t x, int16_t y)
{
	if (hMemDev == NULL)
	{
		return 0x0000;
	}
	return ((uint16_t *)(hMemDev->mem))[x + y * hMemDev->rect.w];
}

void MemDevCopyToLCD(p_mem_dev_t hMemDev)
{
	if (hMemDev == NULL)
	{
		return;
	}
	GUIDrawBitmap(hMemDev->rect.x, hMemDev->rect.y, hMemDev->rect.w, hMemDev->rect.h, hMemDev->mem);
}
