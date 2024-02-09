#include "list_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"

static void GUITimeoutCb(void *arg);

PUBLIC p_xlist_widget_t LIST_WIDGE_MARK_HEAD(Create)(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	p_xlist_widget_t hObject;

	hObject = (p_xlist_widget_t)(XMalloc(sizeof(xlist_widget_t)));
	if (hObject == NULL)
	{
		return NULL;
	}
	GROUP_MARK_HEAD(Init)
	((p_group_widget_t)hObject, x, y, w, h);

	_PToHGroupWidgeType(hObject)->widgeBase.paintFun = LIST_WIDGE_MARK_HEAD(Paint);
	//_PToHGroupWidgeType(hObject)->widgeBase.moveToFun = GROUP_MARK_HEAD(MoveTo);
	_PToHGroupWidgeType(hObject)->widgeBase.widgeCallBackFun = LIST_WIDGE_MARK_HEAD(CallBack);
	_PToHGroupWidgeType(hObject)->widgeBase.widgeCloseFun = LIST_WIDGE_MARK_HEAD(Close);

	_PToHWidgeBaseType(hObject)->pencil.DrawColor = _PToHWidgeBaseType(hObject)->pencil.DrawBkColor;

	hObject->itemsSize.itemSize = LIST_WIDGE_ITEM_SIZE;
	hObject->lastXY = 0;
	hObject->flag = 0;
	hObject->itemGap = 5;
	hObject->dSP = 0;
	hObject->hGUITimeout = GUITimeoutCreate(17, hObject, GUITimeoutCb);
	GUITimeoutClose(hObject->hGUITimeout);

	/*����Ӧģʽ*/
	_SetListWidgeMode(hObject);
	/*Ĭ�ϴ�ֱ����*/
	_SetOTN_V(hObject);
	/*�򿪶���*/
	//_OpenListAnimation(hObject);
	hObject->itemsSize.itemsH = 0;

	return hObject;
}
PUBLIC void LIST_WIDGE_MARK_HEAD(SetFlag)(p_xlist_widget_t hBaseWidge, LIST_WIDGE_FLAG flagBisPoi, BOOL status)
{
	if (hBaseWidge == NULL)
	{
		return;
	}

	hBaseWidge->flag = ((hBaseWidge->flag) & (~(1 << flagBisPoi))) | ((status & 0x01) << flagBisPoi);
}
PUBLIC void LIST_WIDGE_MARK_HEAD(Close)(p_xlist_widget_t hObject)
{
	if (!hObject)
	{
		return;
	}
	GUITimeoutFree(hObject->hGUITimeout);
	GROUP_MARK_HEAD(Close)
	((p_group_widget_t)hObject);
}
/*���һ���ؼ�*/
PUBLIC uint8_t LIST_WIDGE_MARK_HEAD(Add)(p_xlist_widget_t hBaseWidge, p_widget_base_t widge)
{
	uint16_t widgeLength;
	if (hBaseWidge == NULL || widge == NULL)
	{
		return FALSE;
	}

	if (GROUP_MARK_HEAD(Add)((p_group_widget_t)hBaseWidge, widge) == FALSE)
	{
		return FALSE;
	}
	widgeLength = ((p_group_widget_t)hBaseWidge)->widgeLength - 1;
	if (!_GetListWidgeMode(hBaseWidge))
	{ /*����ģʽ*/
		if (_GetOTN(hBaseWidge))
		{
			/*�������ÿؼ�����Ϣ*/
			((p_xrect_t)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x +
								 hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
			((p_xrect_t)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y;
		}
		else
		{
			((p_xrect_t)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
			((p_xrect_t)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y +
								 hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
		}
		((p_xrect_t)widge)->h = hBaseWidge->itemsSize.itemSize;
		((p_xrect_t)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
	}
	else
	{ /*�߶�����Ӧ*/
		/*�������ÿؼ�����Ϣ*/
		if (_GetOTN(hBaseWidge))
		{
			((p_xrect_t)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x +
								 hBaseWidge->itemsSize.itemsH + hBaseWidge->itemGap * widgeLength;
			((p_xrect_t)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y;
			hBaseWidge->itemsSize.itemsH += ((p_xrect_t)widge)->w;
			//((p_xrect_t)widge)->h = hBaseWidge->groupWidge.widgeBase.rect.h;
		}
		else
		{
			((p_xrect_t)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
			((p_xrect_t)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y +
								 hBaseWidge->itemsSize.itemsH + hBaseWidge->itemGap * widgeLength;
			hBaseWidge->itemsSize.itemsH += ((p_xrect_t)widge)->h;
			//((p_xrect_t)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
		}
	}

	/*ˢ��*/
	WindowsInvaildRect((p_widget_base_t)hBaseWidge, NULL);
	return TRUE;
}
/*ListWidge�ڲ��ؼ���λ�û���*/
PRIVATE uint8_t LIST_WIDGE_MARK_HEAD(ListSlide)(p_xlist_widget_t hBaseWidge, int16_t dXY)
{
	p_xrect_t widgeRectLast;
	p_xrect_t widgeRectFirst;

	if (!hBaseWidge)
	{
		return FALSE;
	}
	/*��ȡ��һ�ؼ�*/
	widgeRectFirst = (p_xrect_t)GROUP_MARK_HEAD(GetWidge)((p_group_widget_t)hBaseWidge, 0);
	if (widgeRectFirst == NULL)
	{
		return FALSE;
	}
	/*��ȡ���һ���ؼ�*/
	widgeRectLast = (p_xrect_t)GROUP_MARK_HEAD(GetWidge)((p_group_widget_t)hBaseWidge, hBaseWidge->groupWidge.widgeLength - 1);
	if (widgeRectLast == NULL)
	{
		return FALSE;
	}
	if (_GetOTN(hBaseWidge))
	{ /*����*/
		if (widgeRectLast->x + dXY + widgeRectLast->w < ((p_xrect_t)(hBaseWidge))->x + ((p_xrect_t)(hBaseWidge))->w)
		{
			// return FALSE;
			dXY = ((p_xrect_t)(hBaseWidge))->x + ((p_xrect_t)(hBaseWidge))->w - (widgeRectLast->x + widgeRectLast->w);
		}
		if (widgeRectFirst->x + dXY > ((p_xrect_t)(hBaseWidge))->x)
		{
			dXY = ((p_xrect_t)(hBaseWidge))->x - widgeRectFirst->x;
		}
	}
	else
	{ /*����*/
		if (widgeRectLast->y + dXY + widgeRectLast->h < ((p_xrect_t)(hBaseWidge))->y + ((p_xrect_t)(hBaseWidge))->h)
		{
			dXY = (((p_xrect_t)(hBaseWidge))->y + ((p_xrect_t)(hBaseWidge))->h) - (widgeRectLast->y + widgeRectLast->h);
		}
		if (widgeRectFirst->y + dXY > ((p_xrect_t)(hBaseWidge))->y)
		{
			dXY = ((p_xrect_t)(hBaseWidge))->y - widgeRectFirst->y;
		}
	}
	/*ÿ���ؼ�����ƫ��*/
	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList)
	{ /*��ʼɨ��*/
		p_xrect_t hWidge = (p_xrect_t)_PToHWidgeBaseType(val);
		if (_GetOTN(hBaseWidge))
		{ /*����*/
			hWidge->x += dXY;
		}
		else
		{ /*����*/
			hWidge->y += dXY;
		}
	}
	_EndScanU(); /*����ɨ��*/
	WindowsInvaildRect(_PToHWidgeBaseType(hBaseWidge), (p_xrect_t)hBaseWidge);
	return TRUE;
}
static void GUITimeoutCb(void *arg)
{
	p_xlist_widget_t hBaseWidge = arg;
	if (arg == NULL)
	{
		return;
	}
	if (_GetListAnimation(hBaseWidge))
	{ /*�Ƿ������˶���*/
		if (!(hBaseWidge->dSP >= -1 && hBaseWidge->dSP <= 1))
		{
			if (!LIST_WIDGE_MARK_HEAD(ListSlide)(hBaseWidge, hBaseWidge->dSP))
			{
				hBaseWidge->dSP = 0;
			}
			else
			{
				if (hBaseWidge->dSP < 0)
				{
					hBaseWidge->dSP++;
					if (hBaseWidge->dSP > 0)
					{
						hBaseWidge->dSP = 0;
					}
				}
				else if (hBaseWidge->dSP > 0)
				{
					hBaseWidge->dSP--;
					if (hBaseWidge->dSP < 0)
					{
						hBaseWidge->dSP = 0;
					}
				}
				GUITimeoutOpen(hBaseWidge->hGUITimeout);
			}
		}
	}
}
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void *hObject)
{
	p_xlist_widget_t hBaseWidge;
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
	if (!IsGUINeedCut((p_widget_base_t)hBaseWidge))
	{
		return;
	}
	if (!DrawSetArea((p_widget_base_t)hBaseWidge))
	{
		return;
	} // ����õ���ǰ��ͼ����

	// ����õ���������
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hBaseWidge)->widgetList->next);
	DrawCutRect(hBaseWidge,
				&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList)
	{																/*��ʼɨ��*/
		cutPostionList = RectCutAddRectList(tempItem->next);		/*�����ڸǵļ��о���*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val)); /*���Ʋ������ͼ����*/
		RectCutSplitRectList(cutPostionList);						/*ȥ���ڸǵļ��о���*/
																	// DrawResetArea(hBaseWidge);/*�ָ���ͼ����*/
	}
	_EndScanU(); /*����ɨ��*/

	/*�ָ���ͼ����*/
	DrawResetArea((p_widget_base_t)hBaseWidge);
}
/*�¼��ص�����*/
PUBLIC int8_t LIST_WIDGE_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg)
{
	p_xlist_widget_t hBaseWidge = hObject;
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
							   &(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect)))
		{
			int16_t dXY = 0;
			/*�����¼�*/
			if (hMsg->msgType == MSG_TOUCH)
			{
				switch (hMsg->msgID)
				{
				case MSG_TOUCH_PRESS:
					if (_GetOTN(hBaseWidge))
					{ /*����*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.x;

						/*�����˶����Ż��õ��������*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.x;
					}
					else
					{ /*����*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.y;

						/*�����˶����Ż��õ��������*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.y;
					}
					if (_GetListAnimation(hBaseWidge))
					{
						/*��������˶�������¼��ʱ���µ�ʱ��*/
						hBaseWidge->firstTime = (uint16_t)GUIGetTick();
					}
					break;
				case MSG_TOUCH_MOVE:
					if (_GetOTN(hBaseWidge))
					{ /*����*/
						/*������Ҫƫ�Ƶ�*/
						dXY = hMsg->msgVal.rect.x - hBaseWidge->lastXY;
					}
					else
					{ /*����*/
						dXY = hMsg->msgVal.rect.y - hBaseWidge->lastXY;
					}
					if (dXY != 0)
					{
						_SetSliding(hBaseWidge);
					}
					if (_GetSlideState(hBaseWidge))
					{
						LIST_WIDGE_MARK_HEAD(ListSlide)
						(hBaseWidge, dXY);
						if (_GetOTN(hBaseWidge))
						{ /*����*/
							hBaseWidge->lastXY = hMsg->msgVal.rect.x;
						}
						else
						{
							hBaseWidge->lastXY = hMsg->msgVal.rect.y;
						}
						return RES_OK;
					}
					break;
				case MSG_TOUCH_RELEASE:
					if (_GetSlideState(hBaseWidge))
					{
						if (_GetListAnimation(hBaseWidge))
						{ /*��������˶���*/
							int16_t dis = 0;
							if (_GetOTN(hBaseWidge))
							{ /*����*/
								dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
							}
							else
							{ /*����*/
								dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
							}

							/*�����ϴβ�ֵ�����ٶ�*/
							hBaseWidge->dSP = 25 * (dis) / ((uint16_t)((uint16_t)GUIGetTick() - hBaseWidge->firstTime));
							if (hBaseWidge->dSP != 0)
							{
								GUITimeoutOpen(hBaseWidge->hGUITimeout);
							}
						}
						_ClrSliding(hBaseWidge);
						return RES_OK;
					}
					break;
				}
			}

			/*�¼����ݵ�ÿһ���ؼ�*/
			p_rlist_t hWidgeList = _PToHGroupWidgeType(hBaseWidge)->widgetList;
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
		else
		{ /*������*/
			if (_GetSlideState(hBaseWidge))
			{
				if (_GetListAnimation(hBaseWidge))
				{ /*��������˶���*/
					int16_t dis = 0;
					if (_GetOTN(hBaseWidge))
					{ /*����*/
						dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
					}
					else
					{ /*����*/
						dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
					}

					/*�����ϴβ�ֵ�����ٶ�*/
					hBaseWidge->dSP = (dis) / ((uint16_t)((uint16_t)GUIGetTick() - hBaseWidge->firstTime));
					if (hBaseWidge->dSP != 0)
					{
						GUITimeoutOpen(hBaseWidge->hGUITimeout);
					}
				}
				_ClrSliding(hBaseWidge);
				return RES_OK;
			}
		}
	}
	return -1;
}
