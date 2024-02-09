#include "paint.h"
#include "widge.h"
#include "x_malloc.h"
#include "gui.h"
#include "paint_cut.h"

PUBLIC HWIDGET_BASE _WIDGET(Create)(int16 x, int16 y, int16 w, int16 h)
{
	HWIDGET_BASE hWidgeBase = (HWIDGET_BASE)(xMalloc(sizeof(WIDGET_BASE)));
	if (hWidgeBase == NULL)
	{
		return NULL;
	}
	_WIDGET(Init)
	(hWidgeBase, x, y, w, h);
	return hWidgeBase;
}
PUBLIC HWIDGET_BASE _WIDGET(CreateEx)(HXRECT hXRect)
{
	if (hXRect == NULL)
	{
		return NULL;
	}
	return _WIDGET(Create)(hXRect->x, hXRect->y, hXRect->w, hXRect->h);
}
PUBLIC void _WIDGET(Init)(HWIDGET_BASE hWidgeBase, int16 x, int16 y, int16 w, int16 h)
{
	if (hWidgeBase == NULL)
	{
		return;
	}
	/*�ؼ���С*/
	hWidgeBase->rect.x = x;
	hWidgeBase->rect.y = y;
	hWidgeBase->rect.w = w;
	hWidgeBase->rect.h = h;

	/*���������ص�����*/
	hWidgeBase->paintFun = _WIDGET(Paint);
	hWidgeBase->moveToFun = _WIDGET(MoveTo);
	hWidgeBase->widgeCallBackFun = _WIDGET(CallBack);
	hWidgeBase->widgeCloseFun = _WIDGET(Close);
	hWidgeBase->widgeResizeFun = _WIDGET(Resize);
	/*������ɫ*/
	hWidgeBase->pencil.DrawColor = RGB565_BLACK;
	hWidgeBase->pencil.DrawFrColor = _DefaultFrColor;
	hWidgeBase->pencil.DrawBkColor = _DefaultBKColor;

	/*Ĭ�ϻ�ͼ����Ϊȫ��*/
	hWidgeBase->pencil.x = x;
	hWidgeBase->pencil.y = y;
	hWidgeBase->pencil.w = w;
	hWidgeBase->pencil.h = h;

	hWidgeBase->parentHWIN = NULL;

	hWidgeBase->viewClickCallBack = NULL;
	hWidgeBase->arg = NULL;

	hWidgeBase->flag = 0;

	/*��ǰ�ؼ����Ǵ���*/
	_SET_IS_WIN(hWidgeBase);
	/*Ĭ�Ͽɼ�*/
	_SetVisable(hWidgeBase);
	/*Ĭ�ϲ�͸������*/
	_CLR_IS_DPY(hWidgeBase);
}
PUBLIC void _WIDGET(Close)(HWIDGET_BASE hObject)
{
	if (hObject == NULL)
	{
		return;
	}
	xFree(hObject);
}
PUBLIC void _WIDGET(SetClickBack)(HWIDGET_BASE hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	if (!hObject)
	{
		return;
	}
	hObject->arg = arg;
	hObject->viewClickCallBack = viewClickCallBack;
}
/*�����Ƿ�ɼ�*/
PUBLIC void _WIDGET(SetVisable)(HWIDGET_BASE hObject, uint8 isVisable)
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
	WindowsInvaildRect(hObject->parentHWIN, (HXRECT)hObject);
}
/*�������ô�С*/
PUBLIC void _WIDGET(Resize)(HWIDGET_BASE hObject, int16 x, int16 y, uint16 w, uint16 h)
{
	if (!hObject)
	{
		return;
	}
	hObject->rect.x = x;
	hObject->rect.y = y;
	hObject->rect.w = w;
	hObject->rect.h = h;
	// WindowsInvaildRect(((HWIDGET_BASE)hObject)->parentHWIN, NULL);
}
/*���ø��ؼ�*/
PUBLIC void _WIDGET(SetParentWin)(HWIDGET_BASE hObject, void *hWIN)
{
	if (!hObject)
	{
		return;
	}
	hObject->parentHWIN = hWIN;
}
/*����ǰ��ɫ*/
PUBLIC void _WIDGET(SetColor)(HWIDGET_BASE hObject, uintColor color)
{
	if (!hObject)
	{
		return;
	}
	hObject->pencil.DrawColor = color;
	WindowsInvaildRect(hObject->parentHWIN, (HXRECT)hObject);
}
/*�ƶ��ؼ�*/
PUBLIC void _WIDGET(MoveTo)(HWIDGET_BASE hObject, int16 x, int16 y)
{
	if (!hObject)
	{
		return;
	}
	hObject->rect.x = x;
	hObject->rect.y = y;
	// WindowsInvaildRect(hObject->parentHWIN, (HXRECT)hObject);
}
/*�ػ溯��*/
PRIVATE void _WIDGET(Paint)(void *hObject)
{
	HWIDGET_BASE hWidgeBase;
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
	// ���Ƶ�ɫ
	RECT_CUT_INIT(&(hWidgeBase->rect))
	{
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}
	RECT_CUT_END()

	/*�ָ���ͼ����*/
	DrawResetArea(hWidgeBase);
}
/*�¼��ص�����*/
PUBLIC int8 _WIDGET(CallBack)(void *hObject, HMSGE hMsg)
{
	HWIDGET_BASE hWidgeBase = hObject;
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
				/*����*/
				if (hWidgeBase->viewClickCallBack != NULL)
				{
					hWidgeBase->viewClickCallBack(hWidgeBase, hWidgeBase->arg, _GetBtnStatus(hWidgeBase));
				}
				_SetBtnPress(hWidgeBase);
				WindowsInvaildRect(WIDGE_PARENT(hWidgeBase), (HXRECT)hWidgeBase);
				break;
			case MSG_TOUCH_RELEASE:
				/*�ɿ�*/
				if (_GetBtnStatus(hWidgeBase))
				{
					if (hWidgeBase->viewClickCallBack != NULL)
					{
						hWidgeBase->viewClickCallBack(hWidgeBase, hWidgeBase->arg, _GetBtnStatus(hWidgeBase));
					}
					_SetBtnRelease(hWidgeBase);
					WindowsInvaildRect(WIDGE_PARENT(hWidgeBase), (HXRECT)hWidgeBase);
				}
				break;
			}
			return RES_OK;
		}
		else
		{
			/*�ɿ�*/
			if (_GetBtnStatus(hWidgeBase))
			{
				if (hWidgeBase->viewClickCallBack != NULL)
				{
					hWidgeBase->viewClickCallBack(hWidgeBase, hWidgeBase->arg, _GetBtnStatus(hWidgeBase));
				}
				_SetBtnRelease(hWidgeBase);
				WindowsInvaildRect(WIDGE_PARENT(hWidgeBase), (HXRECT)hWidgeBase);
			}
		}
	}
	return RES_FAIL;
}
