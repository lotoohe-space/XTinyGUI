#pragma once

#ifndef _BITMAP_WIDGE_H__
#define _BITMAP_WIDGE_H__
#include "widge.h"
#include "bitmap.h"
#include "xwindows.h"
#define MARK_HEAD(a) BitmapWidge##a

typedef struct {
	WIDGE_BASE bitmapWidge;

	int8 isVisable;/*是否显示*/
	
	HXBITMAP bitmap;/*显示的图片*/

}*HBITMAP_WIDGE, BITMAP_WIDGE;

PUBLIC HBITMAP_WIDGE MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, HXBITMAP hXBitmap);
PUBLIC	void MARK_HEAD(SetBitmap)(HBITMAP_WIDGE hObject, HXBITMAP hXBitmap);
PUBLIC	void MARK_HEAD(MoveTo)(HBITMAP_WIDGE hObject, int16 x, int16 y);
PUBLIC	void MARK_HEAD(SetParentWin)(HBITMAP_WIDGE hObject, HWIN hWIN);
PUBLIC	void MARK_HEAD(SetColor)(HBITMAP_WIDGE hObject, uintColor color);
PUBLIC	void MARK_HEAD(Paint)(void* hObject);
int8	MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

#endif

