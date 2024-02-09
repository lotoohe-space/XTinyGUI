
#include "xwindows_head.h"
#include "x_malloc.h"
#include "x_tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "text_widge.h"
#include "button_widge.h"

/*����WINDOWS Head�ؼ�*/
PUBLIC p_win_head_t WindowsHeadWidgeCreate(char *title, int16_t x, int16_t y, int16_t w, int16_t h)
{
	p_win_head_t hWinHead;
	p_text_widget_t hTextWidge;

	hWinHead = (p_win_head_t)(XMalloc(sizeof(win_head_t)));
	if (hWinHead == NULL)
	{
		return NULL;
	}
	hTextWidge = TextWidgetCreate(title ? title : _DefaultWinHeadName, 0, 0, w, h);
	if (hTextWidge == NULL)
	{
		XFree(hWinHead);
		return NULL;
	}
	hTextWidge->baseWidge.pencil.DrawBkColor = RGB565_GEN(255, 255, 255);

	hWinHead->hXButtonMin = BUTTON_MARK_HEAD(Create)("-",
													 w - h, 0, h, h);

	if (hWinHead->hXButtonMin == NULL)
	{
		XFree(hWinHead);
		_PToHWidgeBaseType(hTextWidge)->widgeCloseFun(hTextWidge);
		return NULL;
	}
	//_PToHTextWidgeType(hWinHead->hXButtonMin)->hFont = (p_font_t)& fontUNICODE16_16;

	hWinHead->hXButtonMax = BUTTON_MARK_HEAD(Create)("*",
													 w - 2 * h, 0, h, h);
	if (hWinHead->hXButtonMax == NULL)
	{
		XFree(hWinHead);
		_PToHWidgeBaseType(hTextWidge)->widgeCloseFun(hTextWidge);
		_PToHWidgeBaseType(hWinHead->hXButtonMin)->widgeCloseFun(hTextWidge);
		return NULL;
	}
	//_PToHTextWidgeType(hWinHead->hXButtonMax)->hFont = (p_font_t)& fontUNICODE16_16;

	hWinHead->hXButtonClose = BUTTON_MARK_HEAD(Create)("+",
													   w - 3 * h, 0, h, h);
	if (hWinHead->hXButtonClose == NULL)
	{
		XFree(hWinHead);
		_PToHWidgeBaseType(hTextWidge)->widgeCloseFun(hTextWidge);
		_PToHWidgeBaseType(hWinHead->hXButtonMin)->widgeCloseFun(hTextWidge);
		_PToHWidgeBaseType(hWinHead->hXButtonMax)->widgeCloseFun(hTextWidge);
		return NULL;
	}
	//_PToHTextWidgeType(hWinHead->hXButtonClose)->hFont = (p_font_t)& fontUNICODE16_16;

	// �ؼ��б�
	_PToHGroupWidgeType(hWinHead)->widgetList = ListNew();
	if (_PToHGroupWidgeType(hWinHead)->widgetList == NULL)
	{
		XFree(hWinHead);
	}
	WidgetInit((p_widget_base_t)hWinHead, x, y, w, h);

	_PToHGroupWidgeType(hWinHead)->widgeBase.paintFun = WindowsHeadWidgePaint;
	_PToHGroupWidgeType(hWinHead)->widgeBase.moveToFun = WindowsHeadWidgeMoveTo;
	_PToHGroupWidgeType(hWinHead)->widgeBase.widgeCallBackFun = WindowsHeadWidgeCallBack;
	_PToHGroupWidgeType(hWinHead)->widgeBase.widgeCloseFun = WindowsHeadWidgeClose;

	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawColor = RGB565_BLACK;
	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawFrColor = _DefaultFrColor;
	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawBkColor = RGB565_GEN(255, 255, 255);

	/*���ͷ����*/
	WindowsHeadWidgeAdd(hWinHead, hTextWidge);
	/*�����С��BUTTON*/
	WindowsHeadWidgeAdd(hWinHead, hWinHead->hXButtonMin);
	/*������BUTTON*/
	WindowsHeadWidgeAdd(hWinHead, hWinHead->hXButtonMax);
	WindowsHeadWidgeAdd(hWinHead, hWinHead->hXButtonClose);

	return hWinHead;
}
/*��ȡ�����е�ĳ��λ�õĿؼ����õ��Ĳ��Ǵ��ڶ��Ǵ��ڵ�ListItem*/
PUBLIC p_widget_base_t WindowsHeadWidgeGetWidge(p_win_head_t hObject, uint16_t index)
{
	if (!hObject)
	{
		return NULL;
	}
	p_rlist_t hlist = ListGet(_PToHGroupWidgeType(hObject)->widgetList, index);
	if (hlist != NULL)
	{
		return (p_widget_base_t)(hlist->val);
	}
	else
	{
		return NULL;
	}
}
/*�������ô��ڵĴ�С,����Ҫ�����ڲ��ؼ��Ĵ�С��λ��,λ����������ڿؼ��е�λ��*/
PUBLIC void WindowsHeadWidgeResize(p_win_head_t hObject, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	p_widget_base_t hWidgeBase;
	if (!hObject)
	{
		return;
	}
	WidgetResize((p_widget_base_t)hObject, x, y, w, h);

	/*����*/
	hWidgeBase = WindowsHeadWidgeGetWidge(hObject, 0);
	if (hWidgeBase == NULL)
	{
		return;
	}
	WidgetResize(hWidgeBase, x, y, w, h);

	/*��С����ť*/
	hWidgeBase = WindowsHeadWidgeGetWidge(hObject, 1);
	if (hWidgeBase == NULL)
	{
		return;
	}
	WidgetResize(hWidgeBase, x + w - h, y, h, h);

	/*��󻯰�ť*/
	hWidgeBase = WindowsHeadWidgeGetWidge(hObject, 2);
	if (hWidgeBase == NULL)
	{
		return;
	}
	WidgetResize(hWidgeBase, x + (w - 2 * h), y, h, h);

	/*��󻯰�ť*/
	hWidgeBase = WindowsHeadWidgeGetWidge(hObject, 3);
	if (hWidgeBase == NULL)
	{
		return;
	}
	WidgetResize(hWidgeBase, x + (w - 3 * h), y, h, h);

	/*ˢ�¸�����*/
	WindowsInvaildRect(((p_widget_base_t)hObject)->parentHWIN, NULL);
}
// �ر�
PUBLIC void WindowsHeadWidgeClose(p_win_head_t hWinHead)
{
	if (!hWinHead)
	{
		return;
	}
	/*�������ͷŴ����е��ڴ�*/

	/*�ͷű����е�ռ�õ��ڴ�*/

	/*���ø���Ĺرպ���*/
	GROUP_MARK_HEAD(Close)
	((p_group_widget_t)hWinHead);
}
// ���һ���ؼ�
PUBLIC int8_t WindowsHeadWidgeAdd(p_win_head_t hWinHead, void *widge)
{
	if (!hWinHead || !widge)
	{
		return -1;
	}
	p_rlist_t addItem = ListNew();
	if (!addItem)
	{
		return -1;
	}
	addItem->val = widge;
	if (ListAddLast(_PToHGroupWidgeType(hWinHead)->widgetList, addItem) == -1)
	{
		return -1;
	}
	p_widget_base_t hWidge = widge;
	hWidge->parentHWIN = hWinHead;
	hWidge->moveToFun(hWidge,
					  hWidge->rect.x + _PToHGroupWidgeType(hWinHead)->widgeBase.rect.x,
					  hWidge->rect.y + _PToHGroupWidgeType(hWinHead)->widgeBase.rect.y);

	return 0;
}
/*������С����ť�Ļص��ӿ�*/
PUBLIC void WindowsHeadWidgeSetMinimumBtnClickBack(p_win_head_t hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	WidgetSetClickBack((p_widget_base_t)(hObject->hXButtonMin), arg, viewClickCallBack);
}
/*������󻯰�ť�Ļص��ӿ�*/
PUBLIC void WindowsHeadWidgeSetMaximumBtnClickBack(p_win_head_t hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	WidgetSetClickBack((p_widget_base_t)(hObject->hXButtonMax), arg, viewClickCallBack);
}
/*���ùرհ�ť�Ļص��ӿ�*/
PUBLIC void WindowsHeadWidgeSetCloseBtnClickBack(p_win_head_t hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	WidgetSetClickBack((p_widget_base_t)(hObject->hXButtonClose), arg, viewClickCallBack);
}
PUBLIC void WindowsHeadWidgeMoveTo(p_win_head_t hObject, int16_t x, int16_t y)
{
	int16_t dx = 0;
	int16_t dy = 0;
	if (!hObject)
	{
		return;
	}
	dx = x - (_PToHGroupWidgeType(hObject)->widgeBase.rect.x);
	dy = y - (_PToHGroupWidgeType(hObject)->widgeBase.rect.y);
	_PToHGroupWidgeType(hObject)->widgeBase.rect.x = x;
	_PToHGroupWidgeType(hObject)->widgeBase.rect.y = y;
	// ��������ÿһ���ؼ�λ��
	p_rlist_t hWidgeList = _PToHGroupWidgeType(hObject)->widgetList;

	if (hWidgeList != NULL)
	{
		hWidgeList = hWidgeList->next;
		while (hWidgeList)
		{
			p_widget_base_t hWidge = (p_widget_base_t)(hWidgeList->val);
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);
			hWidgeList = hWidgeList->next;
		}
	}
}
PUBLIC void WindowsHeadWidgeSetArea(p_win_head_t hObject, int16_t x, int16_t y, int16_t w, int16_t h)
{
	if (!hObject)
	{
		return;
	}
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.x = x;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.y = y;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.w = w;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.h = h;
}
PUBLIC void WindowsHeadWidgeSetVisable(void *hObject, int8_t isVisable)
{
	WidgetSetVisable(hObject, isVisable);
}
PRIVATE void WindowsHeadWidgePaint(void *hObject)
{
	p_win_head_t hWinHead;
	p_rlist_t cutPostionList;
	hWinHead = hObject;
	if (!hWinHead)
	{
		return;
	}
	if (!_GetVisable(hWinHead))
	{
		return;
	}
	// if (!isGUINeedCut(hWinHead)) { return; }
	if (!DrawSetArea((p_widget_base_t)hWinHead))
	{
		return;
	} // ����õ���ǰ��ͼ����
	// ����õ���������
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hWinHead)->widgetList->next);
	DrawCutRect(hWinHead,
				&(_PToHGroupWidgeType(hWinHead)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hWinHead)->widgetList)
	{																// ��ʼɨ��
		cutPostionList = RectCutAddRectList(tempItem->next);		// �����ڸǵļ��о���
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val)); // ���Ʋ������ͼ����
		RectCutSplitRectList(cutPostionList);						// ȥ���ڸǵļ��о���
																	// DrawResetArea(hWinHead);//�ָ���ͼ����
	}
	_EndScanU(); // ����ɨ��

	/*�ָ���ͼ����*/
	DrawResetArea((p_widget_base_t)hWinHead);
}
PUBLIC int8_t WindowsHeadWidgeCallBack(void *hObject, p_msg_t hMsg)
{
	p_win_head_t hWinHead = hObject;
	if (!hWinHead || !hMsg)
	{
		return RES_ASSERT_ERR;
	}
	if (!_GetVisable(hWinHead))
	{
		return RES_ASSERT_ERR;
	}
	if (hMsg->msgType == MSG_TOUCH)
	{
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
							   &(_PToHGroupWidgeType(hWinHead)->widgeBase.rect)))
		{
			p_rlist_t hWidgeList = _PToHGroupWidgeType(hWinHead)->widgetList;
			if (hWidgeList)
			{
				int8_t ret;
				p_rlist_t lastWidge = ListGetLast(hWidgeList);
				if (lastWidge != NULL)
				{
					while (lastWidge != hWidgeList)
					{
						p_widget_base_t hWidge = (p_widget_base_t)(lastWidge->val);
						if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == RES_OK)
						{
							return RES_OK;
						}
						lastWidge = lastWidge->lnext;
					}
				}
			}
			return RES_OK;
		}
	}
	return RES_FAIL;
}
