#include "group_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "radio_button_widge.h"

PUBLIC p_group_widget_t GROUP_MARK_HEAD(Create)(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	p_group_widget_t hObject;

	hObject = (p_group_widget_t)(xMalloc(sizeof(group_widget_t)));
	if (hObject == NULL)
	{
		return NULL;
	}
	GROUP_MARK_HEAD(Init)
	(hObject, x, y, w, h);
	return hObject;
}
PUBLIC uint8_t GROUP_MARK_HEAD(Init)(p_group_widget_t hBaseWidge, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	WidgetInit((p_widget_base_t)hBaseWidge, x, y, w, h);
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
PUBLIC void GROUP_MARK_HEAD(Close)(p_group_widget_t hObject)
{
	if (!hObject)
	{
		return;
	}
	/*�������ͷŴ����е��ڴ�*/

	/*����ÿһ���ؼ�*/
	_StartScanU(_PToHGroupWidgeType(hObject)->widgetList)
	{ // ��ʼɨ��
		p_widget_base_t hWidge = val;
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
	WidgetClose((p_widget_base_t)hObject);
}
/*��ȡĳ��λ�õĿؼ�*/
PUBLIC p_widget_base_t GROUP_MARK_HEAD(GetWidge)(p_group_widget_t hObject, uint16_t index)
{
	if (!hObject)
	{
		return NULL;
	}
	p_rlist_t hlist = ListGet(hObject->widgetList, index);
	if (hlist != NULL)
	{
		return (p_widget_base_t)(hlist->val);
	}
	else
	{
		return NULL;
	}
}
/*�������ô��ڵĴ�С,����Ҫ�����ڲ��ؼ��Ĵ�С��λ��*/
PUBLIC void GROUP_MARK_HEAD(Resize)(p_group_widget_t hObject, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	if (!hObject)
	{
		return;
	}

	int16_t dx, dy;
	/*����λ�øı��ƫ��*/
	dx = x - ((p_xrect_t)hObject)->x;
	dy = y - ((p_xrect_t)hObject)->y;

	WidgetResize((p_widget_base_t)hObject, x, y, w, h);

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
	// WindowsInvaildRect(((p_widget_base_t)hObject)->parentHWIN, NULL);
}
/*ɾ��һ���ؼ�*/
PUBLIC uint8_t GROUP_MARK_HEAD(Del)(p_group_widget_t hBaseWidge, p_widget_base_t widge)
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
	WindowsInvaildRect(((p_widget_base_t)hBaseWidge), NULL);
	return TRUE;
}
/*���һ���ؼ�*/
PUBLIC uint8_t GROUP_MARK_HEAD(Add)(p_group_widget_t hBaseWidge, p_widget_base_t widge)
{
	if (!hBaseWidge || !widge)
	{
		return FALSE;
	}
	p_rlist_t addItem = ListNew();
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
PUBLIC void GROUP_MARK_HEAD(GroupProcess)(p_group_widget_t hBaseWidge, p_widget_base_t currentWidge)
{
	if (hBaseWidge == NULL)
	{
		return;
	}
	_StartScanU(hBaseWidge->widgetList)
	{ /*��ʼɨ��*/
		p_widget_base_t hWidgeBase = (p_widget_base_t)(val);

		if (_GET_GROUP_CTRL(hWidgeBase))
		{
			/*��Ҫ��������*/
			if (currentWidge != hWidgeBase)
			{
				/*����RADIO_BUTTON�ĺ���*/
				RADIO_BUTTON_MARK_HEAD(SetStatus)
				((p_radio_button_widget_t)hWidgeBase, FALSE);
			}
		}
	}
	_EndScanU(); /*����ɨ��*/
}
PUBLIC void GROUP_MARK_HEAD(Paint)(void *hObject)
{
	p_group_widget_t hBaseWidge;
	p_rlist_t cutPostionList;
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
	if (!DrawSetArea((p_widget_base_t)hBaseWidge))
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
	DrawResetArea((p_widget_base_t)hBaseWidge);
}
PUBLIC void GROUP_MARK_HEAD(MoveTo)(p_group_widget_t hObject, int16_t x, int16_t y)
{
	int16_t dx = 0;
	int16_t dy = 0;
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
	p_rlist_t hWidgeList = hObject->widgetList;
	if (hWidgeList != NULL)
	{
		hWidgeList = hWidgeList->next;
		while (hWidgeList)
		{
			p_widget_base_t hWidge = (p_widget_base_t)(hWidgeList->val);
			/*��������ÿһ���ؼ���λ��*/
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);

			hWidgeList = hWidgeList->next;
		}
	}
}
/*�¼��ص�����*/
PUBLIC int8_t GROUP_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg)
{
	p_group_widget_t hBaseWidge = hObject;
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
			p_rlist_t hWidgeList = hBaseWidge->widgetList;
			if (hWidgeList)
			{
				int8_t ret;
				p_rlist_t lastWidge = ListGetLast(hWidgeList);
				if (lastWidge != NULL)
				{
					while (lastWidge != hWidgeList)
					{
						p_widget_base_t hWidge = (p_widget_base_t)(lastWidge->val);
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