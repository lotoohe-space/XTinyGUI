#pragma once

#ifndef _BITMAP_WIDGE_H__
#define _BITMAP_WIDGE_H__
#include "x_widget.h"
#include "bitmap.h"
#include "xwindows.h"

#define BITMAP_MARK_HEAD(a) BitmapWidget##a

typedef struct
{
	widget_base_t widgeBase;
	p_xbitmap_t bitmap; /*显示的图片*/
} *p_bitmap_widget_t, bitmap_widget_t;

PUBLIC p_bitmap_widget_t BitmapWidgetCreate(int16_t x, int16_t y, int16_t w, int16_t h, p_xbitmap_t hXBitmap);
PUBLIC void BitmapWidgetSetBitmap(p_bitmap_widget_t hObject, p_xbitmap_t hXBitmap);
PUBLIC void BitmapWidgetMoveTo(p_bitmap_widget_t hObject, int16_t x, int16_t y);
PUBLIC void BitmapWidgetSetParentWin(p_bitmap_widget_t hObject, p_win_t hWIN);
PUBLIC void BitmapWidgetSetColor(p_bitmap_widget_t hObject, uintColor color);
PUBLIC void BitmapWidgetPaint(void *hObject);
int8_t BitmapWidgetCallBack(void *hObject, p_msg_t hMsg);

#endif
