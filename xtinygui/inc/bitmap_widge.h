#pragma once

#ifndef _BITMAP_WIDGE_H__
#define _BITMAP_WIDGE_H__
#include "widge.h"
#include "bitmap.h"
#include "xwindows.h"
#define BITMAP_MARK_HEAD(a) BitmapWidge##a

typedef struct
{
	WIDGET_BASE widgeBase;
	HXBITMAP bitmap; /*显示的图片*/
} *HBITMAP_WIDGE, BITMAP_WIDGE;

PUBLIC HBITMAP_WIDGE BITMAP_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, HXBITMAP hXBitmap);
PUBLIC void BITMAP_MARK_HEAD(SetBitmap)(HBITMAP_WIDGE hObject, HXBITMAP hXBitmap);
PUBLIC void BITMAP_MARK_HEAD(MoveTo)(HBITMAP_WIDGE hObject, int16 x, int16 y);
PUBLIC void BITMAP_MARK_HEAD(SetParentWin)(HBITMAP_WIDGE hObject, HWIN hWIN);
PUBLIC void BITMAP_MARK_HEAD(SetColor)(HBITMAP_WIDGE hObject, uintColor color);
PUBLIC void BITMAP_MARK_HEAD(Paint)(void *hObject);
int8 BITMAP_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
