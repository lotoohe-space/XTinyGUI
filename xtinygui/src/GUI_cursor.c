#include "GUI_cursor.h"
#include "color.h"
#include "GUI_interface_extern.h"
#include "paint.h"
#include "x_malloc.h"

/* 7*12 */
const uint32 GUICursorPT[] = {
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

/*�α�*/
GUI_CURSOR GUICursor = {
	7,
	12,
	(uint8 *)GUICursorPT,
	0,
	32,
	BITMAP_DRAW_ARGB8888,
	NULL,
	0, 0,
	0};

/*GUI�α��ʼ��*/
uint8 GUICursorInit(void)
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
/*���浱ǰλ�õı���������*/
void GUICursorLastLCDCopyToMem(void)
{
	int16 i, j;
	int16 x, y;
	x = GUICursor.cursorPOI.x;
	y = GUICursor.cursorPOI.y;
	/*��ȡ��ǰλ�õı���ͼƬ*/
	for (i = 0; i < GUICursor.GUICursorPTBitmap.h; i++)
	{
		for (j = 0; j < GUICursor.GUICursorPTBitmap.w; j++)
		{
			((uint16 *)GUICursor.GUICursorMem)[j + i * GUICursor.GUICursorPTBitmap.w] =
				GUIGetPixel(x + j, y + i);
		}
	}
}
/*
���ù��λ��
λ��û�иı�����Ҫ����
1.����֮ǰ����ı���
2.�������õ�ǰλ�ú���һ�ε�λ��
3.Ȼ�󱣴���λ�õı���������
*/
BOOL GUICursorSetPOI(int16 x, int16 y)
{
	int16 i, j;
	uint32 *pixels = (uint32 *)(GUICursor.GUICursorPTBitmap.pixels);
	/*λ��û�иı�����Ҫ����*/
	if (x == GUICursor.cursorPOI.x && GUICursor.cursorPOI.y == y)
	{
		return FALSE;
	}

	/*����һ��*/
	GUIDrawBitmap(
		GUICursor.cursorPOI.x, GUICursor.cursorPOI.y,
		GUICursor.GUICursorPTBitmap.w, GUICursor.GUICursorPTBitmap.h,
		GUICursor.GUICursorMem);

	/*�����µ�λ��*/
	GUICursor.lastCursorPOI.x = GUICursor.cursorPOI.x;
	GUICursor.lastCursorPOI.y = GUICursor.cursorPOI.y;

	GUICursor.cursorPOI.x = x;
	GUICursor.cursorPOI.y = y;

	/*������ǰ��Ļ���ݵ�����*/
	GUICursorLastLCDCopyToMem();

	/*���ƹ��*/
	// GUICursorDrawLCD();

	for (i = 0; i < GUICursor.GUICursorPTBitmap.h; i++)
	{
		for (j = 0; j < GUICursor.GUICursorPTBitmap.w; j++)
		{
			uint32 color = pixels[(i)*GUICursor.GUICursorPTBitmap.w + j];
			if (!(color & 0xff000000))
			{
				GUIDrawPixel(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
				// GUICursorDrawMem(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
			}
		}
	}

	return TRUE;
}
/*���α�Ļ����ڻ���*/
uint8 GUICursorDrawMem(int16 x, int16 y, uintColor color)
{
	if (x >= GUICursor.cursorPOI.x && x < GUICursor.cursorPOI.x + GUICursor.GUICursorPTBitmap.w && y >= GUICursor.cursorPOI.y && y < GUICursor.cursorPOI.y + GUICursor.GUICursorPTBitmap.h)
	{
		int16 drawX, drawY;
		drawX = x - GUICursor.cursorPOI.x;
		drawY = y - GUICursor.cursorPOI.y;
		//((uint16*)(GUICursor.GUICursorLastPOIMem))[drawX+drawY* GUICursor.GUICursorPTBitmap.w]=color;
		((uint16 *)(GUICursor.GUICursorMem))[drawX + drawY * GUICursor.GUICursorPTBitmap.w] = color;
		_SET_CURSOR_UPT(&GUICursor);
		return TRUE;
	}
	return FALSE;
}
/*д�뵽��Ļ*/
void GUICursorDrawLCD(void)
{
	int16 i, j;
	uint32 *pixels = (uint32 *)(GUICursor.GUICursorPTBitmap.pixels);

	/*����һ��*/
	GUIDrawBitmap(
		GUICursor.cursorPOI.x, GUICursor.cursorPOI.y,
		GUICursor.GUICursorPTBitmap.w, GUICursor.GUICursorPTBitmap.h,
		GUICursor.GUICursorMem);

	for (i = 0; i < GUICursor.GUICursorPTBitmap.h; i++)
	{
		for (j = 0; j < GUICursor.GUICursorPTBitmap.w; j++)
		{
			uint32 color = pixels[(i)*GUICursor.GUICursorPTBitmap.w + j];
			if (!(color & 0xff000000))
			{
				GUIDrawPixel(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
				// GUICursorDrawMem(j + GUICursor.cursorPOI.x, i + GUICursor.cursorPOI.y, BGR888T0RGB565(color));
			}
		}
	}
}
/*�����α�����*/
void GUICursorSetType(uint8 type)
{
}

/*��ʾ�α�*/
void GUICursorShow(void)
{
	_SET_CURSOR_SHOW(&GUICursor);
}

/*������ʾ*/
void GUICursorHide(void)
{
	_SET_CURSOR_HIDE(&GUICursor);
}
/*�ر���ʾ*/
void GUICursorClose(void)
{
	_SET_CURSOR_CLOSE(&GUICursor);
	xFree(GUICursor.GUICursorMem);
}