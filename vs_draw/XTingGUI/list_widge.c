#include "list_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"

static void GUITimeoutCb(void* arg);

PUBLIC HLIST_WIDGE LIST_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h) {
	HLIST_WIDGE hObject;

	hObject = (HLIST_WIDGE)(xMalloc(sizeof(LIST_WIDGE)));
	if (hObject == NULL) {
		return NULL;
	}
	GROUP_MARK_HEAD(Init)((HGROUP_WIDGE)hObject, x, y, w, h);

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
PUBLIC void LIST_WIDGE_MARK_HEAD(SetFlag)(HLIST_WIDGE hBaseWidge, LIST_WIDGE_FLAG flagBisPoi, BOOL status) {
	if (hBaseWidge == NULL) { return; }

	hBaseWidge->flag = ((hBaseWidge->flag) & (~(1 << flagBisPoi))) | ((status & 0x01) << flagBisPoi);
}
PUBLIC void LIST_WIDGE_MARK_HEAD(Close)(HLIST_WIDGE hObject) {
	if (!hObject) { return; }
	GUITimeoutFree(hObject->hGUITimeout);
	GROUP_MARK_HEAD(Close)((HGROUP_WIDGE)hObject);
}
/*添加一个控件*/
PUBLIC uint8 LIST_WIDGE_MARK_HEAD(Add)(HLIST_WIDGE hBaseWidge, HWIDGET_BASE widge) {
	uint16 widgeLength;
	if (hBaseWidge == NULL || widge == NULL) { return FALSE; }

	if (GROUP_MARK_HEAD(Add)((HGROUP_WIDGE)hBaseWidge, widge) == FALSE) {
		return FALSE;
	}
	widgeLength = ((HGROUP_WIDGE)hBaseWidge)->widgeLength - 1;
	if (!_GetListWidgeMode(hBaseWidge)) {/*定高模式*/
		if (_GetOTN(hBaseWidge)) {
			/*重新设置控件的信息*/
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x +
				hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y;
		}
		else {
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y +
				hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
		}
		((HXRECT)widge)->h = hBaseWidge->itemsSize.itemSize;
		((HXRECT)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
	}
	else {/*高度自适应*/
		/*重新设置控件的信息*/
		if (_GetOTN(hBaseWidge)) {
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x +
				hBaseWidge->itemsSize.itemsH + hBaseWidge->itemGap * widgeLength;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y;
			hBaseWidge->itemsSize.itemsH += ((HXRECT)widge)->w;
			//((HXRECT)widge)->h = hBaseWidge->groupWidge.widgeBase.rect.h;
		}
		else {
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y +
				hBaseWidge->itemsSize.itemsH + hBaseWidge->itemGap * widgeLength;
			hBaseWidge->itemsSize.itemsH += ((HXRECT)widge)->h;
			//((HXRECT)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
		}

	}

	/*刷新*/
	WindowsInvaildRect((HWIDGET_BASE)hBaseWidge, NULL);
	return TRUE;
}
/*ListWidge内部控件的位置滑动*/
PRIVATE uint8 LIST_WIDGE_MARK_HEAD(ListSlide)(HLIST_WIDGE hBaseWidge, int16 dXY) {
	HXRECT widgeRectLast;
	HXRECT widgeRectFirst;

	if (!hBaseWidge) { return FALSE; }
	/*获取第一控件*/
	widgeRectFirst = (HXRECT)GROUP_MARK_HEAD(GetWidge)((HGROUP_WIDGE)hBaseWidge, 0);
	if (widgeRectFirst == NULL) { return FALSE; }
	/*获取最后一个控件*/
	widgeRectLast = (HXRECT)GROUP_MARK_HEAD(GetWidge)((HGROUP_WIDGE)hBaseWidge, hBaseWidge->groupWidge.widgeLength - 1);
	if (widgeRectLast == NULL) { return FALSE; }
	if (_GetOTN(hBaseWidge)) {/*横向*/
		if (widgeRectLast->x + dXY + widgeRectLast->w < ((HXRECT)(hBaseWidge))->x + ((HXRECT)(hBaseWidge))->w) {
			//return FALSE;
			dXY = ((HXRECT)(hBaseWidge))->x + ((HXRECT)(hBaseWidge))->w - (widgeRectLast->x + widgeRectLast->w);
		}
		if (widgeRectFirst->x + dXY > ((HXRECT)(hBaseWidge))->x) {
			dXY = ((HXRECT)(hBaseWidge))->x - widgeRectFirst->x;
		}
	}
	else {/*纵向*/
		if (widgeRectLast->y + dXY + widgeRectLast->h < ((HXRECT)(hBaseWidge))->y + ((HXRECT)(hBaseWidge))->h) {
			dXY = (((HXRECT)(hBaseWidge))->y + ((HXRECT)(hBaseWidge))->h) - (widgeRectLast->y + widgeRectLast->h);
		}
		if (widgeRectFirst->y + dXY > ((HXRECT)(hBaseWidge))->y) {
			dXY = ((HXRECT)(hBaseWidge))->y - widgeRectFirst->y;
		}
	}
	/*每个控件进行偏移*/
	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList) {/*开始扫描*/
		HXRECT hWidge = (HXRECT)_PToHWidgeBaseType(val);
		if (_GetOTN(hBaseWidge)) {/*横向*/
			hWidge->x += dXY;
		}
		else {/*纵向*/
			hWidge->y += dXY;
		}
	}
	_EndScanU();	/*结束扫描*/
	WindowsInvaildRect(_PToHWidgeBaseType(hBaseWidge), (HXRECT)hBaseWidge);
	return TRUE;
}
static void GUITimeoutCb(void* arg) {
	HLIST_WIDGE hBaseWidge = arg;
	if (arg == NULL) { return; }
	if (_GetListAnimation(hBaseWidge)) {/*是否开启的了动画*/
		if (!(hBaseWidge->dSP >= -1 && hBaseWidge->dSP <= 1)) {
			if (!LIST_WIDGE_MARK_HEAD(ListSlide)(hBaseWidge, hBaseWidge->dSP)) {
				hBaseWidge->dSP = 0;
			}
			else {
				if (hBaseWidge->dSP < 0) {
					hBaseWidge->dSP++;
					if (hBaseWidge->dSP > 0) {
						hBaseWidge->dSP = 0;
					}
				}
				else if (hBaseWidge->dSP > 0) {
					hBaseWidge->dSP--;
					if (hBaseWidge->dSP < 0) {
						hBaseWidge->dSP = 0;
					}
				}
				GUITimeoutOpen(hBaseWidge->hGUITimeout);
			}
		}
	}
}
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void* hObject) {
	HLIST_WIDGE hBaseWidge;
	HLIST cutPostionList;
	hBaseWidge = hObject;
	if (!hBaseWidge) { return; }
	if (!_GetVisable(hBaseWidge)) { return; }
	if (!IsGUINeedCut((HWIDGET_BASE)hBaseWidge)) { return; }
	if (!DrawSetArea((HWIDGET_BASE)hBaseWidge)) { return; }//计算得到当前绘图区域

	//计算得到剪裁区域
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hBaseWidge)->widgetList->next);
	DrawCutRect(hBaseWidge,
		&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList) {/*开始扫描*/
		cutPostionList = RectCutAddRectList(tempItem->next);/*加入遮盖的剪切矩形*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));/*绘制并计算绘图区域*/
		RectCutSplitRectList(cutPostionList);/*去掉遮盖的剪切矩形*/
		//DrawResetArea(hBaseWidge);/*恢复绘图区域*/
	}
	_EndScanU();	/*结束扫描*/

	/*恢复绘图区域*/
	DrawResetArea((HWIDGET_BASE)hBaseWidge);
}
/*事件回调函数*/
PUBLIC int8 LIST_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HLIST_WIDGE hBaseWidge = hObject;
	if (!hBaseWidge || !hMsg) { return -1; }
	if (!_GetVisable(hBaseWidge)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect))) {
			int16 dXY = 0;
			/*触摸事件*/
			if (hMsg->msgType == MSG_TOUCH) {
				switch (hMsg->msgID) {
				case MSG_TOUCH_PRESS:
					if (_GetOTN(hBaseWidge)) {/*横向*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.x;

						/*开启了动画才会用到这个变量*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.x;
					}
					else {/*竖向*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.y;

						/*开启了动画才会用到这个变量*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.y;
					}
					if (_GetListAnimation(hBaseWidge)) {
						/*如果开启了动画，记录当时按下的时间*/
						hBaseWidge->firstTime = (uint16)GUIGetTick();
					}
					break;
				case MSG_TOUCH_MOVE:
					if (_GetOTN(hBaseWidge)) {/*横向*/
						/*计算需要偏移的*/
						dXY = hMsg->msgVal.rect.x - hBaseWidge->lastXY;
					}
					else {/*竖向*/
						dXY = hMsg->msgVal.rect.y - hBaseWidge->lastXY;
					}
					if (dXY != 0) {
						_SetSliding(hBaseWidge);
					}
					if (_GetSlideState(hBaseWidge)) {
						LIST_WIDGE_MARK_HEAD(ListSlide)(hBaseWidge, dXY);
						if (_GetOTN(hBaseWidge)) {/*横向*/
							hBaseWidge->lastXY = hMsg->msgVal.rect.x;
						}
						else {
							hBaseWidge->lastXY = hMsg->msgVal.rect.y;
						}
						return RES_OK;
					}
					break;
				case MSG_TOUCH_RELEASE:
					if (_GetSlideState(hBaseWidge)) {
						if (_GetListAnimation(hBaseWidge)) {/*如果开启了动画*/
							int16 dis = 0;
							if (_GetOTN(hBaseWidge)) {/*横向*/
								dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
							}
							else {/*竖向*/
								dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
							}

							/*吧与上次差值当做速度*/
							hBaseWidge->dSP = 25 * (dis) / ((uint16)((uint16)GUIGetTick() - hBaseWidge->firstTime));
							if (hBaseWidge->dSP != 0) {
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
			HLIST hWidgeList = _PToHGroupWidgeType(hBaseWidge)->widgetList;
			if (hWidgeList) {
				int8 ret;
				HLIST lastWidge = ListGetLast(hWidgeList);
				if (lastWidge != NULL) {
					while (lastWidge != hWidgeList) {
						HWIDGET_BASE hWidge = (HWIDGET_BASE)(lastWidge->val);
						if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == 0) {
							return 0;
						}
						lastWidge = lastWidge->lnext;
					}
				}
			}
			return 0;
		}
		else {/*出界了*/
			if (_GetSlideState(hBaseWidge)) {
				if (_GetListAnimation(hBaseWidge)) {/*如果开启了动画*/
					int16 dis = 0;
					if (_GetOTN(hBaseWidge)) {/*横向*/
						dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
					}
					else {/*竖向*/
						dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
					}

					/*吧与上次差值当做速度*/
					hBaseWidge->dSP = (dis) / ((uint16)((uint16)GUIGetTick() - hBaseWidge->firstTime));
					if (hBaseWidge->dSP != 0) {
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
