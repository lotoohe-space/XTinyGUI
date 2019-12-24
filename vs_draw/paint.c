
#include "paint.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "widge.h"
#include <stdlib.h>
#ifdef WIN32
 extern void d_pix(int x, int y, int color);
 extern void fill_rect(int x, int y, int w, int h, int color);
 extern void d_lineH(int x0, int y, int x1, int color);
 extern void d_lineV(int x, int y0, int y1, int color);
#endif

 /*绘图区域*/
 XRECT drawArea = {0,0,LCD_SCREEN_W ,LCD_SCREEN_H};//绘制区域

 /*设置无效区域*/
 void DrawInvaildRect(HXRECT hXRect) {
	 if (hXRect == NULL) {
		 drawArea.x = 0;
		 drawArea.y = 0;
		 drawArea.w = LCD_SCREEN_W;
		 drawArea.h = LCD_SCREEN_H;
	 }
	 else {
		 XRECT_COPY(&drawArea, hXRect);
	 }
 }
 /*设置允许绘图的窗口，是当前窗口与桌面的相交区域*/
void DrawSetArea(void* hObject) {
	HWIDGE_BASE hWidgeBase;
	XRECT rRect;
	hWidgeBase = hObject;
	if (!hWidgeBase) { return; }

	/*控件矩形与桌面矩形的相交矩形即为绘图区域*/
	GetOverLapRect(&drawArea ,&(hWidgeBase->rect),&rRect);

	/*重新设置绘图区域*/
	XRECT_COPY(&drawArea, &rRect);

	/*重新设置控件的有效绘图区域*/
	XRECT_COPY(&(hWidgeBase->pencil), &rRect);
}
/*复位绘图区域*/
void DrawResetArea(void* hObject) {
	HWIDGE_BASE hWidgeBase;
	HPENCIL hPencil;
	hWidgeBase = hObject;
	if (!hWidgeBase) { return; }
	hPencil = &(hWidgeBase->pencil);

	XRECT_COPY(&drawArea, hPencil);
}
uint8 DrawPixel(uintColor color,int16 x, int16 y) {
	d_pix(x, y, RGB565TORGB888(color));
	return (uint8)1;
}
uint8 DrawLineH(HPENCIL hPencil, int16 x0, int16 y, int16 x1) {
	if (!hPencil ) {
		return (uint8)0;
	}
	//剪切横线
	XHLINE xLadLine;
	if (y < hPencil->y && y > hPencil->y + hPencil->h) {
		xLadLine.x0 = x0;
		xLadLine.x1 = 0;
		xLadLine.y = y;
	}
	else {
		xLadLine.y = y;
		//计算相交区域
		xLadLine.x0 = hPencil->x;
		if (xLadLine.x0<x0 || xLadLine.x0>x1) {
			xLadLine.x0 = x0;
		}
		xLadLine.x1 = hPencil->x + hPencil->w;
		if (xLadLine.x1<x0 || xLadLine.x1>x1) {
			xLadLine.x1 = x1;
		}
	}

	d_lineH(xLadLine.x0, xLadLine.y, xLadLine.x1, RGB565TORGB888(hPencil->DrawColor));
	return (uint8)1;
}
uint8 DrawLineV(HPENCIL hPencil, int16 x, int16 y0, int16 y1) {
	if (!hPencil) {
		return (uint8)0;
	}
	d_lineV(x, y0, y1, hPencil->DrawColor);
	return (uint8)1;
}

uint8 DrawRect(HPENCIL hPencil, HXRECT hXRECT) {
	XRECT rRect;
	if (!hPencil ) {
		return (uint8)0;
	}
	GetOverLapRect(hXRECT, (HXRECT)hPencil,&rRect);
	fill_rect(rRect.x, rRect.y, rRect.w, rRect.h, RGB565TORGB888(hPencil->DrawColor));
	//fill_rect(rRect.x, rRect.y, rRect.w, rRect.h, RGB565TORGB888(rand()%65535));

	return (uint8)1;
}
void delay_ms(int n) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < 1000000; j++);
	}
}
uint8 DrawCutRect(void* hObject,HXRECT hXRECT) {
	HWIDGE_BASE hWidgeBase=hObject;
	if (!hWidgeBase) { return (uint8)0; }
	/*循环绘制剪裁后的矩形*/
	RECT_CUT_INIT(hXRECT) {
		//delay_ms(1000);
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}
	RECT_CUT_END()

	return (uint8)1;
}
uint8 DrawChar(HXRECT hXRectArea, HFONTF hFont,int16 x, int16 y,uint8 ch,uintColor color) {
	uint8 *chData;
	if (!hXRectArea) {
		return (uint8)0;
	}
	chData = FontReadChar(hFont, ch);
	if (chData != NULL) {
		int16 i, j;
		int16 stx = MAX(hXRectArea->x, x);
		int16 sty = MAX(hXRectArea->y, y);
		int16 endy = MIN(hFont->fontInfo.h + y, hXRectArea->y + hXRectArea->h);
		int16 endx= MIN(hFont->fontInfo.w + x, hXRectArea->x + hXRectArea->w);
		
		for (i = y; i < endy; i++) {
			if (i >= sty) {
				for (j = x; j < endx; j++) {
					if (j >= stx) {
						uint16 tCHData = hFont->fontInfo.perRowBytes==2 ? ((uint16*)chData)[i - y] : chData[i - y];
						if (_GET_BIT(tCHData, j - x)) {
							DrawPixel(color, j, i);
						}
					}
				}
			}
		}
	}
	return (uint8)1;
}
uint8 DrawString(HPENCIL hPencil, HFONTF hFont, HXRECT dHRect ,int16 x,int16 y, uint8* text) {
	XRECT rRect;
	uint16 i;
	int16 stx;
	int16 sty;
	int16 endy;
	int16 endx;
	if (!hPencil) {
		return (uint8)0;
	}

	GetOverLapRect(dHRect, (HXRECT)hPencil,&rRect);

	stx = MAX(rRect.x, x);
	sty = MAX(rRect.y, y);
	endy = MIN((hFont->fontInfo.h) + y, rRect.y + rRect.h);
	endx = MIN((int16)(TStrlen(text)*(hFont->fontInfo.w)) + x, rRect.x + rRect.w);

	for (i = 0; text[i]; i++) {
		uint8 *chData;
		chData = FontReadChar(hFont, text[i]);
		if (chData != NULL) {
			int16 i, j;
			for (i = y; i < MIN(hFont->fontInfo.h + y,endy); i++) {
				if (i >= sty) {
					for (j = x; j < MIN(hFont->fontInfo.w+x,endx); j++) {
						if (j >= stx) {
							uint16 tCHData = hFont->fontInfo.perRowBytes==2 ? ((uint16*)chData)[i - y] : chData[i - y];
							if (_GET_BIT(tCHData, j - x)) {
								DrawPixel(hPencil->DrawColor, j, i);
							}
							else {
								DrawPixel(hPencil->DrawBkColor, j, i);
							}
						}
					}
				}
			}
		}
		x += hFont->fontInfo.w;
	}
	return (uint8)1;
}
uint8 DrawCutString(void* hObject, HFONTF hFont,HXRECT border, uint8* text) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase) { return (uint8)0; }
	RECT_CUT_INIT(border) {
		DrawString(&(hWidgeBase->pencil), hFont, nextCutRect, border->x, border->y, text);
	}
	RECT_CUT_END()
	return (int8)1;
}
