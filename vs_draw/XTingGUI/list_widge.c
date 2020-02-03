#include "list_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"

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

	hObject->itemsSize.itemSize = LIST_WIDGE_ITEM_SIZE;
	hObject->lastY = 0;
	hObject->flag = 0;

	_SetListWidgeMode(hObject);
	hObject->itemsSize.itemsH = 0;

	return hObject;
}
/*添加一个控件*/
PUBLIC uint8 LIST_WIDGE_MARK_HEAD(Add)(HLIST_WIDGE hBaseWidge, HWIDGE_BASE widge) {
	uint16 widgeLength;
	if (GROUP_MARK_HEAD(Add)((HGROUP_WIDGE)hBaseWidge, widge) == FALSE) {
		return FALSE;
	}
	widgeLength = ((HGROUP_WIDGE)hBaseWidge)->widgeLength - 1;
	if (!_GetListWidgeMode(hBaseWidge)) {/*定高模式*/
		/*重新设置控件的信息*/
		((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
		((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y + hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
		((HXRECT)widge)->h = hBaseWidge->itemsSize.itemSize;
		((HXRECT)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
	}
	else {/*高度自适应*/
		/*重新设置控件的信息*/
		((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
		((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y + hBaseWidge->itemsSize.itemsH;
		((HXRECT)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;

		hBaseWidge->itemsSize.itemsH += ((HXRECT)widge)->h;
	}
	
	/*刷新*/
	WindowsInvaildRect((HWIDGE_BASE)hBaseWidge, NULL);
	return TRUE;
}
/*ListWidge内部控件的位置滑动*/
PRIVATE void LIST_WIDGE_MARK_HEAD(ListSlide)(HLIST_WIDGE hBaseWidge,int16 dy) {
	HXRECT widgeRectLast;
	HXRECT widgeRectFirst;

	if (!hBaseWidge) { return; }
	widgeRectFirst = (HXRECT)GROUP_MARK_HEAD(GetWidge)((HGROUP_WIDGE)hBaseWidge, 0);
	widgeRectLast = (HXRECT)GROUP_MARK_HEAD(GetWidge)((HGROUP_WIDGE)hBaseWidge, hBaseWidge->groupWidge.widgeLength - 1);
	if (widgeRectLast == NULL || widgeRectFirst == NULL) { return; }
	
	if (widgeRectLast->y + dy + widgeRectLast->h < ((HXRECT)(hBaseWidge))->y+ ((HXRECT)(hBaseWidge))->h) {
		return;
	}
	if (widgeRectFirst->y + dy> ((HXRECT)(hBaseWidge))->y) {
		return;
	}
	/*每个控件进行偏移*/
	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList) {/*开始扫描*/
		HXRECT hWidge = (HXRECT)_PToHWidgeBaseType(val);
		hWidge->y += dy;
	}
	_EndScanU();	/*结束扫描*/
	WindowsInvaildRect((HWIDGE_BASE)hBaseWidge, NULL);
}
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void* hObject) {
	HLIST_WIDGE hBaseWidge;
	HLIST cutPostionList;
	hBaseWidge = hObject;
	if (!hBaseWidge) { return; }
	if (!_GetVisable(hBaseWidge)) { return; }
	if (!IsGUINeedCut((HXRECT)hBaseWidge)) { return; }
	DrawSetArea(hBaseWidge);//计算得到当前绘图区域
	//计算得到剪裁区域
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hBaseWidge)->widgetList->next);
	DrawCutRect(hBaseWidge,
		&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList) {/*开始扫描*/
		cutPostionList = RectCutAddRectList(tempItem->next);/*加入遮盖的剪切矩形*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));/*绘制并计算绘图区域*/
		RectCutSplitRectList(cutPostionList);/*去掉遮盖的剪切矩形*/
		DrawResetArea(hBaseWidge);/*恢复绘图区域*/
	}
	_EndScanU();	/*结束扫描*/
}
/*事件回调函数*/
PUBLIC int8 LIST_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HLIST_WIDGE hBaseWidge = hObject;
	if (!hBaseWidge || !hMsg) { return -1; }
	if (!_GetVisable(hBaseWidge)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect))) {
			int16 dy;
			/*触摸事件*/
			if (hMsg->msgType == MSG_TOUCH) {
				switch (hMsg->msgID) {
				case MSG_TOUCH_PRESS:		
					hBaseWidge->lastY = hMsg->msgVal.rect.y;
					//hBaseWidge->dy = hMsg->msgVal.rect.y - _PToHXRECTType(hBaseWidge)->y;
					break;
				case MSG_TOUCH_MOVE:
					dy = hMsg->msgVal.rect.y - hBaseWidge->lastY;
					if (dy != 0) {
						_SetSliding(hBaseWidge);
					}
					if (_GetSlideState(hBaseWidge)) {
						LIST_WIDGE_MARK_HEAD(ListSlide)(hBaseWidge, dy);
						hBaseWidge->lastY = hMsg->msgVal.rect.y;
						return RES_OK;
					}
					break;
				case MSG_TOUCH_RELEASE:
					if (_GetSlideState(hBaseWidge)) {
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
						HWIDGE_BASE hWidge = (HWIDGE_BASE)(lastWidge->val);
						if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == 0) {
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
