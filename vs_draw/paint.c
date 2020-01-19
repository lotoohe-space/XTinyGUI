  
#include "paint.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "widge.h"
#include "msg.h"
#include "bitmap.h"
#include "text_widge.h"
#include <stdlib.h>
#ifdef WIN32
 extern void d_pix(int x, int y, int color);
 extern void fill_rect(int x, int y, int w, int h, int color);
 extern void d_lineH(int x0, int y, int x1, int color);
 extern void d_lineV(int x, int y0, int y1, int color);
 void d_bmp(int x, int y, int w, int h, unsigned short* bitmap, int x1, int y1, int w1, int h1);
#endif

 /*绘图区域*/
 //XRECT drawArea = {0,0,LCD_SCREEN_W ,LCD_SCREEN_H};//绘制区域

 /*设置无效区域*/
 //void DrawInvaildRect(HXRECT hXRect) {
	// if (hXRect == NULL) {
	//	 GUISendDrawMsg(NULL, MSG_WIN_INVAILD_UPDATE, 0, 0, 0, LCD_SCREEN_W, LCD_SCREEN_H 
	//		// 0, 0, LCD_SCREEN_W, LCD_SCREEN_H
	//	 );
	// }
	// else {
	//	 GUISendDrawMsg(NULL, MSG_WIN_INVAILD_UPDATE, 0, hXRect->x, hXRect->y, hXRect->w, hXRect->h
	//		 //,  hXRect->x, hXRect->y, hXRect->w, hXRect->h
	//	 );
	// }
	//
 //}
 ///*事件处理程序使用*/
 //void _DrawInvaildRect(HXRECT hXRect) {
	//XRECT_COPY(&(hXDesktop->drawArea), hXRect);
 //}
 /*设置允许绘图的窗口，是当前窗口与桌面的相交区域*/
void DrawSetArea(void* hObject) {
	HWIDGE_BASE hWidgeBase;
	XRECT rRect;
	hWidgeBase = hObject;
	if (!hWidgeBase) { return; }

	/*控件矩形与桌面矩形的相交矩形即为绘图区域*/
	GetOverLapRect(&(hXDesktop->drawArea),&(hWidgeBase->rect),&rRect);
	//GetOverLapRect(&(((HWIDGE_BASE)(hWidgeBase->parentHWIN))->pencil),&(hWidgeBase->rect),&rRect);

	/*重新设置绘图区域*/
	XRECT_COPY(&(hXDesktop->drawArea), &rRect);

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

	XRECT_COPY(&(hXDesktop->drawArea), hPencil);
}
//extern void Vsrefresh();
uint8 DrawPixel(uintColor color,int16 x, int16 y) {
	d_pix(x, y, RGB565TORGB888(color));
	//Vsrefresh();
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

//
//void delay_ms(int n) {
//	int i, j;
//	for (i = 0; i < n; i++) {
//		for (j = 0; j < 1000000; j++);
//	}
//}


/*在规定区域内绘制矩形*/
uint8 DrawRect(HPENCIL hPencil, HXRECT hXRECT) {
	XRECT rRect;
	if (!hPencil) {
		return (uint8)0;
	}
	GetOverLapRect(hXRECT, (HXRECT)hPencil, &rRect);
	fill_rect(rRect.x, rRect.y, rRect.w, rRect.h, RGB565TORGB888(hPencil->DrawColor));
	//fill_rect(rRect.x, rRect.y, rRect.w, rRect.h, RGB565TORGB888(rand()%65535));

	return (uint8)1;
}

/*绘制二进制图片,宽和高只能是8的倍数*/
void DrawBitmapBinary(HXRECT drawBorder, HXPOINT startDrawPT, HXBITMAP hXBitmap) {
	uint16 i, j;
	uint16 pixel;
	uint16 draw_w;
	uint16 draw_h;
	if (drawBorder == NULL || startDrawPT == NULL || hXBitmap == NULL) {
		return;
	}
	draw_w = MIN(drawBorder->w,hXBitmap->w);
	draw_h = MIN(drawBorder->h,hXBitmap->h);

	if (hXBitmap->bitsPerPixel == 1) {
		for (i = 0; i < draw_h; i++) {
			for (j = 0; j < draw_w; j++) {
				uint16 temp = (i+startDrawPT->y) * hXBitmap->w +
					(j+startDrawPT->x);//当前像素的位置
				pixel = hXBitmap->pixels[temp / 8]&((1<< (7 - (temp % 8))));
				if (pixel) {
					DrawPixel(0x0000, j+ drawBorder->x, i+ drawBorder->y);
				}
				else {
					DrawPixel(0xffff, j+ drawBorder->x, i+ drawBorder->y);
				}
			}
		}
	}
}
/*
* 在规定区域内绘制图片
* hPencil 画布
* border 绘图的边界
* bgBorder 控件的边界
* hXBitmap 绘制的图片
*/
uint8 DrawBitmap(HPENCIL hPencil, HXRECT border,HXRECT bgBorder,HXBITMAP hXBitmap) {
	XRECT rRect;/*可以绘制的区域*/
	XRECT rRect1;/*可以绘制的区域*/
	XRECT temp;
	if (!hPencil) {
		return (uint8)0;
	}
	GetOverLapRect(border, (HXRECT)hPencil, &rRect);
	temp.x = bgBorder->x;
	temp.y = bgBorder->y;
	temp.w = hXBitmap->w;
	temp.h = hXBitmap->h;

	GetOverLapRect(&rRect, (HXRECT)&temp, &rRect1);

	XPOINT xPoint;
	xPoint.x = rRect.x - bgBorder->x;
	xPoint.y = rRect.y - bgBorder->y;
	switch (hXBitmap->bitsPerPixel) {
	case 1:
		DrawBitmapBinary(&rRect1, &xPoint, hXBitmap);
		break;
	case 16:
		d_bmp(
			rRect1.x, rRect1.y, /*实际图片绘制的起点*/
			rRect1.w, rRect1.h, /*需要绘制的图片的大小*/
			hXBitmap->pixels,
			rRect.x - bgBorder->x, rRect.y - bgBorder->y, /*从图片中绘制的开始位置*/
			hXBitmap->w, hXBitmap->h
		);
		break;
	}
	return TRUE;
}

/*矩形剪裁绘制*/
uint8 DrawCutRect(void* hObject, HXRECT hXRECT) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase) { return (uint8)0; }
	/*循环绘制剪裁后的矩形*/
	RECT_CUT_INIT(hXRECT) {
		//delay_ms(1000);
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}
	RECT_CUT_END()

	return (uint8)1;
}

/*
* 在规定区域内绘制字符串
*/
uint8 DrawString(HPENCIL hPencil, HFONTF hFont, HXRECT border, int16 x, int16 y, uint8* text) {
	XRECT rRect;
	uint16 i;
	int16 stx;
	int16 sty;
	int16 endy;
	int16 endx;
	if (!hPencil) {
		return (uint8)0;
	}

	GetOverLapRect(border, (HXRECT)hPencil, &rRect);

	stx = MAX(rRect.x, x);
	sty = MAX(rRect.y, y);
	endy = MIN((hFont->fontInfo.h) + y, rRect.y + rRect.h);
	endx = MIN((int16)(TStrlen(text) * (hFont->fontInfo.w)) + x, rRect.x + rRect.w);

	for (i = 0; text[i]; i++) {
		uint8* chData;
		chData = FontReadChar(hFont, text[i]);
		if (chData != NULL) {
			int16 i, j;
			for (i = y; i < MIN(hFont->fontInfo.h + y, endy); i++) {
				if (i >= sty) {
					for (j = x; j < MIN(hFont->fontInfo.w + x, endx); j++) {
						if (j >= stx) {
							uint16 tCHData = hFont->fontInfo.perRowBytes == 2 ? ((uint16*)chData)[i - y] : chData[i - y];
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

static uint8 _DrawChar(HPENCIL hPencil, HXRECT hXRectArea, HFONTF hFont, HXPOINT startPoint, uint8 ch) {
	uint8* chData;
	if (!hXRectArea) {
		return (uint8)0;
	}
	chData = FontReadChar(hFont, ch);
	if (chData != NULL) {
		uint16 i, j;
		uint16 pixel;
		uint16 draw_w;
		uint16 draw_h;
		if (hXRectArea == NULL || startPoint == NULL ) {
			return 0;
		}
		draw_w = MIN(hXRectArea->w, hFont->fontInfo.w);
		draw_h = MIN(hXRectArea->h, hFont->fontInfo.h);

		for (i = 0; i < draw_h; i++) {
			for (j = 0; j < draw_w; j++) {
				uint16 temp = (i + startPoint->y) * hFont->fontInfo.w +
					(j + startPoint->x);//当前像素的位置
				pixel = chData[temp / 8] & ((1 << ( (temp % 8))));
				if (pixel) {
					DrawPixel(hPencil->DrawColor, j + hXRectArea->x, i + hXRectArea->y);
				}
				else {
					DrawPixel(hPencil->DrawBkColor, j + hXRectArea->x, i + hXRectArea->y);
				}
			}
		}

	}
	return (uint8)1;
}


uint8 _DrawCharEx(HXRECT bgRect, HXRECT drawRect, HXPOINT hStartXPoint, HFONTF hFont, char ch, HPENCIL hPencil) {
	uint16 i, j;
	uint16 pixel;
	uint8* chData;
	uint16 draw_w;
	uint16 draw_h;
	uint16 sDrawX;
	uint16 sDrawY;

	chData = FontReadChar(hFont, ch);
	if (chData != NULL) {

		sDrawX = bgRect->x + hStartXPoint->x;
		sDrawY = bgRect->y + hStartXPoint->y;

		draw_w = hFont->fontInfo.w;//MIN(drawRect->w, hFont->fontInfo.w);
		draw_h = hFont->fontInfo.h;//MIN(drawRect->h, hFont->fontInfo.h);

		for (i = drawRect->y; i < drawRect->y + drawRect->h; i++) {
			for (j = drawRect->x; j < drawRect->x + drawRect->w; j++) {
				if (j >= sDrawX && i >= sDrawY
					&& j < sDrawX + draw_w && i < sDrawY + draw_h
					) {

					uint16 temp = (i - sDrawY) * hFont->fontInfo.w +
						(j - sDrawX);//当前像素的位置
					pixel = chData[temp / 8] & ((1 << ((temp % 8))));
					if (pixel) {
						DrawPixel(hPencil->DrawColor,
							j, i );
					}else {
						DrawPixel(hPencil->DrawBkColor,
							j , i );
					}
				}
				else {
					DrawPixel(hPencil->DrawBkColor,
						j , i);
				}
			}
		}
	}

	return 1;
}
uint8 _DrawStringEx(HXRECT bgRect, HXRECT drawRect, HXPOINT hStartXPoint, HFONTF hFont, const char * text, HPENCIL hPencil) {
	//int6 drawX=0;
	int16 i, j;
	uint16 pixel;
	uint8* chData;
	uint16 draw_w;
	uint16 draw_h;
	int16 sDrawX;
	int16 sDrawY;
	uint16 m;
	/*for (m = 0; text[m]; m++) {
		chData = FontReadChar(hFont, text[m]);
		if (chData != NULL) {*/

			sDrawX = bgRect->x + hStartXPoint->x;
			sDrawY = bgRect->y + hStartXPoint->y;

			draw_w = hFont->fontInfo.w * TStrlen(text);//MIN(drawRect->w, hFont->fontInfo.w);
			draw_h = hFont->fontInfo.h;//MIN(drawRect->h, hFont->fontInfo.h);

			for (i = drawRect->y; i < drawRect->y + drawRect->h; i++) {
				for (j = drawRect->x; j < drawRect->x + drawRect->w; j++) {
					if (j >= sDrawX && i >= sDrawY
						&& j < sDrawX + draw_w 
						&& i < sDrawY + draw_h
						) {
						uint16 temp = ((i - sDrawY)% hFont->fontInfo.h) * hFont->fontInfo.w +
							((j - sDrawX)% hFont->fontInfo.w);//当前像素的位置
						uint16 index = (j - sDrawX)/ hFont->fontInfo.w;//当前像素的位置

							//* hFont->fontInfo.w +
							//((j - sDrawX) );//当前像素的位置

						chData = FontReadChar(hFont, text[index]);
						pixel = chData[temp / 8] & ((1 << ((temp % 8))));
						if (pixel) {
							DrawPixel(hPencil->DrawColor,
								j, i);
						}
						else {
							DrawPixel(hPencil->DrawBkColor,
								j, i);
						}
				
					}
					else {
						DrawPixel(hPencil->DrawBkColor,
							j, i);
					}
				}
			}
		//}
	//}
	//Vsrefresh();
	return 1;
}
uint8 DrawStringEx(HXRECT bgBorder, HXRECT drawBorder, HXPOINT hStartXPoint, HFONTF hFont, const char* text, HPENCIL hPencil) {
	XRECT rRect;/*可以绘制的区域*/
	XRECT rRect1;/*可以绘制的区域*/
	XRECT temp;
	if (!hPencil) {
		return (uint8)0;
	}
	GetOverLapRect(drawBorder, (HXRECT)hPencil, &rRect);

	_DrawStringEx(bgBorder, &rRect, hStartXPoint, hFont, text, hPencil);
	return TRUE;
}

uint8 DrawCharEx(HXRECT bgBorder, HXRECT drawBorder, HXPOINT hStartXPoint,  HFONTF hFont,  char ch, HPENCIL hPencil) {
	XRECT rRect;/*可以绘制的区域*/
	XRECT rRect1;/*可以绘制的区域*/
	XRECT temp;
	if (!hPencil) {
		return (uint8)0;
	}
	GetOverLapRect(drawBorder, (HXRECT)hPencil, &rRect);

	_DrawCharEx(bgBorder, &rRect,  hStartXPoint, hFont, ch, hPencil);
	return TRUE;
}

uint8 DrawChar(HPENCIL hPencil, HFONTF hFont,HXRECT border, HXRECT bgBorder, char ch) {
	XRECT rRect;/*可以绘制的区域*/
	XRECT rRect1;/*可以绘制的区域*/
	XRECT temp;
	if (!hPencil) {
		return (uint8)0;
	}
	GetOverLapRect(border, (HXRECT)hPencil, &rRect);
	temp.x = bgBorder->x;
	temp.y = bgBorder->y;
	temp.w = hFont->fontInfo.w;
	temp.h = hFont->fontInfo.h;

	GetOverLapRect(&rRect, (HXRECT)& temp, &rRect1);

	XPOINT xPoint;
	xPoint.x = rRect.x - bgBorder->x;
	xPoint.y = rRect.y - bgBorder->y;

	_DrawChar(hPencil, &rRect1, hFont, &xPoint, ch);
	return TRUE;
}

/*字绘制*/
uint8 DrawCutChar(void* hObject, HFONTF hFont, HXRECT bgRect, HXPOINT hStartXPoint, char ch) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase) { return (uint8)FALSE; }
	RECT_CUT_INIT(bgRect) {
		DrawCharEx(bgRect, nextCutRect, hStartXPoint, hFont, ch, &(hWidgeBase->pencil));
	}
	RECT_CUT_END()
		return (int8)TRUE;
}

/*字符串剪裁绘制*/
uint8 DrawCutString(void* hObject, HFONTF hFont, HXRECT border, HXPOINT hXPoint, uint8* text) {
	XPOINT startPoint;
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase) { return (uint8)FALSE; }

	startPoint.x = 0;
	startPoint.y = 0;

	RECT_CUT_INIT(border) {
	//	DrawString(&(hWidgeBase->pencil), hFont, nextCutRect, !hXPoint ? border->x : hXPoint->x, !hXPoint ? border->y : hXPoint->y, text);
		DrawStringEx(border, nextCutRect, hXPoint==0?&startPoint: hXPoint, hFont, text, &(hWidgeBase->pencil));

	}
	RECT_CUT_END()
	return (int8)TRUE;
}

/*图片剪裁绘制*/
uint8 DrawCutBitmap(void* hObject, HXRECT border, HXBITMAP hXBitmap) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase) { return (uint8)0; }
	/*循环绘制剪裁后的矩形*/
	RECT_CUT_INIT(border) {
		DrawBitmap(&(hWidgeBase->pencil), nextCutRect, border, hXBitmap);
	}
	RECT_CUT_END()

	return (uint8)1;
}
