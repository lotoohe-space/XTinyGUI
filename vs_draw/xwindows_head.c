
#include "xwindows_head.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "text_widge.h"
#include "button_widge.h"

PUBLIC HWIN_HEAD WINDOWS_HEAD_MARK_HEAD(Create)(char *title ,int16 x,int16 y,int16 w,int16 h) {
	HWIN_HEAD hWinHead;
	HTEXT_WIDGE hTextWidge;
	
	
	hWinHead= (HWIN_HEAD)(xMalloc(sizeof(WIN_HEAD)));
	if (hWinHead == NULL) {
		return NULL;
	}
	hTextWidge = TextWidegeCreate(title ? title : _DefaultWinHeadName, 0, 0,w,h);
	if (hTextWidge == NULL) {
		xFree(hWinHead);
		return NULL;
	}
	hWinHead->hXButtonMin = BUTTON_MARK_HEAD(Create)("*",
		w- h, 0,h, h);
	if (hWinHead->hXButtonMin == NULL) {
		xFree(hWinHead);
		xFree(hTextWidge);
		return NULL;
	}

	//控件列表
	hWinHead->widgetList = ListNew();
	if (hWinHead->widgetList == NULL) {
		xFree( hWinHead);
	}
	WIDGE_MARK_HEAD(Init)((HWIDGE_BASE)hWinHead, x, y, w, h);

	//hWinHead->widgeBase.rect.x = x;
	//hWinHead->widgeBase.rect.y = y;
	//hWinHead->widgeBase.rect.w = w;
	//hWinHead->widgeBase.rect.h = h;
	hWinHead->widgeBase.paintFun = WINDOWS_HEAD_MARK_HEAD(Paint);
	hWinHead->widgeBase.moveToFun = WINDOWS_HEAD_MARK_HEAD(MoveTo);
	hWinHead->widgeBase.widgeCallBackFun = WINDOWS_HEAD_MARK_HEAD(CallBack);
	hWinHead->widgeBase.widgeCloseFun = WINDOWS_HEAD_MARK_HEAD(Close);
	
	//hWinHead->widgeBase.pencil.DrawColor = _DefaultHeadColor;
	//hWinHead->widgeBase.pencil.DrawBkColor = _DefaultFontColor;

	//hWinHead->widgeBase.pencil.x = x;
	//hWinHead->widgeBase.pencil.y = y;
	//hWinHead->widgeBase.pencil.w = w;
	//hWinHead->widgeBase.pencil.h = h;

	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hTextWidge);
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonMin);

	return hWinHead;
}

//添加一个控件
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Close)(HWIN_HEAD hWinHead) {
	if (!hWinHead) { return; }
	/*在这里释放窗口中的内存*/
	/*获取每一个控件*/
	HLIST hWidgeList = hWinHead->widgetList;
	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);
			/*调用每一个控件的关闭函数*/
			hWidge->widgeCloseFun(hWidge);

			hWidgeList = hWidgeList->next;
		}
	}
	WIDGE_MARK_HEAD(Close)((HWIDGE_BASE)hWinHead);
}
//添加一个控件
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(Add)(HWIN_HEAD hWinHead, void *widge) {
	if (!hWinHead || !widge) { return -1; }
	HLIST addItem = ListNew();
	if (!addItem) { return -1; }
	addItem->val = widge;
	if (ListAddLast(hWinHead->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGE_BASE hWidge = widge;
	hWidge->parentHWIN = hWinHead;
	hWidge->moveToFun(hWidge, 
		hWidge->rect.x + hWinHead->widgeBase.rect.x,
		hWidge->rect.y + hWinHead->widgeBase.rect.y );

	return 0;
}
/*设置最小化按钮的回调接口*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(HWIN_HEAD hObject,void *arg, ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)((HWIDGE_BASE)(hObject->hXButtonMin), arg,viewClickCallBack);
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(MoveTo)(HWIN_HEAD hObject, int16 x, int16 y) {
	int16 dx=0;
	int16 dy=0;
	if (!hObject) { return; }
	dx = x - (hObject->widgeBase.rect.x);
	dy = y - (hObject->widgeBase.rect.y);
	hObject->widgeBase.rect.x = x;
	hObject->widgeBase.rect.y = y;
	//重新设置每一个控件位置
	HLIST hWidgeList = hObject->widgetList;

	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);
			hWidgeList = hWidgeList->next;
		}
	}
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetArea)(HWIN_HEAD hObject,int16 x, int16 y, int16 w, int16 h) {
	if (!hObject) { return; }
	hObject->widgeBase.pencil.x = x;
	hObject->widgeBase.pencil.y = y;
	hObject->widgeBase.pencil.w = w;
	hObject->widgeBase.pencil.h = h;
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetVisable)(void* hObject,int8 isVisable) {
	WIDGE_MARK_HEAD(SetVisable)(hObject, isVisable);
	//HWIN_HEAD hWinHead;
	//hWinHead = hObject;
	//if (!hWinHead) { return; }
	//hWinHead->widgeBase.isVisable = isVisable;
}
PRIVATE void WINDOWS_HEAD_MARK_HEAD(Paint)(void * hObject){
	HWIN_HEAD hWinHead;
	HLIST cutPostionList;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	if (!_GetVisable(hWinHead)) { return; }
	//if (!isGUINeedCut(hWinHead)) { return; }
	DrawSetArea(hWinHead);//计算得到当前绘图区域
	//计算得到剪裁区域
	cutPostionList=RectCutAddRectList(hWinHead->widgetList->next);
	DrawCutRect(hWinHead, 
		&(hWinHead->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(hWinHead->widgetList) {//开始扫描
		cutPostionList = RectCutAddRectList(tempItem->next);//加入遮盖的剪切矩形
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));//绘制并计算绘图区域
		RectCutSplitRectList(cutPostionList);//去掉遮盖的剪切矩形
		DrawResetArea(hWinHead);//恢复绘图区域
	}
	_EndScanU();	//结束扫描
	

}
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	HWIN_HEAD hWinHead = hObject;
	if (!hWinHead || !hMsg) { return -1; }
	if (!_GetVisable(hWinHead)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(hWinHead->widgeBase.rect))) {
			HLIST hWidgeList = hWinHead->widgetList;
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
