#include "paint.h"
#include "widge.h"
#include "x_malloc.h"
#include "gui.h"
#include "paint_cut.h"

PUBLIC HWIDGE_BASE WidgeCreate(int16 x, int16 y, int16 w, int16 h) {
	HWIDGE_BASE hWidgeBase = (HWIDGE_BASE)(xMalloc( sizeof(WIDGE_BASE)));
	if (hWidgeBase == NULL) {
		return NULL;
	}
	hWidgeBase->rect.x = x;
	hWidgeBase->rect.y = y;
	hWidgeBase->rect.w = w;
	hWidgeBase->rect.h = h;

	/*设置三个回调函数*/
	hWidgeBase->paintFun = WidgePaint;
	hWidgeBase->moveToFun = WidgeMoveTo;
	hWidgeBase->widgeCallBackFun = WidgeCallBack;

	/*设置颜色*/
	hWidgeBase->pencil.DrawColor = _DefaultFrColor;

	hWidgeBase->pencil.x = x;
	hWidgeBase->pencil.y = y;
	hWidgeBase->pencil.w = w;
	hWidgeBase->pencil.h = h;

	hWidgeBase->isVisable = TRUE;

	return hWidgeBase;
}

PUBLIC void WidgeMoveTo(HWIDGE_BASE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->rect.x = x;
	hObject->rect.y = y;
} 

PUBLIC void WidgeSetParentWin(HWIDGE_BASE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->parentHWIN = hWIN;
}
PUBLIC void WidgeSetColor(HWIDGE_BASE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->pencil.DrawColor = color;
}
PUBLIC void WidgePaint(void *hObject) {
	HWIDGE_BASE hWidgeBase;
	hWidgeBase = hObject;
	if (!hObject) { return; }
	if (hWidgeBase->isVisable == 0) { return; }
	if (!isGUINeedCut(hWidgeBase)) { return; }

	DrawSetArea(hWidgeBase);

	//绘制底色
	//hWidgeBase->pencil.DrawColor = _DefaultFrColor;
	//绘制底色
	RECT_CUT_INIT(&(hWidgeBase->rect)) {
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}RECT_CUT_END()
}

int8 WidgeCallBack(void *hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPoint(hMsg->msgVal.xy.x, hMsg->msgVal.xy.y, hWidgeBase->pencil.x, hWidgeBase->pencil.y, hWidgeBase->pencil.w, hWidgeBase->pencil.h)) {
			return 0;
		}
	}
	return -1;
}
