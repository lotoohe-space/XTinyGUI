
#include "xwindows_head.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "text_widge.h"
#include "button_widge.h"

/*创建WINDOWS Head控件*/
PUBLIC HWIN_HEAD WINDOWS_HEAD_MARK_HEAD(Create)(char* title, int16 x, int16 y, int16 w, int16 h) {
	HWIN_HEAD hWinHead;
	HTEXT_WIDGE hTextWidge;


	hWinHead = (HWIN_HEAD)(xMalloc(sizeof(WIN_HEAD)));
	if (hWinHead == NULL) {
		return NULL;
	}
	hTextWidge = TextWidgeCreate(title ? title : _DefaultWinHeadName, 0, 0, w, h);
	if (hTextWidge == NULL) {
		xFree(hWinHead);
		return NULL;
	}
	hTextWidge->baseWidge.pencil.DrawBkColor = RGB565(255, 255, 255);

	hWinHead->hXButtonMin = BUTTON_MARK_HEAD(Create)("-",
		w - h, 0, h, h);

	if (hWinHead->hXButtonMin == NULL) {
		xFree(hWinHead);
		_PToHWidgeBaseType(hTextWidge)->widgeCloseFun(hTextWidge);
		return NULL;
	}
	//_PToHTextWidgeType(hWinHead->hXButtonMin)->hFont = (HFONTF)& fontUNICODE16_16;

	hWinHead->hXButtonMax = BUTTON_MARK_HEAD(Create)("*",
		w - 2 * h, 0, h, h);
	if (hWinHead->hXButtonMax == NULL) {
		xFree(hWinHead);
		_PToHWidgeBaseType(hTextWidge)->widgeCloseFun(hTextWidge);
		_PToHWidgeBaseType(hWinHead->hXButtonMin)->widgeCloseFun(hTextWidge);
		return NULL;
	}
	//_PToHTextWidgeType(hWinHead->hXButtonMax)->hFont = (HFONTF)& fontUNICODE16_16;

	hWinHead->hXButtonClose = BUTTON_MARK_HEAD(Create)("+",
		w - 3 * h, 0, h, h);
	if (hWinHead->hXButtonClose == NULL) {
		xFree(hWinHead);
		_PToHWidgeBaseType(hTextWidge)->widgeCloseFun(hTextWidge);
		_PToHWidgeBaseType(hWinHead->hXButtonMin)->widgeCloseFun(hTextWidge);
		_PToHWidgeBaseType(hWinHead->hXButtonMax)->widgeCloseFun(hTextWidge);
		return NULL;
	}
	//_PToHTextWidgeType(hWinHead->hXButtonClose)->hFont = (HFONTF)& fontUNICODE16_16;

	//控件列表
	_PToHGroupWidgeType(hWinHead)->widgetList = ListNew();
	if (_PToHGroupWidgeType(hWinHead)->widgetList == NULL) {
		xFree(hWinHead);
	}
	_WIDGET(Init)((HWIDGET_BASE)hWinHead, x, y, w, h);

	_PToHGroupWidgeType(hWinHead)->widgeBase.paintFun = WINDOWS_HEAD_MARK_HEAD(Paint);
	_PToHGroupWidgeType(hWinHead)->widgeBase.moveToFun = WINDOWS_HEAD_MARK_HEAD(MoveTo);
	_PToHGroupWidgeType(hWinHead)->widgeBase.widgeCallBackFun = WINDOWS_HEAD_MARK_HEAD(CallBack);
	_PToHGroupWidgeType(hWinHead)->widgeBase.widgeCloseFun = WINDOWS_HEAD_MARK_HEAD(Close);

	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawColor = RGB565_BLACK;
	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawFrColor = _DefaultFrColor;
	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawBkColor = RGB565(255, 255, 255);

	/*添加头标题*/
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hTextWidge);
	/*添加最小化BUTTON*/
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonMin);
	/*添加最大化BUTTON*/
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonMax);
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonClose);

	return hWinHead;
}
/*获取窗口中的某个位置的控件，得到的不是窗口而是窗口的ListItem*/
PUBLIC HWIDGET_BASE WINDOWS_HEAD_MARK_HEAD(GetWidge)(HWIN_HEAD hObject, uint16 index) {
	if (!hObject) { return NULL; }
	HLIST hlist = ListGet(_PToHGroupWidgeType(hObject)->widgetList, index);
	if (hlist != NULL) {
		return (HWIDGET_BASE)(hlist->val);
	}
	else {
		return NULL;
	}
}
/*重新设置窗口的大小,还需要更改内部控件的大小与位置,位置是相对于在控件中的位置*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Resize)(HWIN_HEAD hObject, int16 x, int16 y, uint16 w, uint16 h) {
	HWIDGET_BASE hWidgeBase;
	if (!hObject) { return; }
	_WIDGET(Resize)((HWIDGET_BASE)hObject, x, y, w, h);

	/*标题*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 0);
	if (hWidgeBase == NULL) { return; }
	_WIDGET(Resize)(hWidgeBase, x, y, w, h);

	/*最小化按钮*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 1);
	if (hWidgeBase == NULL) { return; }
	_WIDGET(Resize)(hWidgeBase, x + w - h, y, h, h);

	/*最大化按钮*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 2);
	if (hWidgeBase == NULL) { return; }
	_WIDGET(Resize)(hWidgeBase, x + (w - 2 * h), y, h, h);

	/*最大化按钮*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 3);
	if (hWidgeBase == NULL) { return; }
	_WIDGET(Resize)(hWidgeBase, x + (w - 3 * h), y, h, h);

	/*刷新父窗口*/
	WindowsInvaildRect(((HWIDGET_BASE)hObject)->parentHWIN, NULL);
}
//关闭
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Close)(HWIN_HEAD hWinHead) {
	if (!hWinHead) { return; }
	/*在这里释放窗口中的内存*/

	/*释放本类中的占用的内存*/

	/*调用父类的关闭函数*/
	GROUP_MARK_HEAD(Close)((HGROUP_WIDGE)hWinHead);
}
//添加一个控件
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(Add)(HWIN_HEAD hWinHead, void* widge) {
	if (!hWinHead || !widge) { return -1; }
	HLIST addItem = ListNew();
	if (!addItem) { return -1; }
	addItem->val = widge;
	if (ListAddLast(_PToHGroupWidgeType(hWinHead)->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGET_BASE hWidge = widge;
	hWidge->parentHWIN = hWinHead;
	hWidge->moveToFun(hWidge,
		hWidge->rect.x + _PToHGroupWidgeType(hWinHead)->widgeBase.rect.x,
		hWidge->rect.y + _PToHGroupWidgeType(hWinHead)->widgeBase.rect.y);

	return 0;
}
/*设置最小化按钮的回调接口*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack) {
	_WIDGET(SetClickBack)((HWIDGET_BASE)(hObject->hXButtonMin), arg, viewClickCallBack);
}
/*设置最大化按钮的回调接口*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMaximumBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack) {
	_WIDGET(SetClickBack)((HWIDGET_BASE)(hObject->hXButtonMax), arg, viewClickCallBack);
}
/*设置关闭按钮的回调接口*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetCloseBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack) {
	_WIDGET(SetClickBack)((HWIDGET_BASE)(hObject->hXButtonClose), arg, viewClickCallBack);
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(MoveTo)(HWIN_HEAD hObject, int16 x, int16 y) {
	int16 dx = 0;
	int16 dy = 0;
	if (!hObject) { return; }
	dx = x - (_PToHGroupWidgeType(hObject)->widgeBase.rect.x);
	dy = y - (_PToHGroupWidgeType(hObject)->widgeBase.rect.y);
	_PToHGroupWidgeType(hObject)->widgeBase.rect.x = x;
	_PToHGroupWidgeType(hObject)->widgeBase.rect.y = y;
	//重新设置每一个控件位置
	HLIST hWidgeList = _PToHGroupWidgeType(hObject)->widgetList;

	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGET_BASE hWidge = (HWIDGET_BASE)(hWidgeList->val);
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);
			hWidgeList = hWidgeList->next;
		}
	}
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetArea)(HWIN_HEAD hObject, int16 x, int16 y, int16 w, int16 h) {
	if (!hObject) { return; }
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.x = x;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.y = y;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.w = w;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.h = h;
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetVisable)(void* hObject, int8 isVisable) {
	_WIDGET(SetVisable)(hObject, isVisable);
}
PRIVATE void WINDOWS_HEAD_MARK_HEAD(Paint)(void* hObject) {
	HWIN_HEAD hWinHead;
	HLIST cutPostionList;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	if (!_GetVisable(hWinHead)) { return; }
	//if (!isGUINeedCut(hWinHead)) { return; }
	if (!DrawSetArea((HWIDGET_BASE)hWinHead)) { return; }//计算得到当前绘图区域
	//计算得到剪裁区域
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hWinHead)->widgetList->next);
	DrawCutRect(hWinHead,
		&(_PToHGroupWidgeType(hWinHead)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hWinHead)->widgetList) {//开始扫描
		cutPostionList = RectCutAddRectList(tempItem->next);//加入遮盖的剪切矩形
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));//绘制并计算绘图区域
		RectCutSplitRectList(cutPostionList);//去掉遮盖的剪切矩形
		//DrawResetArea(hWinHead);//恢复绘图区域
	}
	_EndScanU();	//结束扫描

	/*恢复绘图区域*/
	DrawResetArea((HWIDGET_BASE)hWinHead);
}
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIN_HEAD hWinHead = hObject;
	if (!hWinHead || !hMsg) { return RES_ASSERT_ERR; }
	if (!_GetVisable(hWinHead)) { return RES_ASSERT_ERR; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(_PToHGroupWidgeType(hWinHead)->widgeBase.rect))) {
			HLIST hWidgeList = _PToHGroupWidgeType(hWinHead)->widgetList;
			if (hWidgeList) {
				int8 ret;
				HLIST lastWidge = ListGetLast(hWidgeList);
				if (lastWidge != NULL) {
					while (lastWidge != hWidgeList) {
						HWIDGET_BASE hWidge = (HWIDGET_BASE)(lastWidge->val);
						if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == RES_OK) {
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
