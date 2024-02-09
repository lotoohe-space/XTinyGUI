#pragma once

#ifndef _BITMAP_WIDGE_H__
#define _BITMAP_WIDGE_H__
#include "x_widget.h"
#include "bitmap.h"
#include "xwindows.h"
#define BITMAP_MARK_HEAD(a) BitmapWidge##a

typedef struct
{
	widget_base_t widgeBase;
	p_xbitmap_t bitmap; /*显示的图片*/
} *p_bitmap_widget_t, bitmap_widget_t;

PUBLIC p_bitmap_widget_t BITMAP_MARK_HEAD(Create)(int16_t x, int16_t y, int16_t w, int16_t h, p_xbitmap_t hXBitmap);
PUBLIC void BITMAP_MARK_HEAD(SetBitmap)(p_bitmap_widget_t hObject, p_xbitmap_t hXBitmap);
PUBLIC void BITMAP_MARK_HEAD(MoveTo)(p_bitmap_widget_t hObject, int16_t x, int16_t y);
PUBLIC void BITMAP_MARK_HEAD(SetParentWin)(p_bitmap_widget_t hObject, p_win_t hWIN);
PUBLIC void BITMAP_MARK_HEAD(SetColor)(p_bitmap_widget_t hObject, uintColor color);
PUBLIC void BITMAP_MARK_HEAD(Paint)(void *hObject);
int8_t BITMAP_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg);

#endif
