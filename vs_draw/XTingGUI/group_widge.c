#include "group_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "radio_button_widge.h"

PUBLIC HGROUP_WIDGE GROUP_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h) {
	HGROUP_WIDGE hObject;

	hObject = (HGROUP_WIDGE)(xMalloc(sizeof(GROUP_WIDGE)));
	if (hObject == NULL) {
		return NULL;
	}
	GROUP_MARK_HEAD(Init)(hObject, x, y, w, h);
	return hObject;
}
PUBLIC uint8 GROUP_MARK_HEAD(Init)(HGROUP_WIDGE hBaseWidge, int16 x, int16 y, uint16 w, uint16 h) {
	_WIDGET(Init)((HWIDGET_BASE)hBaseWidge, x, y, w, h);
	/*控件列表*/
	hBaseWidge->widgetList = ListNew();
	if (hBaseWidge->widgetList == NULL) {
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
PUBLIC void GROUP_MARK_HEAD(Close)(HGROUP_WIDGE hObject) {
	if (!hObject) { return ; }
	/*在这里释放窗口中的内存*/

	/*迭代每一个控件*/
	_StartScanU(_PToHGroupWidgeType(hObject)->widgetList) {//开始扫描
		HWIDGET_BASE hWidge = val;
		/*调用每一个控件的关闭函数*/
		hWidge->widgeCloseFun(hWidge);
	}
	/*结束扫描*/
	_EndScanU();

	/*释放掉List*/
	ListClear(_PToHGroupWidgeType(hObject)->widgetList);
	_PToHGroupWidgeType(hObject)->widgeLength = 0;
	/*删除List头*/
	ListDel(_PToHGroupWidgeType(hObject)->widgetList);

	/*调用父控件释放函数*/
	_WIDGET(Close)((HWIDGET_BASE)hObject);
}
/*获取某个位置的控件*/
PUBLIC HWIDGET_BASE GROUP_MARK_HEAD(GetWidge)(HGROUP_WIDGE hObject, uint16 index) {
	if (!hObject) { return NULL; }
	HLIST hlist = ListGet(hObject->widgetList, index);
	if (hlist != NULL) {
		return (HWIDGET_BASE)(hlist->val);
	}
	else {
		return NULL;
	}
}
/*重新设置窗口的大小,还需要更改内部控件的大小与位置*/
PUBLIC void GROUP_MARK_HEAD(Resize)(HGROUP_WIDGE hObject, int16 x, int16 y, uint16 w, uint16 h) {
	if (!hObject) { return; }

	int16 dx, dy;
	/*窗口位置改变的偏移*/
	dx = x - ((HXRECT)hObject)->x;
	dy = y - ((HXRECT)hObject)->y;

	_WIDGET(Resize)((HWIDGET_BASE)hObject, x, y, w, h);

	/*迭代每一个控件*/
	_StartScanU(hObject->widgetList) {//开始扫描
		_PToHWidgeBaseType(val)->widgeResizeFun(
			_PToHWidgeBaseType(val),
			_PToHWidgeBaseType(val)->rect.x + dx,
			_PToHWidgeBaseType(val)->rect.y + dy,
			_PToHWidgeBaseType(val)->rect.w,
			_PToHWidgeBaseType(val)->rect.h);
	}
	/*结束扫描*/
	_EndScanU();
	/*改变窗口大小必须刷新父控件*/
	//WindowsInvaildRect(((HWIDGET_BASE)hObject)->parentHWIN, NULL);
}
/*删除一个控件*/
PUBLIC uint8 GROUP_MARK_HEAD(Del)(HGROUP_WIDGE hBaseWidge, HWIDGET_BASE widge) {
	if (hBaseWidge == NULL || widge == NULL) { return FALSE; }
	/*在list中删除*/
	if (ListDelByVal(hBaseWidge->widgetList, widge) == FALSE) { return FALSE; }
	hBaseWidge->widgeLength--;
	/*调用控件的关闭函数*/
	widge->widgeCloseFun(widge);
	WindowsInvaildRect(((HWIDGET_BASE)hBaseWidge), NULL);
	return TRUE;
}
/*添加一个控件*/
PUBLIC uint8 GROUP_MARK_HEAD(Add)(HGROUP_WIDGE hBaseWidge, HWIDGET_BASE widge) {
	if (!hBaseWidge || !widge) { return FALSE; }
	HLIST addItem = ListNew();
	if (!addItem) { return FALSE; }
	addItem->val = widge;
	/*添加到List的结尾*/
	if (ListAddLast(hBaseWidge->widgetList, addItem) == -1) {
		/*失败则释放之前申请的内存*/
		ListDel(addItem);
		return FALSE;
	}
	/*设置父控件*/
	widge->parentHWIN = hBaseWidge;

	/*移动控件到指定位置*/
	widge->moveToFun(widge,
		widge->rect.x + hBaseWidge->widgeBase.rect.x,
		widge->rect.y + hBaseWidge->widgeBase.rect.y);
	/*增加长度*/
	hBaseWidge->widgeLength++;
	return TRUE;
}
PUBLIC void GROUP_MARK_HEAD(GroupProcess)(HGROUP_WIDGE hBaseWidge,HWIDGET_BASE currentWidge) {
	if (hBaseWidge == NULL) { return; }
	_StartScanU(hBaseWidge->widgetList) {/*开始扫描*/
		HWIDGET_BASE hWidgeBase = (HWIDGET_BASE)(val);

		if (_GET_GROUP_CTRL(hWidgeBase)) {
			/*需要联动控制*/
			if (currentWidge != hWidgeBase) {
				/*调用RADIO_BUTTON的函数*/
				RADIO_BUTTON_MARK_HEAD(SetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase,FALSE);
			}
		}

	}
	_EndScanU();	/*结束扫描*/
}
PUBLIC void GROUP_MARK_HEAD(Paint)(void* hObject) {
	HGROUP_WIDGE hBaseWidge;
	HLIST cutPostionList;
	hBaseWidge = hObject;
	if (!hBaseWidge) { return; }
	if (!_GetVisable(hBaseWidge)) { return; }
	//if (!IsGUINeedCut(hWinHead)) { return; }
	if (!DrawSetArea((HWIDGET_BASE)hBaseWidge)) { return; }//计算得到当前绘图区域
	/*计算得到剪裁区域*/
	cutPostionList = RectCutAddRectList(hBaseWidge->widgetList->next);
	DrawCutRect(hBaseWidge,
		&(hBaseWidge->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(hBaseWidge->widgetList) {					/*开始扫描*/
		cutPostionList = RectCutAddRectList(tempItem->next);/*加入遮盖的剪切矩形*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));/*绘制并计算绘图区域*/
		RectCutSplitRectList(cutPostionList);				/*去掉遮盖的剪切矩形*/
	}
	_EndScanU();	/*结束扫描*/

	/*恢复绘图区域*/
	DrawResetArea((HWIDGET_BASE)hBaseWidge);
}
PUBLIC void GROUP_MARK_HEAD(MoveTo)(HGROUP_WIDGE hObject, int16 x, int16 y) {
	int16 dx = 0;
	int16 dy = 0;
	if (!hObject) { return; }
	/*计算偏移位置*/
	dx = x - (hObject->widgeBase.rect.x);
	dy = y - (hObject->widgeBase.rect.y);
	/*重新设置位置*/
	hObject->widgeBase.rect.x = x;
	hObject->widgeBase.rect.y = y;
	/*重新设置每一个控件位置*/
	HLIST hWidgeList = hObject->widgetList;
	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGET_BASE hWidge = (HWIDGET_BASE)(hWidgeList->val);
			/*重新设置每一个控件的位置*/
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);

			hWidgeList = hWidgeList->next;
		}
	}
}
/*事件回调函数*/
PUBLIC int8 GROUP_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HGROUP_WIDGE hBaseWidge = hObject;
	if (!hBaseWidge || !hMsg) { return -1; }
	if (!_GetVisable(hBaseWidge)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(hBaseWidge->widgeBase.rect))) {
			HLIST hWidgeList = hBaseWidge->widgetList;
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
	}
	return -1;
}