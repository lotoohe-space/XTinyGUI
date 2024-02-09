
#include "paint.h"
#include "x_tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "widge.h"
#include "msg.h"
#include "bitmap.h"
#include "text_widge.h"
#include "mem_dev.h"
#include "GUI_cursor.h"
#include "GUI_interface_extern.h"
#include <stdlib.h>

/*读点*/
uintColor _GetPixel(int16_t x, int16_t y)
{
#if USE_MEM_DEV
	int16_t rX = x - hXDesktop->hMemDev->rect.x;
	int16_t rY = y - hXDesktop->hMemDev->rect.y;
	return MemDevReadPT(hXDesktop->hMemDev, rX, rY);
#else
	return GUIGetPixel(x, y);
#endif
}
/*写点*/
void DrawPixel(uintColor color, int16_t x, int16_t y)
{
#if USE_MEM_DEV
	int16_t rX = x - hXDesktop->hMemDev->rect.x;
	int16_t rY = y - hXDesktop->hMemDev->rect.y;
	MemDevDrawPT(hXDesktop->hMemDev, rX, rY, color);
#else /*USE_MEM_DEV*/
#if USE_CURSOR
	if (!GUICursorDrawMem(x, y, color))
	{ /*写入失败则写入到屏幕，否则写入到缓存中*/
		GUIDrawPixel(x, y, color);
	}
#else  /*USE_CURSOR*/
	GUIDrawPixel(x, y, color);
#endif /*USE_CURSOR*/
#endif /*USE_MEM_DEV*/
}

/*画一个透明的点*/
void DrawAPixel(uintColor aColor, int16_t x, int16_t y)
{

	uint16_t lcColor = _GetPixel(x, y);
	uint8_t A = 0xFF - C565A(aColor);
	if (A == 0x00)
	{
		return;
	}
	else if (A == 0xff)
	{
		DrawPixel(aColor, x, y);
		return;
	}
	else
	{
		uint8_t R = ((C565R(aColor) * A + C565R(lcColor) * (0xff - A)) >> 8) & 0xff;
		uint8_t G = ((C565G(aColor) * A + C565G(lcColor) * (0xff - A)) >> 8) & 0xff;
		uint8_t B = ((C565B(aColor) * A + C565B(lcColor) * (0xff - A)) >> 8) & 0xff;

		DrawPixel(rgb565_t(R, G, B), x, y);
	}
}
/*画矩形*/
void RawDrawRect(p_xrect_t hRect, uintColor color)
{
	int16_t i, j;
	for (i = hRect->y; i < hRect->y + hRect->h; i++)
	{
		for (j = hRect->x; j < hRect->x + hRect->w; j++)
		{
			DrawPixel(color, j, i);
		}
	}
}
/*画透明矩形*/
void RawDrawARect(p_xrect_t hRect, uintColor color)
{
	int16_t i, j;
	for (i = hRect->y; i < hRect->y + hRect->h; i++)
	{
		for (j = hRect->x; j < hRect->x + hRect->w; j++)
		{
			DrawAPixel(color, j, i);
		}
	}
}

/*绘制原始图片*/
void RawDrawBitmap(p_pencil_t hPencil, p_xrect_t drawBorder, p_xpoint_t startDrawBMP, p_xbitmap_t hXBitmap)
{
	int16_t i, j;
	int16_t endy;
	int16_t endx;
	uint16_t *pixels;
	pixels = (uint16_t *)(hXBitmap->pixels);
	endy = drawBorder->h + drawBorder->y;
	endx = drawBorder->w + drawBorder->x;
	for (i = drawBorder->y; i < endy; i++)
	{
		for (j = drawBorder->x; j < endx; j++)
		{
			if ((i - drawBorder->y + startDrawBMP->y) < hXBitmap->h && (j - drawBorder->x + startDrawBMP->x) < hXBitmap->w)
			{
				uintColor color = pixels[(i - drawBorder->y + startDrawBMP->y) * hXBitmap->w + (j - drawBorder->x) + startDrawBMP->x];
				DrawPixel(color, j, i);
			}
			else
			{
				DrawPixel(hPencil->DrawBkColor, j, i);
			}
		}
	}
#ifdef _X_DEBUG
	Vsrefresh();
#endif
}
/*画透明图片*/
void RawDrawABitmap(
	p_pencil_t hPencil, p_xrect_t drawBorder, p_xpoint_t startDrawBMP, p_xbitmap_t hXBitmap)
{
	int16_t i, j;
	int16_t endy;
	int16_t endx;
	uint16_t *pixels;
	pixels = (uint16_t *)(hXBitmap->pixels);
	endy = drawBorder->h + drawBorder->y;
	endx = drawBorder->w + drawBorder->x;
	for (i = drawBorder->y; i < endy; i++)
	{
		for (j = drawBorder->x; j < endx; j++)
		{
			if ((i - drawBorder->y + startDrawBMP->y) < hXBitmap->h && (j - drawBorder->x + startDrawBMP->x) < hXBitmap->w)
			{
				uintColor color = pixels[(i - drawBorder->y + startDrawBMP->y) * hXBitmap->w + (j - drawBorder->x) + startDrawBMP->x];
				DrawAPixel((hXBitmap->alpha << 16) | color, j, i);
			}
			else
			{
				DrawPixel(hPencil->DrawBkColor, j, i);
			}
		}
	}
#ifdef _X_DEBUG
	Vsrefresh();
#endif
}
/*画单独通道的透明图片ARGB8888*/
void RawDrawARGBBitmap(
	p_pencil_t hPencil, p_xrect_t drawBorder, p_xpoint_t startDrawBMP, p_xbitmap_t hXBitmap)
{
	int16_t i, j;
	int16_t endy;
	int16_t endx;
	uint32_t *pixels;
	pixels = (uint32_t *)(hXBitmap->pixels);
	endy = drawBorder->h + drawBorder->y;
	endx = drawBorder->w + drawBorder->x;
	for (i = drawBorder->y; i < endy; i++)
	{
		for (j = drawBorder->x; j < endx; j++)
		{
			if ((i - drawBorder->y + startDrawBMP->y) < hXBitmap->h && (j - drawBorder->x + startDrawBMP->x) < hXBitmap->w)
			{
				uint32_t color = pixels[(i - drawBorder->y + startDrawBMP->y) * hXBitmap->w + (j - drawBorder->x) + startDrawBMP->x];
				DrawAPixel(
					((color & 0xff000000) >> 8) |
						BGR888T0RGB565(color),
					j, i);
			}
			else
			{
				DrawPixel(hPencil->DrawBkColor, j, i);
			}
		}
	}
#ifdef _X_DEBUG
	Vsrefresh();
#endif
}

/*绘制二进制图片,宽和高只能是8的倍数*/
void DrawBitmapBinary(p_pencil_t hPencil, p_xrect_t drawBorder, p_xpoint_t startDrawPT, p_xbitmap_t hXBitmap)
{
	uint16_t i, j;
	uint16_t pixel;
	uint16_t draw_w;
	uint16_t draw_h;
	if (drawBorder == NULL || startDrawPT == NULL || hXBitmap == NULL)
	{
		return;
	}
	draw_w = MIN(drawBorder->w, hXBitmap->w);
	draw_h = MIN(drawBorder->h, hXBitmap->h);

	if (hXBitmap->bitsPerPixel == 1)
	{
		for (i = 0; i < draw_h; i++)
		{
			for (j = 0; j < draw_w; j++)
			{
				uint16_t temp = (i + startDrawPT->y) * hXBitmap->w +
								(j + startDrawPT->x); // 当前像素的位置
				pixel = hXBitmap->pixels[temp / 8] & ((1 << (7 - (temp % 8))));
				if (pixel)
				{
					DrawPixel(hPencil->DrawFrColor, j + drawBorder->x, i + drawBorder->y);
				}
				else
				{
					DrawPixel(hPencil->DrawBkColor, j + drawBorder->x, i + drawBorder->y);
				}
			}
		}
	}
#ifdef _X_DEBUG
	Vsrefresh();
#endif
}

/*在规定区域内绘制透明矩形*/
uint8_t DrawARect(p_pencil_t hPencil, p_xrect_t hXRECT)
{
	xrect_t rRect;
	if (!hPencil)
	{
		return (uint8_t)0;
	}
	GetOverLapRect(hXRECT, (p_xrect_t)hPencil, &rRect);

	RawDrawARect(&rRect, hPencil->DrawColor);
#ifdef _X_DEBUG
	Vsrefresh();
#endif
	return (uint8_t)1;
}

/*在规定区域内绘制矩形*/
uint8_t DrawRect(p_pencil_t hPencil, p_xrect_t hXRECT)
{
	xrect_t rRect;
	if (!hPencil)
	{
		return FALSE;
	}
	if (GetOverLapRect(hXRECT, (p_xrect_t)hPencil, &rRect) == FALSE)
	{
		return FALSE;
	}

#if USE_MEM_DEV
	RawDrawRect(&rRect, hPencil->DrawColor);
#else
	GUIDrawRect(rRect.x, rRect.y, rRect.w, rRect.h, hPencil->DrawColor);
#endif
#ifdef _X_DEBUG
	Vsrefresh();
#endif
	return TRUE;
}

/*
 * 在规定区域内绘制图片
 * hPencil 画布
 * border 绘图的边界
 * bgBorder 控件的边界
 * hXBitmap 绘制的图片
 */
uint8_t DrawBitmap(p_pencil_t hPencil, p_xrect_t border, p_xrect_t bgBorder, p_xbitmap_t hXBitmap)
{
	xrect_t rRect;	/*可以绘制的区域*/
	xrect_t rRect1; /*可以绘制的区域*/
	xrect_t temp;
	if (!hPencil)
	{
		return (uint8_t)0;
	}

	GetOverLapRect(border, (p_xrect_t)hPencil, &rRect);
	temp.x = bgBorder->x;
	temp.y = bgBorder->y;
	temp.w = hXBitmap->w;
	temp.h = hXBitmap->h;

	GetOverLapRect(&rRect, (p_xrect_t)&temp, &rRect1);

	xpoint_t xPoint;
	xPoint.x = rRect.x - bgBorder->x;
	xPoint.y = rRect.y - bgBorder->y;
	switch (hXBitmap->bitsPerPixel)
	{
	case 1: /*2位色*/
		DrawBitmapBinary(hPencil, &rRect1, &xPoint, hXBitmap);
		break;
	case 16: /*16位色*/
		if (hXBitmap->flag & BITMAP_DRAW_ARGB)
		{ /*统一控制透明度*/
			RawDrawABitmap(
				hPencil, &rRect, &xPoint, hXBitmap);
		}
		else
		{
			RawDrawBitmap(
				hPencil, &rRect, &xPoint, hXBitmap);
		}
		break;
	case 32: /*32位色*/
		if (hXBitmap->flag & BITMAP_DRAW_ARGB8888)
		{ /*单独通道控制透明度*/
			RawDrawARGBBitmap(hPencil, &rRect, &xPoint, hXBitmap);
		}
		break;
	}
	return TRUE;
}

/*
 * 在规定区域内绘制字符串
 */
uint8_t DrawString(p_pencil_t hPencil, p_font_t hFont, p_xrect_t border, int16_t x, int16_t y, uint8_t *text)
{
	xrect_t rRect;
	uint16_t i;
	int16_t stx;
	int16_t sty;
	int16_t endy;
	int16_t endx;
	if (!hPencil)
	{
		return (uint8_t)0;
	}

	GetOverLapRect(border, (p_xrect_t)hPencil, &rRect);

	stx = MAX(rRect.x, x);
	sty = MAX(rRect.y, y);
	endy = MIN((hFont->fontInfo.h) + y, rRect.y + rRect.h);
	endx = MIN((int16_t)(GBK_Strlen(text) * (hFont->fontInfo.w)) + x, rRect.x + rRect.w);

	for (i = 0; text[i]; i++)
	{
		uint8_t *chData;
		chData = FontReadChar(hFont, text[i]);
		if (chData != NULL)
		{
			int16_t i, j;
			for (i = y; i < MIN(hFont->fontInfo.h + y, endy); i++)
			{
				if (i >= sty)
				{
					for (j = x; j < MIN(hFont->fontInfo.w + x, endx); j++)
					{
						if (j >= stx)
						{
							uint16_t tCHData = hFont->fontInfo.perRowBytes == 2 ? ((uint16_t *)chData)[i - y] : chData[i - y];
							if (_GET_BIT(tCHData, j - x))
							{
								DrawPixel(hPencil->DrawFrColor, j, i);
							}
							else
							{
								DrawPixel(hPencil->DrawBkColor, j, i);
							}
						}
					}
				}
			}
		}
		x += hFont->fontInfo.w;
	}
	return (uint8_t)1;
}

static uint8_t _DrawChar(p_pencil_t hPencil, p_xrect_t hXRectArea, p_font_t hFont, p_xpoint_t startPoint, uint8_t ch)
{
	uint8_t *chData;
	if (!hXRectArea)
	{
		return (uint8_t)0;
	}
	chData = FontReadChar(hFont, ch);
	if (chData != NULL)
	{
		uint16_t i, j;
		uint16_t pixel;
		uint16_t draw_w;
		uint16_t draw_h;
		if (hXRectArea == NULL || startPoint == NULL)
		{
			return 0;
		}
		draw_w = MIN(hXRectArea->w, hFont->fontInfo.w);
		draw_h = MIN(hXRectArea->h, hFont->fontInfo.h);

		for (i = 0; i < draw_h; i++)
		{
			for (j = 0; j < draw_w; j++)
			{
				uint16_t temp = (i + startPoint->y) * hFont->fontInfo.w +
								(j + startPoint->x); // 当前像素的位置
				pixel = chData[temp / 8] & ((1 << ((temp % 8))));
				if (pixel)
				{
					DrawPixel(hPencil->DrawFrColor, j + hXRectArea->x, i + hXRectArea->y);
				}
				else
				{
					DrawPixel(hPencil->DrawBkColor, j + hXRectArea->x, i + hXRectArea->y);
				}
			}
		}
	}
	return (uint8_t)1;
}

uint8_t _DrawCharEx(p_xrect_t bgRect, p_xrect_t drawRect, p_xpoint_t hStartXPoint, p_font_t hFont, char ch, p_pencil_t hPencil)
{
	uint16_t i, j;
	uint16_t pixel;
	uint8_t *chData;
	uint16_t draw_w;
	uint16_t draw_h;
	uint16_t sDrawX;
	uint16_t sDrawY;

	chData = FontReadChar(hFont, ch);
	if (chData != NULL)
	{

		sDrawX = bgRect->x + hStartXPoint->x;
		sDrawY = bgRect->y + hStartXPoint->y;

		draw_w = hFont->fontInfo.w; // MIN(drawRect->w, hFont->fontInfo.w);
		draw_h = hFont->fontInfo.h; // MIN(drawRect->h, hFont->fontInfo.h);

		for (i = drawRect->y; i < drawRect->y + drawRect->h; i++)
		{
			for (j = drawRect->x; j < drawRect->x + drawRect->w; j++)
			{
				if (j >= sDrawX && i >= sDrawY && j <= sDrawX + draw_w && i <= sDrawY + draw_h)
				{

					uint16_t temp = (i - sDrawY) * hFont->fontInfo.w +
									(j - sDrawX); // 当前像素的位置
					pixel = chData[temp / 8] & ((1 << ((temp % 8))));
					if (pixel)
					{
						DrawPixel(hPencil->DrawFrColor,
								  j, i);
					}
					else
					{
						DrawPixel(hPencil->DrawBkColor,
								  j, i);
					}
				}
				else
				{
					DrawPixel(hPencil->DrawBkColor,
							  j, i);
				}
			}
		}
	}

	return 1;
}
uint8_t _DrawStringEx(p_xrect_t bgRect, p_xrect_t drawRect, p_xpoint_t hStartXPoint, p_font_t hFont, const char *text, p_pencil_t hPencil)
{
	// int6 drawX=0;
	int16_t i, j;
	uint16_t pixel;
	uint8_t *chData = NULL;
	uint16_t draw_w;
	uint16_t draw_h;
	int16_t sDrawX;
	int16_t sDrawY;
	// uint16_t m;
	int16_t lastIndex = 0;
	int16_t lastIndex1 = 0;
	uint16_t ct = -1;
	sDrawX = bgRect->x + hStartXPoint->x;
	sDrawY = bgRect->y + hStartXPoint->y;
	if (hFont->fontType == 3)
	{ /*UNICODE*/
		draw_w = hFont->fontInfo.w * UNI_Strlen((uint16_t *)text);
	}
	else
	{ /*ASCII GB2312*/
		draw_w = hFont->fontInfo.w * GBK_Strlen(text);
	}
	draw_h = hFont->fontInfo.h;
	for (i = drawRect->y; i < drawRect->y + drawRect->h; i++)
	{
		lastIndex = 0;
		lastIndex1 = 0;
		for (j = drawRect->x; j < drawRect->x + drawRect->w; j++)
		{
			if (j >= sDrawX && i >= sDrawY && j < sDrawX + draw_w && i < sDrawY + draw_h)
			{
				uint16_t index = (j - sDrawX) / hFont->fontInfo.w; /*当前需要获取的字符索引ASCII字符*/
				uint16_t temp = (j - sDrawX) % hFont->fontInfo.w;
				uint16_t temp1 = ((i - sDrawY) % hFont->fontInfo.h) * hFont->fontInfo.perRowBytes;
				if (hFont->fontType == 3)
				{ // UNICODE
					uint16_t Wchar = (uint16_t)((((uint16_t)(text[index * hFont->fontInfo.perRowBytes + 1]) << 8) & 0xff00) | (text[index * hFont->fontInfo.perRowBytes] & 0xff));
					chData = FontReadChar(hFont, Wchar);
				}
				else
				{ /*ascii gb22312*/
					const char *wChar = GBK_CharGet(text, index);
					if (wChar == NULL)
					{
						continue;
					}
					if (wChar[0] & 0x80)
					{
						uint16_t Wchar = (uint16_t)(((uint16_t)(wChar[0]) << 8) | (wChar[1] & 0xff));
						chData = FontReadChar(hFont, Wchar);
					}
					else
					{
						chData = FontReadChar(hFont, wChar[0]);
					}
				}
				if (chData == NULL)
				{
					continue;
				}
				pixel = chData[temp1 + temp / 8] & ((1 << (temp % 8)));
				if (pixel)
				{
					DrawPixel(hPencil->DrawFrColor, j, i);
				}
				else
				{
					DrawPixel(hPencil->DrawBkColor, j, i);
				}
			}
			else
			{
				DrawPixel(hPencil->DrawBkColor,
						  j, i);
			}
		}
	}

	// Vsrefresh();
	return 1;
}
uint8_t DrawStringEx(p_xrect_t bgBorder, p_xrect_t drawBorder, p_xpoint_t hStartXPoint, p_font_t hFont, const char *text, p_pencil_t hPencil)
{
	xrect_t rRect;	/*可以绘制的区域*/
	xrect_t rRect1; /*可以绘制的区域*/
	xrect_t temp;
	if (!hPencil)
	{
		return (uint8_t)0;
	}
	GetOverLapRect(drawBorder, (p_xrect_t)hPencil, &rRect);

	_DrawStringEx(bgBorder, &rRect, hStartXPoint, hFont, text, hPencil);
	return TRUE;
}

uint8_t DrawCharEx(p_xrect_t bgBorder, p_xrect_t drawBorder, p_xpoint_t hStartXPoint, p_font_t hFont, char ch, p_pencil_t hPencil)
{
	xrect_t rRect;	/*可以绘制的区域*/
	xrect_t rRect1; /*可以绘制的区域*/
	xrect_t temp;
	if (!hPencil)
	{
		return (uint8_t)0;
	}
	GetOverLapRect(drawBorder, (p_xrect_t)hPencil, &rRect);

	_DrawCharEx(bgBorder, &rRect, hStartXPoint, hFont, ch, hPencil);
	return TRUE;
}

uint8_t DrawChar(p_pencil_t hPencil, p_font_t hFont, p_xrect_t border, p_xrect_t bgBorder, char ch)
{
	xrect_t rRect;	/*可以绘制的区域*/
	xrect_t rRect1; /*可以绘制的区域*/
	xrect_t temp;
	if (!hPencil)
	{
		return (uint8_t)0;
	}
	GetOverLapRect(border, (p_xrect_t)hPencil, &rRect);
	temp.x = bgBorder->x;
	temp.y = bgBorder->y;
	temp.w = hFont->fontInfo.w;
	temp.h = hFont->fontInfo.h;

	GetOverLapRect(&rRect, (p_xrect_t)&temp, &rRect1);

	xpoint_t xPoint;
	xPoint.x = rRect.x - bgBorder->x;
	xPoint.y = rRect.y - bgBorder->y;

	_DrawChar(hPencil, &rRect1, hFont, &xPoint, ch);
	return TRUE;
}

/*字绘制*/
uint8_t DrawCutChar(void *hObject, p_font_t hFont, p_xrect_t bgRect, p_xpoint_t hStartXPoint, char ch)
{
	p_widget_base_t hWidgeBase = hObject;
	if (!hWidgeBase)
	{
		return (uint8_t)FALSE;
	}
	p_xrect_t nextCutRect = NULL;
	RECT_CUT_FOREACH(nextCutRect, bgRect)
	{
		DrawCharEx(bgRect, nextCutRect, hStartXPoint, hFont, ch, &(hWidgeBase->pencil));
	}
	return (int8_t)TRUE;
}

/*字符串剪裁绘制*/
uint8_t DrawCutString(void *hObject, p_font_t hFont, p_xrect_t border, p_xpoint_t hXPoint, const char *text)
{
	xpoint_t startPoint;
	p_widget_base_t hWidgeBase = hObject;
	if (!hWidgeBase)
	{
		return (uint8_t)FALSE;
	}

	startPoint.x = 0;
	startPoint.y = 0;
	p_xrect_t nextCutRect = NULL;

	RECT_CUT_FOREACH(nextCutRect, border)
	{
		//	DrawString(&(hWidgeBase->pencil), hFont, nextCutRect, !hXPoint ? border->x : hXPoint->x, !hXPoint ? border->y : hXPoint->y, text);
		DrawStringEx(border, nextCutRect, hXPoint == 0 ? &startPoint : hXPoint, hFont, text, &(hWidgeBase->pencil));
	}
	return (int8_t)TRUE;
}

/*图片剪裁绘制*/
uint8_t DrawCutBitmap(void *hObject, p_xrect_t border, p_xbitmap_t hXBitmap)
{
	p_xrect_t nextCutRect = NULL;
	p_widget_base_t hWidgeBase = hObject;
	if (!hWidgeBase || !border || !hXBitmap)
	{
		return (uint8_t)0;
	}
	/*循环绘制剪裁后的矩形*/
	RECT_CUT_FOREACH(nextCutRect, border)
	{
		DrawBitmap(&(hWidgeBase->pencil), nextCutRect, border, hXBitmap);
	}

	return (uint8_t)1;
}
/*矩形剪裁绘制*/
uint8_t DrawCutRect(void *hObject, p_xrect_t hXRECT)
{
	p_xrect_t nextCutRect = NULL;
	p_widget_base_t hWidgeBase = hObject;
	if (!hWidgeBase)
	{
		return (uint8_t)0;
	}
	/*循环绘制剪裁后的矩形*/
	RECT_CUT_FOREACH(nextCutRect, hXRECT)
	{
		// delay_ms(1000);
		if (_GET_IS_DPY(hWidgeBase))
		{
			/*透明矩形*/
			DrawARect(&(hWidgeBase->pencil), nextCutRect);
		}
		else
		{
			DrawRect(&(hWidgeBase->pencil), nextCutRect);
		}
	}

	return (uint8_t)1;
}

// uint8_t DrawLineH(p_pencil_t hPencil, int16_t x0, int16_t y, int16_t x1) {
//	if (!hPencil ) {
//		return (uint8_t)0;
//	}
//	//剪切横线
//	xhline_t xLadLine;
//	if (y < hPencil->y && y > hPencil->y + hPencil->h) {
//		xLadLine.x0 = x0;
//		xLadLine.x1 = 0;
//		xLadLine.y = y;
//	}
//	else {
//		xLadLine.y = y;
//		//计算相交区域
//		xLadLine.x0 = hPencil->x;
//		if (xLadLine.x0<x0 || xLadLine.x0>x1) {
//			xLadLine.x0 = x0;
//		}
//		xLadLine.x1 = hPencil->x + hPencil->w;
//		if (xLadLine.x1<x0 || xLadLine.x1>x1) {
//			xLadLine.x1 = x1;
//		}
//	}
//
//	d_lineH(xLadLine.x0, xLadLine.y, xLadLine.x1, RGB565TORGB888(hPencil->DrawColor));
//	return (uint8_t)1;
// }
// uint8_t DrawLineV(p_pencil_t hPencil, int16_t x, int16_t y0, int16_t y1) {
//	if (!hPencil) {
//		return (uint8_t)0;
//	}
//	d_lineV(x, y0, y1, hPencil->DrawColor);
//	return (uint8_t)1;
// }

///*绘制原始图片*/
// void RawDrawBitmap(
//	p_pencil_t hPencil,
//	int16_t x, int16_t y,	/*实际图片绘制的起点*/
//	uint16_t w, uint16_t h,	/*需要绘制的图片的大小*/
//	uint16_t* bitmap,
//	int16_t x1, int16_t y1,	/*从图片中绘制的开始位置*/
//	uint16_t w1, uint16_t h1	/*图片的大小*/
//) {
//	int i, j;
//	int endy;
//	int endx;
//	endy = h + y;
//	endx = w + x;
//	for (i = y; i < endy; i++) {
//		for (j = x; j < endx; j++) {
//			DrawPixel((bitmap[(i - y + y1) * w1 + (j - x) + x1]), j, i);
//		}
//	}
// #ifdef _X_DEBUG
//	Vsrefresh();
// #endif
// }
///*画透明图片*/
// void RawDrawABitmap(
//	p_pencil_t hPencil,
//	int16_t x, int16_t y,	/*实际图片绘制的起点*/
//	uint16_t w, uint16_t h,	/*需要绘制的图片的大小*/
//	uint16_t* bitmap,
//	int16_t x1, int16_t y1,	/*从图片中绘制的开始位置*/
//	uint16_t w1, uint16_t h1,	/*图片的大小*/
//	uint8_t alpha
//) {
//	int i, j;
//	int endy;
//	int endx;
//	endy = h + y;
//	endx = w + x;
//	for (i = y; i < endy; i++) {
//		for (j = x; j < endx; j++) {
//			DrawAPixel((alpha << 16) | bitmap[(i - y + y1) * w1 + (j - x) + x1], j, i);
//		}
//	}
// #ifdef _X_DEBUG
//	Vsrefresh();
// #endif
// }
///*画单独通道的透明图片ARGB8888*/
// void RawDrawARGBBitmap(
//	int16_t x, int16_t y,	/*实际图片绘制的起点*/
//	uint16_t w, uint16_t h,	/*需要绘制的图片的大小*/
//	uint32_t* bitmap,
//	int16_t x1, int16_t y1,	/*从图片中绘制的开始位置*/
//	uint16_t w1, uint16_t h1	/*图片的大小*/
//) {
//	int i, j;
//	int endy;
//	int endx;
//	endy = h + y;
//	endx = w + x;
//	for (i = y; i < endy; i++) {
//		for (j = x; j < endx; j++) {
//			DrawAPixel(
//				((bitmap[(i - y + y1) * w1 + (j - x) + x1] & 0xff000000) >> 8) |
//				BGR888T0RGB565(bitmap[(i - y + y1) * w1 + (j - x) + x1]),
//				j, i);
//		}
//	}
// #ifdef _X_DEBUG
//	Vsrefresh();
// #endif
// }
