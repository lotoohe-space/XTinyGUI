#include "paint.h"
#include "widge.h"
#include "x_malloc.h"
#include "gui.h"
#include "paint_cut.h"

PUBLIC p_widget_base_t WidgetCreate(int16_t x, int16_t y, int16_t w, int16_t h)
{
	p_widget_base_t hWidgeBase = (p_widget_base_t)(xMalloc(sizeof(widget_base_t)));
	if (hWidgeBase == NULL)
	{
		return NULL;
	}
	WidgetInit(hWidgeBase, x, y, w, h);
	return hWidgeBase;
}
PUBLIC p_widget_base_t WidgetCreateEx(p_xrect_t hXRect)
{
	if (hXRect == NULL)
	{
		return NULL;
	}
	return WidgetCreate(hXRect->x, hXRect->y, hXRect->w, hXRect->h);
}
PUBLIC void WidgetInit(p_widget_base_t hWidgeBase, int16_t x, int16_t y, int16_t w, int16_t h)
{
	if (hWidgeBase == NULL)
	{
		return;
	}
	/*控件大小*/
	hWidgeBase->rect.x = x;
	hWidgeBase->rect.y = y;
	hWidgeBase->rect.w = w;
	hWidgeBase->rect.h = h;

	/*设置三个回调函数*/
	hWidgeBase->paintFun = WidgetPaint;
	hWidgeBase->moveToFun = WidgetMoveTo;
	hWidgeBase->widgeCallBackFun = WidgetCallBack;
	hWidgeBase->widgeCloseFun = WidgetClose;
	hWidgeBase->widgeResizeFun = WidgetResize;
	/*设置颜色*/
	hWidgeBase->pencil.DrawColor = RGB565_BLACK;
	hWidgeBase->pencil.DrawFrColor = _DefaultFrColor;
	hWidgeBase->pencil.DrawBkColor = _DefaultBKColor;

	/*默认绘图区域为全部*/
	hWidgeBase->pencil.x = x;
	hWidgeBase->pencil.y = y;
	hWidgeBase->pencil.w = w;
	hWidgeBase->pencil.h = h;

	hWidgeBase->parentHWIN = NULL;

	hWidgeBase->viewClickCallBack = NULL;
	hWidgeBase->arg = NULL;

	hWidgeBase->flag = 0;

	/*当前控件不是窗口*/
	_SET_IS_WIN(hWidgeBase);
	/*默认可见*/
	_SetVisable(hWidgeBase);
	/*默认不透明处理*/
	_CLR_IS_DPY(hWidgeBase);
}
PUBLIC void WidgetClose(p_widget_base_t hObject)
{
	if (hObject == NULL)
	{
		return;
	}
	xFree(hObject);
}
PUBLIC void WidgetSetClickBack(p_widget_base_t hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	if (!hObject)
	{
		return;
	}
	hObject->arg = arg;
	hObject->viewClickCallBack = viewClickCallBack;
}
/*设置是否可见*/
PUBLIC void WidgetSetVisable(p_widget_base_t hObject, uint8_t isVisable)
{
	if (!hObject)
	{
		return;
	}
	if (isVisable)
	{
		_SetVisable(hObject);
	}
	else
	{
		_ClrVisable(hObject);
	}
	WindowsInvaildRect(hObject->parentHWIN, (p_xrect_t)hObject);
}
/*重新设置大小*/
PUBLIC void WidgetResize(p_widget_base_t hObject, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	if (!hObject)
	{
		return;
	}
	hObject->rect.x = x;
	hObject->rect.y = y;
	hObject->rect.w = w;
	hObject->rect.h = h;
	// WindowsInvaildRect(((p_widget_base_t)hObject)->parentHWIN, NULL);
}
/*设置父控件*/
PUBLIC void WidgetSetParentWin(p_widget_base_t hObject, void *hWIN)
{
	if (!hObject)
	{
		return;
	}
	hObject->parentHWIN = hWIN;
}
/*设置前景色*/
PUBLIC void WidgetSetColor(p_widget_base_t hObject, uintColor color)
{
	if (!hObject)
	{
		return;
	}
	hObject->pencil.DrawColor = color;
	WindowsInvaildRect(hObject->parentHWIN, (p_xrect_t)hObject);
}
/*移动控件*/
PUBLIC void WidgetMoveTo(p_widget_base_t hObject, int16_t x, int16_t y)
{
	if (!hObject)
	{
		return;
	}
	hObject->rect.x = x;
	hObject->rect.y = y;
	// WindowsInvaildRect(hObject->parentHWIN, (p_xrect_t)hObject);
}
/*重绘函数*/
PRIVATE void WidgetPaint(void *hObject)
{
	p_widget_base_t hWidgeBase;
	hWidgeBase = hObject;
	if (!hObject)
	{
		return;
	}
	if (!_GetVisable(hWidgeBase))
	{
		return;
	}
	// if (!IsGUINeedCutEx(hWidgeBase)) { return; }

	if (!DrawSetArea(hWidgeBase))
	{
		return;
	}
	hWidgeBase->pencil.DrawColor = hWidgeBase->pencil.DrawBkColor;
	// 绘制底色
	RECT_CUT_INIT(&(hWidgeBase->rect))
	{
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}
	RECT_CUT_END()

	/*恢复绘图区域*/
	DrawResetArea(hWidgeBase);
}
/*事件回调函数*/
PUBLIC int8_t WidgetCallBack(void *hObject, p_msg_t hMsg)
{
	p_widget_base_t hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg)
	{
		return RES_ASSERT_ERR;
	}
	if (!_GetVisable(hWidgeBase))
	{
		return RES_ASSERT_ERR;
	}
	if (hMsg->msgType == MSG_TOUCH)
	{

		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, (&(hWidgeBase->rect))))
		{
			switch (hMsg->msgID)
			{
			case MSG_TOUCH_MOVE:
				break;
			case MSG_TOUCH_PRESS:
				/*按下*/
				if (hWidgeBase->viewClickCallBack != NULL)
				{
					hWidgeBase->viewClickCallBack(hWidgeBase, hWidgeBase->arg, _GetBtnStatus(hWidgeBase));
				}
				_SetBtnPress(hWidgeBase);
				WindowsInvaildRect(WIDGE_PARENT(hWidgeBase), (p_xrect_t)hWidgeBase);
				break;
			case MSG_TOUCH_RELEASE:
				/*松开*/
				if (_GetBtnStatus(hWidgeBase))
				{
					if (hWidgeBase->viewClickCallBack != NULL)
					{
						hWidgeBase->viewClickCallBack(hWidgeBase, hWidgeBase->arg, _GetBtnStatus(hWidgeBase));
					}
					_SetBtnRelease(hWidgeBase);
					WindowsInvaildRect(WIDGE_PARENT(hWidgeBase), (p_xrect_t)hWidgeBase);
				}
				break;
			}
			return RES_OK;
		}
		else
		{
			/*松开*/
			if (_GetBtnStatus(hWidgeBase))
			{
				if (hWidgeBase->viewClickCallBack != NULL)
				{
					hWidgeBase->viewClickCallBack(hWidgeBase, hWidgeBase->arg, _GetBtnStatus(hWidgeBase));
				}
				_SetBtnRelease(hWidgeBase);
				WindowsInvaildRect(WIDGE_PARENT(hWidgeBase), (p_xrect_t)hWidgeBase);
			}
		}
	}
	return RES_FAIL;
}
