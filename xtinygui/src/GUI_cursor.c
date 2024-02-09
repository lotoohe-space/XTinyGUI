#include "GUI_cursor.h"
#include "color.h"
#include "GUI_interface_extern.h"
#include "paint.h"
#include "x_malloc.h"

/* 7*12 */
const uint32_t GUICursorPT[] = {
	0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x00000000, 0x00FFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x00000000, 0x00FFFFFF, 0x00FFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x00000000, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
	0x00000000, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00000000, 0xFFFFFFFF,
	0x00000000, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00000000,
	0x00000000, 0x00FFFFFF, 0x00FFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00FFFFFF, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
	0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000};

/*游标*/
gui_cursor_t GUICursor = {
	7,
	12,
	(uint8_t *)GUICursorPT,
	0,
	32,
	BITMAP_DRAW_ARGB8888,
	NULL,
	0, 0,
	0};

/*GUI游标初始化*/
uint8_t GUICursorInit(void)
{
	GUICursor.GUICursorMem = xMalloc(
		GUICursor.GUICursorPTBitmap.w * GUICursor.GUICursorPTBitmap.h * (GUI_COLOR_DEEP / 8));
	if (GUICursor.GUICursorMem == NULL)
	{
		_SET_CURSOR_HIDE(&GUICursor);
		_SET_CURSOR_CLOSE(&GUICursor);
		return FALSE;
	}
	GUICursor.GUICursorLastPOIMem = xMalloc(
		GUICursor.GUICursorPTBitmap.w * GUICursor.GUICursorPTBitmap.h * (GUI_COLOR_DEEP / 8));
	if (GUICursor.GUICursorMem == NULL)
	{
		_SET_CURSOR_HIDE(&GUICursor);
		_SET_CURSOR_CLOSE(&GUICursor);
		xFree(GUICursor.GUICursorMem);
		return FALSE;
	}
	GUICursor.lastCursorPOI.x = 0;
	GUICursor.lastCursorPOI.y = 0;
	GUICursor.cursorPOI.x = 0;
	GUICursor.cursorPOI.y = 0;
	_SET_CURSOR_SHOW(&GUICursor);
	_SET_CURSOR_OPEN(&GUICursor);

	_SET_CURSOR_FIRST(&GUICursor);
	return TRUE;
}
/*保存当前位置的背景到缓存*/
void GUICursorLastLCDCopyToMem(void)
{
	int16_t i, j;
	int16_t x, y;
	x = GUICursor.cursorPOI.x;
	y = GUICursor.cursorPOI.y;
	/*获取当前位置的背景图片*/
	for (i = 0; i < GUICursor.GUICursorPTBitmap.h; i++)
	{
		for (j = 0; j < GUICursor.GUICursorPTBitmap.w; j++)
		{
			((uint16_t *)GUICursor.GUICursorMem)[j + i * GUICursor.GUICursorPTBitmap.w] =
				GUIGetPixel(x + j, y + i);
		}
	}
}
/*
设置光标位置
位置没有改变则不需要操作
1.绘制之前保存的背景
2.重新设置当前位置和上一次的位置
3.然后保存新位置的背景到缓存
*/
BOOL GUICursorSetPOI(int16_t x, int16_t y)
{
	int16_t i, j;
	uint32_t *pixels = (uint32_t *)(GUICursor.GUICursorPTBitmap.pixels);
	/*位置没有改变则不需要操作*/
	if (x == GUICursor.cursorPOI.x && GUICursor.cursorPOI.y == y)
	{
		return FALSE;
	}

	/*画上一次*/
	GUIDrawBitmap(
		GUICursor.cursorPOI.x, GUICursor.cursorPOI.y,
		GUICursor.GUICursorPTBitmap.w, GUICursor.GUICursorPTBitmap.h,
		GUICursor.GUICursorMem);

	/*设置新的位置*/
	GUICursor.lastCursorPOI.x = GUICursor.cursorPOI.x;
	GUICursor.lastCursorPOI.y = GUICursor.cursorPOI.y;

	GUICursor.cursorPOI.x = x;
	GUICursor.cursorPOI.y = y;

	/*拷贝当前屏幕内容到缓存*/
	GUICursorLastLCDCopyToMem();

	/*绘制光标*/
	// GUICursorDrawLCD();

	for (i = 0; i < GUICursor.GUICursorPTBitmap.h; i++)
	{
		for (j = 0; j < GUICursor.GUICursorPTBitmap.w; j++)
		{
			uint32_t color = pixels[(i)*GUICursor.GUICursorPTBitmap.w + j];
			if (!(color & 0xff000000))
			{
				GUIDrawPixel(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
				// GUICursorDrawMem(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
			}
		}
	}

	return TRUE;
}
/*往游标的缓存内绘制*/
uint8_t GUICursorDrawMem(int16_t x, int16_t y, uintColor color)
{
	if (x >= GUICursor.cursorPOI.x && x < GUICursor.cursorPOI.x + GUICursor.GUICursorPTBitmap.w && y >= GUICursor.cursorPOI.y && y < GUICursor.cursorPOI.y + GUICursor.GUICursorPTBitmap.h)
	{
		int16_t drawX, drawY;
		drawX = x - GUICursor.cursorPOI.x;
		drawY = y - GUICursor.cursorPOI.y;
		//((uint16_t*)(GUICursor.GUICursorLastPOIMem))[drawX+drawY* GUICursor.GUICursorPTBitmap.w]=color;
		((uint16_t *)(GUICursor.GUICursorMem))[drawX + drawY * GUICursor.GUICursorPTBitmap.w] = color;
		_SET_CURSOR_UPT(&GUICursor);
		return TRUE;
	}
	return FALSE;
}
/*写入到屏幕*/
void GUICursorDrawLCD(void)
{
	int16_t i, j;
	uint32_t *pixels = (uint32_t *)(GUICursor.GUICursorPTBitmap.pixels);

	/*画上一次*/
	GUIDrawBitmap(
		GUICursor.cursorPOI.x, GUICursor.cursorPOI.y,
		GUICursor.GUICursorPTBitmap.w, GUICursor.GUICursorPTBitmap.h,
		GUICursor.GUICursorMem);

	for (i = 0; i < GUICursor.GUICursorPTBitmap.h; i++)
	{
		for (j = 0; j < GUICursor.GUICursorPTBitmap.w; j++)
		{
			uint32_t color = pixels[(i)*GUICursor.GUICursorPTBitmap.w + j];
			if (!(color & 0xff000000))
			{
				GUIDrawPixel(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
				// GUICursorDrawMem(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
			}
		}
	}
}
/*设置游标类型*/
void GUICursorSetType(uint8_t type)
{
}

/*显示游标*/
void GUICursorShow(void)
{
	_SET_CURSOR_SHOW(&GUICursor);
}

/*隐藏显示*/
void GUICursorHide(void)
{
	_SET_CURSOR_HIDE(&GUICursor);
}
/*关闭显示*/
void GUICursorClose(void)
{
	_SET_CURSOR_CLOSE(&GUICursor);
	xFree(GUICursor.GUICursorMem);
}