#include "switch_button_widge.h"
#include "paint.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"
#include "bitmap.h"

/*��������*/
PUBLIC HSWITCH_BUTTON_WIDGE SWITCH_BUTTON_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h,
																  const char *onText, const char *offText)
{
	HSWITCH_BUTTON_WIDGE hWidge = (HSWITCH_BUTTON_WIDGE)(xMalloc(sizeof(SWITCH_BUTTON_WIDGE)));
	if (hWidge == NULL)
	{
		return NULL;
	}
	TEXT_MARK_HEAD(Init)
	((HTEXT_WIDGE)hWidge, "", x, y, w, h);

	/*���������ص�����*/
	_PToHTextWidgeType(hWidge)->baseWidge.paintFun = SWITCH_BUTTON_WIDGE_MARK_HEAD(Paint);
	_PToHTextWidgeType(hWidge)->baseWidge.moveToFun = SWITCH_BUTTON_WIDGE_MARK_HEAD(MoveTo);
	_PToHTextWidgeType(hWidge)->baseWidge.widgeCallBackFun = SWITCH_BUTTON_WIDGE_MARK_HEAD(CallBack);
	// hWidge->widgeBase.widgeCloseFun = SWITCH_BUTTON_WIDGE_MARK_HEAD(Close);

	_PToHTextWidgeType(hWidge)->text = "";
	_PToHTextWidgeType(hWidge)->hFont = (HFONTF)(&fontASCII16_16);
	hWidge->flag = 0;
	hWidge->onText = onText ? onText : "ON";
	hWidge->offText = offText ? offText : "OFF";
	hWidge->onBitmap = NULL;
	hWidge->offBitmap = NULL;

	return hWidge;
}
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOnText)(HSWITCH_BUTTON_WIDGE hObject, const char *text)
{
	if (!hObject)
	{
		return;
	}
	hObject->onText = text;
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOffText)(HSWITCH_BUTTON_WIDGE hObject, const char *text)
{
	if (!hObject)
	{
		return;
	}
	hObject->offText = text;
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOnBitmap)(HSWITCH_BUTTON_WIDGE hObject, HXBITMAP hBitmap)
{
	if (!hObject)
	{
		return;
	}
	hObject->onBitmap = hBitmap;
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOffBitmap)(HSWITCH_BUTTON_WIDGE hObject, HXBITMAP hBitmap)
{
	if (!hObject)
	{
		return;
	}
	hObject->offBitmap = hBitmap;
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
/*���õ���ص�*/
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetClickBack)(HSWITCH_BUTTON_WIDGE hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	_WIDGET(SetClickBack)
	((HWIDGET_BASE)hObject, arg, viewClickCallBack);
}
/*����״̬���ڲ����õĺ���*/
PRIVATE void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetStatus)(HSWITCH_BUTTON_WIDGE hObject, uint8 status)
{
	if (!hObject)
	{
		return;
	}
	hObject->flag &= ~0x3;
	hObject->flag |= status & 0x3;
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
/*��ȥ״̬*/
PUBLIC uint8 SWITCH_BUTTON_WIDGE_MARK_HEAD(GetStatus)(HSWITCH_BUTTON_WIDGE hObject)
{
	if (!hObject)
	{
		return 0;
	}
	return (hObject->flag) & 0x3;
}
/*�ƶ�����*/
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(MoveTo)(HSWITCH_BUTTON_WIDGE hObject, int16 x, int16 y)
{
	if (!hObject)
	{
		return;
	}
	_PToHTextWidgeType(hObject)->baseWidge.rect.x = x;
	_PToHTextWidgeType(hObject)->baseWidge.rect.y = y;
	// WindowsInvaildRect(hObject->widgeBase.parentHWIN, (HXRECT)hObject);
}
/*���ø�����*/
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetParentWin)(HSWITCH_BUTTON_WIDGE hObject, HWIDGET_BASE parentWidge)
{
	if (!hObject)
	{
		return;
	}
	_PToHTextWidgeType(hObject)->baseWidge.parentHWIN = parentWidge;
}
/*������ɫ*/
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetColor)(HSWITCH_BUTTON_WIDGE hObject, uintColor color)
{
	if (!hObject)
	{
		return;
	}
	_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawColor = color;
}
/*���ƿؼ�*/
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(Paint)(void *hObject)
{
	XPOINT startPoint;
	HSWITCH_BUTTON_WIDGE hWidge;
	hWidge = hObject;
	if (!hObject)
	{
		return;
	}
	if (!_GetVisable(hWidge))
	{
		return;
	}
	// if (!IsGUINeedCut(hWidge)) { return; }
	if (!DrawSetArea((HWIDGET_BASE)hWidge))
	{
		return;
	}

	switch (SWITCH_BUTTON_WIDGE_MARK_HEAD(GetStatus)(hWidge))
	{
	case 0: /*δѡ��״̬*/
	{
		if (!_GET_SW_MODE(hWidge))
		{ /*�ı�ģʽ*/
			_PToHTextWidgeType(hWidge)->text = hWidge->offText;
			TEXT_MARK_HEAD(GetPOIByTextMode)
			((HTEXT_WIDGE)hWidge, &startPoint);
			/*����ģʽȷ�����*/
			DrawCutString(hWidge,
						  _PToHTextWidgeType(hWidge)->hFont,
						  _PToHXRECTType(hWidge),
						  &startPoint,
						  (uint8 *)(_PToHTextWidgeType(hWidge)->text));
		}
		else
		{ /*ͼƬģʽ*/
			DrawCutBitmap(hWidge, _PToHXRECTType(hWidge), hWidge->offBitmap);
		}
	}
	break;
	case 1: /*���״̬*/
		if (!_GET_SW_MODE(hWidge))
		{ /*�ı�ģʽ*/
			_PToHTextWidgeType(hWidge)->text = hWidge->onText;
			TEXT_MARK_HEAD(GetPOIByTextMode)
			((HTEXT_WIDGE)hWidge, &startPoint);
			/*����ģʽȷ�����*/
			DrawCutString(hWidge,
						  _PToHTextWidgeType(hWidge)->hFont,
						  _PToHXRECTType(hWidge),
						  &startPoint,
						  (uint8 *)(_PToHTextWidgeType(hWidge)->text));
		}
		else
		{ /*ͼƬģʽ*/
			DrawCutBitmap(hWidge, _PToHXRECTType(hWidge), hWidge->onBitmap);
		}
		break;
	case 2:
		break;
	}
	/*�ָ���ͼ����*/
	DrawResetArea((HWIDGET_BASE)hWidge);
}
/*�¼��ص�*/
int8 SWITCH_BUTTON_WIDGE_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg)
{
	HWIDGET_BASE hWidgeBase = hObject;
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

		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, &(hWidgeBase->rect)))
		{
			switch (hMsg->msgID)
			{
			case MSG_TOUCH_MOVE:
				break;
			case MSG_TOUCH_PRESS:

				break;
			case MSG_TOUCH_RELEASE:
				if (SWITCH_BUTTON_WIDGE_MARK_HEAD(GetStatus)((HSWITCH_BUTTON_WIDGE)hWidgeBase) == 1)
				{
					SWITCH_BUTTON_WIDGE_MARK_HEAD(SetStatus)
					((HSWITCH_BUTTON_WIDGE)hWidgeBase, 0);
					if (_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack != NULL)
					{
						_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack(hWidgeBase, hWidgeBase->arg, SWITCH_BUTTON_WIDGE_MARK_HEAD(GetStatus)((HSWITCH_BUTTON_WIDGE)hWidgeBase));
					}
				}
				else if (SWITCH_BUTTON_WIDGE_MARK_HEAD(GetStatus)((HSWITCH_BUTTON_WIDGE)hWidgeBase) == 0)
				{
					SWITCH_BUTTON_WIDGE_MARK_HEAD(SetStatus)
					((HSWITCH_BUTTON_WIDGE)hWidgeBase, 1);
					if (_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack != NULL)
					{
						_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack(hWidgeBase, hWidgeBase->arg, SWITCH_BUTTON_WIDGE_MARK_HEAD(GetStatus)((HSWITCH_BUTTON_WIDGE)hWidgeBase));
					}
				}
				break;
			}
			return 0;
		}
	}
	return -1;
}
