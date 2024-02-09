#include "list_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"

static void GUITimeoutCb(void *arg);

PUBLIC p_xlist_widget_t LIST_WIDGE_MARK_HEAD(Create)(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	p_xlist_widget_t hObject;

	hObject = (p_xlist_widget_t)(xMalloc(sizeof(xlist_widget_t)));
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

	/*自适应模式*/
	_SetListWidgeMode(hObject);
	/*默认垂直方向*/
	_SetOTN_V(hObject);
	/*打开动画*/
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
/*添加一个控件*/
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
	{ /*定高模式*/
		if (_GetOTN(hBaseWidge))
		{
			/*重新设置控件的信息*/
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
	{ /*高度自适应*/
		/*重新设置控件的信息*/
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

	/*刷新*/
	WindowsInvaildRect((p_widget_base_t)hBaseWidge, NULL);
	return TRUE;
}
/*ListWidge内部控件的位置滑动*/
PRIVATE uint8_t LIST_WIDGE_MARK_HEAD(ListSlide)(p_xlist_widget_t hBaseWidge, int16_t dXY)
{
	p_xrect_t widgeRectLast;
	p_xrect_t widgeRectFirst;

	if (!hBaseWidge)
	{
		return FALSE;
	}
	/*获取第一控件*/
	widgeRectFirst = (p_xrect_t)GROUP_MARK_HEAD(GetWidge)((p_group_widget_t)hBaseWidge, 0);
	if (widgeRectFirst == NULL)
	{
		return FALSE;
	}
	/*获取最后一个控件*/
	widgeRectLast = (p_xrect_t)GROUP_MARK_HEAD(GetWidge)((p_group_widget_t)hBaseWidge, hBaseWidge->groupWidge.widgeLength - 1);
	if (widgeRectLast == NULL)
	{
		return FALSE;
	}
	if (_GetOTN(hBaseWidge))
	{ /*横向*/
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
	{ /*纵向*/
		if (widgeRectLast->y + dXY + widgeRectLast->h < ((p_xrect_t)(hBaseWidge))->y + ((p_xrect_t)(hBaseWidge))->h)
		{
			dXY = (((p_xrect_t)(hBaseWidge))->y + ((p_xrect_t)(hBaseWidge))->h) - (widgeRectLast->y + widgeRectLast->h);
		}
		if (widgeRectFirst->y + dXY > ((p_xrect_t)(hBaseWidge))->y)
		{
			dXY = ((p_xrect_t)(hBaseWidge))->y - widgeRectFirst->y;
		}
	}
	/*每个控件进行偏移*/
	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList)
	{ /*开始扫描*/
		p_xrect_t hWidge = (p_xrect_t)_PToHWidgeBaseType(val);
		if (_GetOTN(hBaseWidge))
		{ /*横向*/
			hWidge->x += dXY;
		}
		else
		{ /*纵向*/
			hWidge->y += dXY;
		}
	}
	_EndScanU(); /*结束扫描*/
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
	{ /*是否开启的了动画*/
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
	} // 计算得到当前绘图区域

	// 计算得到剪裁区域
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hBaseWidge)->widgetList->next);
	DrawCutRect(hBaseWidge,
				&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList)
	{																/*开始扫描*/
		cutPostionList = RectCutAddRectList(tempItem->next);		/*加入遮盖的剪切矩形*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val)); /*绘制并计算绘图区域*/
		RectCutSplitRectList(cutPostionList);						/*去掉遮盖的剪切矩形*/
																	// DrawResetArea(hBaseWidge);/*恢复绘图区域*/
	}
	_EndScanU(); /*结束扫描*/

	/*恢复绘图区域*/
	DrawResetArea((p_widget_base_t)hBaseWidge);
}
/*事件回调函数*/
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
			/*触摸事件*/
			if (hMsg->msgType == MSG_TOUCH)
			{
				switch (hMsg->msgID)
				{
				case MSG_TOUCH_PRESS:
					if (_GetOTN(hBaseWidge))
					{ /*横向*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.x;

						/*开启了动画才会用到这个变量*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.x;
					}
					else
					{ /*竖向*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.y;

						/*开启了动画才会用到这个变量*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.y;
					}
					if (_GetListAnimation(hBaseWidge))
					{
						/*如果开启了动画，记录当时按下的时间*/
						hBaseWidge->firstTime = (uint16_t)GUIGetTick();
					}
					break;
				case MSG_TOUCH_MOVE:
					if (_GetOTN(hBaseWidge))
					{ /*横向*/
						/*计算需要偏移的*/
						dXY = hMsg->msgVal.rect.x - hBaseWidge->lastXY;
					}
					else
					{ /*竖向*/
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
						{ /*横向*/
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
						{ /*如果开启了动画*/
							int16_t dis = 0;
							if (_GetOTN(hBaseWidge))
							{ /*横向*/
								dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
							}
							else
							{ /*竖向*/
								dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
							}

							/*吧与上次差值当做速度*/
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

			/*事件传递到每一个控件*/
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
		{ /*出界了*/
			if (_GetSlideState(hBaseWidge))
			{
				if (_GetListAnimation(hBaseWidge))
				{ /*如果开启了动画*/
					int16_t dis = 0;
					if (_GetOTN(hBaseWidge))
					{ /*横向*/
						dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
					}
					else
					{ /*竖向*/
						dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
					}

					/*吧与上次差值当做速度*/
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
