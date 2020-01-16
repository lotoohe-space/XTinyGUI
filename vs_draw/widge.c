#include "paint.h"
#include "widge.h"
#include "x_malloc.h"
#include "gui.h"
#include "paint_cut.h"

PUBLIC HWIDGE_BASE WIDGE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h) {
	HWIDGE_BASE hWidgeBase = (HWIDGE_BASE)(xMalloc( sizeof(WIDGE_BASE)));
	if (hWidgeBase == NULL) {
		return NULL;
	}
	WidgeInit(hWidgeBase, x, y, w, h);
	return hWidgeBase;
}
PUBLIC HWIDGE_BASE WIDGE_MARK_HEAD(CreateEx)(HXRECT hXRect) {
	if (hXRect == NULL) {
		return NULL;
	}
	return WIDGE_MARK_HEAD(Create)(hXRect->x, hXRect->y, hXRect->w, hXRect->h);
}
PUBLIC void WIDGE_MARK_HEAD(Init)(HWIDGE_BASE hWidgeBase,int16 x, int16 y, int16 w, int16 h) {
	if (hWidgeBase == NULL) {
		return ;
	}
	/*控件大小*/
	hWidgeBase->rect.x = x;
	hWidgeBase->rect.y = y;
	hWidgeBase->rect.w = w;
	hWidgeBase->rect.h = h;

	/*设置三个回调函数*/
	hWidgeBase->paintFun = WIDGE_MARK_HEAD(Paint);
	hWidgeBase->moveToFun = WIDGE_MARK_HEAD(MoveTo);
	hWidgeBase->widgeCallBackFun = WIDGE_MARK_HEAD(CallBack);
	hWidgeBase->widgeCloseFun = WIDGE_MARK_HEAD(Close);
	/*设置颜色*/
	hWidgeBase->pencil.DrawColor = _DefaultFrColor;
	hWidgeBase->pencil.DrawBkColor = _DefaultBKColor;

	/*默认绘图区域为全部*/
	hWidgeBase->pencil.x = x;
	hWidgeBase->pencil.y = y;
	hWidgeBase->pencil.w = w;
	hWidgeBase->pencil.h = h;

	/*默认可见*/
	hWidgeBase->isVisable = TRUE;

	hWidgeBase->viewClickCallBack = NULL;

	/*当前控件不是窗口*/
	_SET_IS_WIN(hWidgeBase);
}
PUBLIC void WIDGE_MARK_HEAD(Close)(HWIDGE_BASE hObject){
	if (hObject == NULL) { return; }
	xFree(hObject);
}
PUBLIC void WIDGE_MARK_HEAD(SetClickBack)(HWIDGE_BASE hObject, ViewClickCallBack viewClickCallBack) {
	if (!hObject) { return; }
	hObject->viewClickCallBack = viewClickCallBack;
}
/*设置是否可见*/
PUBLIC void WIDGE_MARK_HEAD(SetVisable)(HWIDGE_BASE hObject, uint8 isVisable) {
	if (!hObject) { return; }
	hObject->isVisable = isVisable;
	WindowsInvaildRect(hObject->parentHWIN, hObject);
}

/*设置父控件*/
PUBLIC void WIDGE_MARK_HEAD(SetParentWin)(HWIDGE_BASE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->parentHWIN = hWIN;
}
/*设置前景色*/
PUBLIC void WIDGE_MARK_HEAD(SetColor)(HWIDGE_BASE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->pencil.DrawColor = color;
	WindowsInvaildRect(hObject->parentHWIN, hObject);
}
/*移动控件*/
PUBLIC void WIDGE_MARK_HEAD(MoveTo)(HWIDGE_BASE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->rect.x = x;
	hObject->rect.y = y;
	WindowsInvaildRect(hObject->parentHWIN, hObject);
}
/*重绘函数*/
PRIVATE void WIDGE_MARK_HEAD(Paint)(void *hObject) {
	HWIDGE_BASE hWidgeBase;
	hWidgeBase = hObject;
	if (!hObject) { return; }
	if (hWidgeBase->isVisable == 0) { return; }
	//if (!isGUINeedCut(hWidgeBase)) { return; }

	DrawSetArea(hWidgeBase);

	//绘制底色
	//hWidgeBase->pencil.DrawColor = _DefaultFrColor;
	//绘制底色
	RECT_CUT_INIT(&(hWidgeBase->rect)) {
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}RECT_CUT_END()
}
/*事件回调函数*/
PUBLIC int8 WIDGE_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (!(hWidgeBase->isVisable)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, (&(hWidgeBase->rect)))) {
			switch (hMsg->msgID) {
			case MSG_TOUCH_MOVE:
				break;
			case MSG_TOUCH_PRESS:
				if (hWidgeBase->viewClickCallBack != NULL) {
					hWidgeBase->viewClickCallBack(hWidgeBase, _GetBtnStatus(hWidgeBase));
				}
				_SetBtnPress(hWidgeBase);
				WindowsInvaildRect(hWidgeBase->parentHWIN, hWidgeBase);
				break;
			case MSG_TOUCH_RELEASE:
				if (_GetBtnStatus(hWidgeBase)) {
					if (hWidgeBase->viewClickCallBack != NULL) {
						hWidgeBase->viewClickCallBack(hWidgeBase, _GetBtnStatus(hWidgeBase));
					}
					_SetBtnRelease(hWidgeBase);
					WindowsInvaildRect(hWidgeBase->parentHWIN, hWidgeBase);
				}
				break;
			}
			return 0;
		}
	}
	return -1;
}
