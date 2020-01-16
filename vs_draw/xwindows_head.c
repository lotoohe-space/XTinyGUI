
#include "xwindows_head.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "text_widge.h"
#include "button_widge.h"

HWIN_HEAD WindowsHeadCreate(char *title ,int16 x,int16 y,int16 w,int16 h) {
	HWIN_HEAD hWinHead;
	HTEXT_WIDGE hTextWidge;
	HXBUTTON hXButtonMin;
	
	hWinHead= (HWIN_HEAD)(xMalloc(sizeof(WIN_HEAD)));
	if (hWinHead == NULL) {
		return NULL;
	}
	hTextWidge = TextWidegeCreate(title ? title : _DefaultWinHeadName, 0, 0);
	if (hTextWidge == NULL) {
		xFree(hWinHead);
		return NULL;
	}
	hXButtonMin = BUTTON_MARK_HEAD(Create)("-", 
		w- hTextWidge->hFont->fontInfo.w-1, h+1,
		hTextWidge->hFont->fontInfo.w, hTextWidge->hFont->fontInfo.h-2);
	if (hXButtonMin == NULL) {
		xFree(hWinHead);
		xFree(hTextWidge);
		return NULL;
	}
	//控件列表
	hWinHead->widgetList = ListNew();
	if (hWinHead->widgetList == NULL) {
		xFree( hWinHead);
	}
	WidgeInit((HWIDGE_BASE)hWinHead, x, y, w, h);

	hWinHead->headWidge.rect.x = x;
	hWinHead->headWidge.rect.y = y;
	hWinHead->headWidge.rect.w = w;
	hWinHead->headWidge.rect.h = hTextWidge->hFont->fontInfo.h;
	hWinHead->headWidge.paintFun = WindowsHeadPaint;
	hWinHead->headWidge.moveToFun = WindowsHeadMoveTo;
	hWinHead->headWidge.widgeCallBackFun = WindowsHeadsCallBack;
	hWinHead->headWidge.widgeCloseFun = WindowsHeadClose;
	
	hWinHead->headWidge.pencil.DrawColor = _DefaultHeadColor;
	hWinHead->headWidge.pencil.DrawBkColor = _DefaultFontColor;

	hWinHead->headWidge.pencil.x = x;
	hWinHead->headWidge.pencil.y = y;
	hWinHead->headWidge.pencil.w = w;
	hWinHead->headWidge.pencil.h = hWinHead->headWidge.rect.h;

	hTextWidge->textWidge.parentHWIN = hWinHead;
	hXButtonMin->buttonWidge.parentHWIN = hWinHead;

	WindowsHeadAdd(hWinHead, hTextWidge);
	WindowsHeadAdd(hWinHead, hXButtonMin);

	return hWinHead;
}

//添加一个控件
void WindowsHeadClose(HWIN_HEAD hWinHead) {
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
	WIDGE_MARK_HEAD(Close)(hWinHead);
}
//添加一个控件
int8 WindowsHeadAdd(HWIN_HEAD hWinHead, void *widge) {
	if (!hWinHead || !widge) { return -1; }
	HLIST addItem = ListNew();
	if (!addItem) { return -1; }
	addItem->val = widge;
	if (ListAddLast(hWinHead->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGE_BASE hWidge = widge;

	hWidge->moveToFun(hWidge, 
		hWidge->rect.x + hWinHead->headWidge.rect.x, 
		hWidge->rect.y + hWinHead->headWidge.rect.y );

	return 0;
}
void WindowsHeadMoveTo(HWIN_HEAD hObject, int16 x, int16 y) {
	int16 dx=0;
	int16 dy=0;
	if (!hObject) { return; }
	dx = x - (hObject->headWidge.rect.x);
	dy = y - (hObject->headWidge.rect.y);
	hObject->headWidge.rect.x = x;
	hObject->headWidge.rect.y = y;
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
void WindowsHeadSetArea(HWIN_HEAD hObject,int16 x, int16 y, int16 w, int16 h) {
	if (!hObject) { return; }
	hObject->headWidge.pencil.x = x;
	hObject->headWidge.pencil.y = y;
	hObject->headWidge.pencil.w = w;
	hObject->headWidge.pencil.h = h;
}
void WindowsHeadSetVisable(void* hObject,int8 isVisable) {
	HWIN_HEAD hWinHead;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	hWinHead->headWidge.isVisable = isVisable;
}
void WindowsHeadPaint(void * hObject){
	HWIN_HEAD hWinHead;
	HLIST cutPostionList;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	if (!hWinHead->headWidge.isVisable) { return; }
	//if (!isGUINeedCut(hWinHead)) { return; }
	DrawSetArea(hWinHead);//计算得到当前绘图区域
	//计算得到剪裁区域
	cutPostionList=RectCutAddRectList(hWinHead->widgetList->next);
	DrawCutRect(hWinHead, 
		&(hWinHead->headWidge.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(hWinHead->widgetList) {//开始扫描
		cutPostionList = RectCutAddRectList(tempItem->next);//加入遮盖的剪切矩形
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));//绘制并计算绘图区域
		RectCutSplitRectList(cutPostionList);//去掉遮盖的剪切矩形
		DrawResetArea(hWinHead);//恢复绘图区域
	}
	_EndScanU()	//结束扫描
	

}
int8 WindowsHeadsCallBack(void *hObject, HMSGE hMsg) {
	HWIN_HEAD hWinHead = hObject;
	if (!hWinHead || !hMsg) { return -1; }
	if (!(hWinHead->headWidge.isVisable)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(hWinHead->headWidge.rect))) {
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
