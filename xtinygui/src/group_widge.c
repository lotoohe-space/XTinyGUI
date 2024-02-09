#include "group_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "radio_button_widge.h"

PUBLIC HGROUP_WIDGE GROUP_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h)
{
	HGROUP_WIDGE hObject;

	hObject = (HGROUP_WIDGE)(xMalloc(sizeof(GROUP_WIDGE)));
	if (hObject == NULL)
	{
		return NULL;
	}
	GROUP_MARK_HEAD(Init)
	(hObject, x, y, w, h);
	return hObject;
}
PUBLIC uint8 GROUP_MARK_HEAD(Init)(HGROUP_WIDGE hBaseWidge, int16 x, int16 y, uint16 w, uint16 h)
{
	_WIDGET(Init)
	((HWIDGET_BASE)hBaseWidge, x, y, w, h);
	/*�ؼ��б�*/
	hBaseWidge->widgetList = ListNew();
	if (hBaseWidge->widgetList == NULL)
	{
		return FALSE;
	}
	hBaseWidge->widgeBase.paintFun = GROUP_MARK_HEAD(Paint);
	hBaseWidge->widgeBase.moveToFun = GROUP_MARK_HEAD(MoveTo);
	hBaseWidge->widgeBase.widgeCallBackFun = GROUP_MARK_HEAD(CallBack);
	hBaseWidge->widgeBase.widgeResizeFun = GROUP_MARK_HEAD(Resize);
	hBaseWidge->widgeBase.widgeCloseFun = GROUP_MARK_HEAD(Close);

	hBaseWidge->widgeLength = 0;

	return TRUE;
}
PUBLIC void GROUP_MARK_HEAD(Close)(HGROUP_WIDGE hObject)
{
	if (!hObject)
	{
		return;
	}
	/*�������ͷŴ����е��ڴ�*/

	/*����ÿһ���ؼ�*/
	_StartScanU(_PToHGroupWidgeType(hObject)->widgetList)
	{ // ��ʼɨ��
		HWIDGET_BASE hWidge = val;
		/*����ÿһ���ؼ��Ĺرպ���*/
		hWidge->widgeCloseFun(hWidge);
	}
	/*����ɨ��*/
	_EndScanU();

	/*�ͷŵ�List*/
	ListClear(_PToHGroupWidgeType(hObject)->widgetList);
	_PToHGroupWidgeType(hObject)->widgeLength = 0;
	/*ɾ��Listͷ*/
	ListDel(_PToHGroupWidgeType(hObject)->widgetList);

	/*���ø��ؼ��ͷź���*/
	_WIDGET(Close)
	((HWIDGET_BASE)hObject);
}
/*��ȡĳ��λ�õĿؼ�*/
PUBLIC HWIDGET_BASE GROUP_MARK_HEAD(GetWidge)(HGROUP_WIDGE hObject, uint16 index)
{
	if (!hObject)
	{
		return NULL;
	}
	HLIST hlist = ListGet(hObject->widgetList, index);
	if (hlist != NULL)
	{
		return (HWIDGET_BASE)(hlist->val);
	}
	else
	{
		return NULL;
	}
}
/*�������ô��ڵĴ�С,����Ҫ�����ڲ��ؼ��Ĵ�С��λ��*/
PUBLIC void GROUP_MARK_HEAD(Resize)(HGROUP_WIDGE hObject, int16 x, int16 y, uint16 w, uint16 h)
{
	if (!hObject)
	{
		return;
	}

	int16 dx, dy;
	/*����λ�øı��ƫ��*/
	dx = x - ((HXRECT)hObject)->x;
	dy = y - ((HXRECT)hObject)->y;

	_WIDGET(Resize)
	((HWIDGET_BASE)hObject, x, y, w, h);

	/*����ÿһ���ؼ�*/
	_StartScanU(hObject->widgetList)
	{ // ��ʼɨ��
		_PToHWidgeBaseType(val)->widgeResizeFun(
			_PToHWidgeBaseType(val),
			_PToHWidgeBaseType(val)->rect.x + dx,
			_PToHWidgeBaseType(val)->rect.y + dy,
			_PToHWidgeBaseType(val)->rect.w,
			_PToHWidgeBaseType(val)->rect.h);
	}
	/*����ɨ��*/
	_EndScanU();
	/*�ı䴰�ڴ�С����ˢ�¸��ؼ�*/
	// WindowsInvaildRect(((HWIDGET_BASE)hObject)->parentHWIN, NULL);
}
/*ɾ��һ���ؼ�*/
PUBLIC uint8 GROUP_MARK_HEAD(Del)(HGROUP_WIDGE hBaseWidge, HWIDGET_BASE widge)
{
	if (hBaseWidge == NULL || widge == NULL)
	{
		return FALSE;
	}
	/*��list��ɾ��*/
	if (ListDelByVal(hBaseWidge->widgetList, widge) == FALSE)
	{
		return FALSE;
	}
	hBaseWidge->widgeLength--;
	/*���ÿؼ��Ĺرպ���*/
	widge->widgeCloseFun(widge);
	WindowsInvaildRect(((HWIDGET_BASE)hBaseWidge), NULL);
	return TRUE;
}
/*���һ���ؼ�*/
PUBLIC uint8 GROUP_MARK_HEAD(Add)(HGROUP_WIDGE hBaseWidge, HWIDGET_BASE widge)
{
	if (!hBaseWidge || !widge)
	{
		return FALSE;
	}
	HLIST addItem = ListNew();
	if (!addItem)
	{
		return FALSE;
	}
	addItem->val = widge;
	/*��ӵ�List�Ľ�β*/
	if (ListAddLast(hBaseWidge->widgetList, addItem) == -1)
	{
		/*ʧ�����ͷ�֮ǰ������ڴ�*/
		ListDel(addItem);
		return FALSE;
	}
	/*���ø��ؼ�*/
	widge->parentHWIN = hBaseWidge;

	/*�ƶ��ؼ���ָ��λ��*/
	widge->moveToFun(widge,
					 widge->rect.x + hBaseWidge->widgeBase.rect.x,
					 widge->rect.y + hBaseWidge->widgeBase.rect.y);
	/*���ӳ���*/
	hBaseWidge->widgeLength++;
	return TRUE;
}
PUBLIC void GROUP_MARK_HEAD(GroupProcess)(HGROUP_WIDGE hBaseWidge, HWIDGET_BASE currentWidge)
{
	if (hBaseWidge == NULL)
	{
		return;
	}
	_StartScanU(hBaseWidge->widgetList)
	{ /*��ʼɨ��*/
		HWIDGET_BASE hWidgeBase = (HWIDGET_BASE)(val);

		if (_GET_GROUP_CTRL(hWidgeBase))
		{
			/*��Ҫ��������*/
			if (currentWidge != hWidgeBase)
			{
				/*����RADIO_BUTTON�ĺ���*/
				RADIO_BUTTON_MARK_HEAD(SetStatus)
				((HRADIO_BUTTON_WIDGE)hWidgeBase, FALSE);
			}
		}
	}
	_EndScanU(); /*����ɨ��*/
}
PUBLIC void GROUP_MARK_HEAD(Paint)(void *hObject)
{
	HGROUP_WIDGE hBaseWidge;
	HLIST cutPostionList;
	hBaseWidge = hObject;
	if (!hBaseWidge)
	{
		return;
	}
	if (!_GetVisable(hBaseWidge))
	{
		return;
	}
	// if (!IsGUINeedCut(hWinHead)) { return; }
	if (!DrawSetArea((HWIDGET_BASE)hBaseWidge))
	{
		return;
	} // ����õ���ǰ��ͼ����
	/*����õ���������*/
	cutPostionList = RectCutAddRectList(hBaseWidge->widgetList->next);
	DrawCutRect(hBaseWidge,
				&(hBaseWidge->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(hBaseWidge->widgetList)
	{																/*��ʼɨ��*/
		cutPostionList = RectCutAddRectList(tempItem->next);		/*�����ڸǵļ��о���*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val)); /*���Ʋ������ͼ����*/
		RectCutSplitRectList(cutPostionList);						/*ȥ���ڸǵļ��о���*/
	}
	_EndScanU(); /*����ɨ��*/

	/*�ָ���ͼ����*/
	DrawResetArea((HWIDGET_BASE)hBaseWidge);
}
PUBLIC void GROUP_MARK_HEAD(MoveTo)(HGROUP_WIDGE hObject, int16 x, int16 y)
{
	int16 dx = 0;
	int16 dy = 0;
	if (!hObject)
	{
		return;
	}
	/*����ƫ��λ��*/
	dx = x - (hObject->widgeBase.rect.x);
	dy = y - (hObject->widgeBase.rect.y);
	/*��������λ��*/
	hObject->widgeBase.rect.x = x;
	hObject->widgeBase.rect.y = y;
	/*��������ÿһ���ؼ�λ��*/
	HLIST hWidgeList = hObject->widgetList;
	if (hWidgeList != NULL)
	{
		hWidgeList = hWidgeList->next;
		while (hWidgeList)
		{
			HWIDGET_BASE hWidge = (HWIDGET_BASE)(hWidgeList->val);
			/*��������ÿһ���ؼ���λ��*/
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);

			hWidgeList = hWidgeList->next;
		}
	}
}
/*�¼��ص�����*/
PUBLIC int8 GROUP_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg)
{
	HGROUP_WIDGE hBaseWidge = hObject;
	if (!hBaseWidge || !hMsg)
	{
		return -1;
	}
	if (!_GetVisable(hBaseWidge))
	{
		return -1;
	}
	if (hMsg->msgType == MSG_TOUCH)
	{
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
							   &(hBaseWidge->widgeBase.rect)))
		{
			HLIST hWidgeList = hBaseWidge->widgetList;
			if (hWidgeList)
			{
				int8 ret;
				HLIST lastWidge = ListGetLast(hWidgeList);
				if (lastWidge != NULL)
				{
					while (lastWidge != hWidgeList)
					{
						HWIDGET_BASE hWidge = (HWIDGET_BASE)(lastWidge->val);
						if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == 0)
						{
							return 0;
						}
						lastWidge = lastWidge->lnext;
					}
				}
			}
			return 0;
		}
	}
	return -1;
}