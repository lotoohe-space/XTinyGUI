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
	/*�ؼ���С*/
	hWidgeBase->rect.x = x;
	hWidgeBase->rect.y = y;
	hWidgeBase->rect.w = w;
	hWidgeBase->rect.h = h;

	/*���������ص�����*/
	hWidgeBase->paintFun = WIDGE_MARK_HEAD(Paint);
	hWidgeBase->moveToFun = WIDGE_MARK_HEAD(MoveTo);
	hWidgeBase->widgeCallBackFun = WIDGE_MARK_HEAD(CallBack);
	hWidgeBase->widgeCloseFun = WIDGE_MARK_HEAD(Close);
	/*������ɫ*/
	hWidgeBase->pencil.DrawColor = _DefaultFrColor;
	hWidgeBase->pencil.DrawBkColor = _DefaultBKColor;

	/*Ĭ�ϻ�ͼ����Ϊȫ��*/
	hWidgeBase->pencil.x = x;
	hWidgeBase->pencil.y = y;
	hWidgeBase->pencil.w = w;
	hWidgeBase->pencil.h = h;

	/*Ĭ�Ͽɼ�*/
	hWidgeBase->isVisable = TRUE;

	hWidgeBase->viewClickCallBack = NULL;

	/*��ǰ�ؼ����Ǵ���*/
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
/*�����Ƿ�ɼ�*/
PUBLIC void WIDGE_MARK_HEAD(SetVisable)(HWIDGE_BASE hObject, uint8 isVisable) {
	if (!hObject) { return; }
	hObject->isVisable = isVisable;
	WindowsInvaildRect(hObject->parentHWIN, hObject);
}

/*���ø��ؼ�*/
PUBLIC void WIDGE_MARK_HEAD(SetParentWin)(HWIDGE_BASE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->parentHWIN = hWIN;
}
/*����ǰ��ɫ*/
PUBLIC void WIDGE_MARK_HEAD(SetColor)(HWIDGE_BASE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->pencil.DrawColor = color;
	WindowsInvaildRect(hObject->parentHWIN, hObject);
}
/*�ƶ��ؼ�*/
PUBLIC void WIDGE_MARK_HEAD(MoveTo)(HWIDGE_BASE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->rect.x = x;
	hObject->rect.y = y;
	WindowsInvaildRect(hObject->parentHWIN, hObject);
}
/*�ػ溯��*/
PRIVATE void WIDGE_MARK_HEAD(Paint)(void *hObject) {
	HWIDGE_BASE hWidgeBase;
	hWidgeBase = hObject;
	if (!hObject) { return; }
	if (hWidgeBase->isVisable == 0) { return; }
	//if (!isGUINeedCut(hWidgeBase)) { return; }

	DrawSetArea(hWidgeBase);

	//���Ƶ�ɫ
	//hWidgeBase->pencil.DrawColor = _DefaultFrColor;
	//���Ƶ�ɫ
	RECT_CUT_INIT(&(hWidgeBase->rect)) {
		DrawRect(&(hWidgeBase->pencil), nextCutRect);
	}RECT_CUT_END()
}
/*�¼��ص�����*/
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
