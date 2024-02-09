#include "paint.h"
#include "bitmap_widge.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"

PUBLIC p_bitmap_widget_t BITMAP_MARK_HEAD(Create)(int16_t x, int16_t y, int16_t w, int16_t h, p_xbitmap_t hXBitmap)
{
	p_bitmap_widget_t hBitmapWidge = (p_bitmap_widget_t)(xMalloc(sizeof(bitmap_widget_t)));
	if (hBitmapWidge == NULL)
	{
		return NULL;
	}
	WidgetInit((p_widget_base_t)hBitmapWidge, x, y, w, h);

	/*设置三个回调函数*/
	hBitmapWidge->widgeBase.paintFun = BITMAP_MARK_HEAD(Paint);
	hBitmapWidge->widgeBase.moveToFun = BITMAP_MARK_HEAD(MoveTo);
	hBitmapWidge->widgeBase.widgeCallBackFun = BITMAP_MARK_HEAD(CallBack);

	/*设置显示的图片*/
	hBitmapWidge->bitmap = hXBitmap;

	return hBitmapWidge;
}
PUBLIC void BITMAP_MARK_HEAD(SetBitmap)(p_bitmap_widget_t hObject, p_xbitmap_t hXBitmap)
{
	if (!hObject)
	{
		return;
	}
	/*设置显示的图片*/
	hObject->bitmap = hXBitmap;
	WindowsInvaildRect((p_widget_base_t)hObject, NULL);
}
PUBLIC void BITMAP_MARK_HEAD(MoveTo)(p_bitmap_widget_t hObject, int16_t x, int16_t y)
{
	WidgetMoveTo((p_widget_base_t)hObject, x, y);
	// if (!hObject) { return; }
	// hObject->widgeBase.rect.x = x;
	// hObject->widgeBase.rect.y = y;
}

PUBLIC void BITMAP_MARK_HEAD(SetParentWin)(p_bitmap_widget_t hObject, p_win_t hWIN)
{
	if (!hObject)
	{
		return;
	}
	hObject->widgeBase.parentHWIN = hWIN;
}
PUBLIC void BITMAP_MARK_HEAD(SetColor)(p_bitmap_widget_t hObject, uintColor color)
{
	if (!hObject)
	{
		return;
	}
	hObject->widgeBase.pencil.DrawColor = color;
}
PUBLIC void BITMAP_MARK_HEAD(Paint)(void *hObject)
{
	p_bitmap_widget_t hBitmapWidge;
	hBitmapWidge = hObject;
	if (!hObject)
	{
		return;
	}
	if (!_GetVisable(hBitmapWidge))
	{
		return;
	}
	// if (!IsGUINeedCut(hBitmapWidge)) { return; }

	if (!DrawSetArea((p_widget_base_t)hBitmapWidge))
	{
		return;
	}

	DrawCutBitmap(hBitmapWidge, (p_xrect_t)hBitmapWidge, hBitmapWidge->bitmap);
	/*恢复绘图区域*/
	DrawResetArea((p_widget_base_t)hBitmapWidge);
}

int8_t BITMAP_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg)
{
	p_widget_base_t hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg)
	{
		return -1;
	}
	if (!_GetVisable(hWidgeBase))
	{
		return -1;
	}
	if (hMsg->msgType == MSG_TOUCH)
	{
		if (_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, hWidgeBase->pencil.x, hWidgeBase->pencil.y, hWidgeBase->pencil.w, hWidgeBase->pencil.h))
		{
			return 1;
		}
	}
	return -1;
}
