
#include "progress_widge.h"
#include "paint.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"

/*��������*/
PUBLIC HPROGRESS_WIDGE PROGRESS_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, uint16 maxVal) {
	HPROGRESS_WIDGE hWidge = (HPROGRESS_WIDGE)(xMalloc(sizeof(PROGRESS_WIDGE)));
	if (hWidge == NULL) {
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hWidge, x, y, w, h);

	/*���������ص�����*/
	hWidge->widgeBase.paintFun = PROGRESS_MARK_HEAD(Paint);
	hWidge->widgeBase.moveToFun = PROGRESS_MARK_HEAD(MoveTo);
	hWidge->widgeBase.widgeCallBackFun = PROGRESS_MARK_HEAD(CallBack);

	hWidge->maxVal = maxVal;
	hWidge->currentVal = 0;

	return hWidge;
}

/*�ƶ�����*/
PUBLIC void PROGRESS_MARK_HEAD(MoveTo)(HPROGRESS_WIDGE hObject, int16 x, int16 y) {
	WIDGE_MARK_HEAD(MoveTo)((HWIDGE_BASE)hObject,x,y);
	//if (!hObject) { return; }
	//hObject->widgeBase.rect.x = x;
	//hObject->widgeBase.rect.y = y;
}
/*���ø�����*/
PUBLIC void PROGRESS_MARK_HEAD(SetParentWin)(HPROGRESS_WIDGE hObject, HWIN hWIN) {
	WIDGE_MARK_HEAD(SetParentWin)((HWIDGE_BASE)hObject, hWIN);
	//if (!hObject) { return; }
	//hObject->widgeBase.parentHWIN = hWIN;
}
/*������ɫ*/
PUBLIC void PROGRESS_MARK_HEAD(SetColor)(HPROGRESS_WIDGE hObject, uintColor color) {
	WIDGE_MARK_HEAD(SetColor)((HWIDGE_BASE)hObject, color);
	//if (!hObject) { return; }
	//hObject->widgeBase.pencil.DrawColor = color;
}

PUBLIC void PROGRESS_MARK_HEAD(SetProgressVal)(HPROGRESS_WIDGE hObject, uint16 val) {
	if (!hObject) { return; }
	hObject->currentVal = MIN(val, hObject->maxVal);
	WindowsInvaildRect(hObject->widgeBase.parentHWIN, (HXRECT)hObject);
}

/*���ƿؼ�*/
PRIVATE void PROGRESS_MARK_HEAD(Paint)(void* hObject) {
	uintColor color;
	XRECT xRect;
	float progressing;
	HPROGRESS_WIDGE hWidge;
	hWidge = hObject;
	if (!hObject) { return; }
	if (!_GetVisable(hWidge)) { return; }
	//if (!isGUINeedCut(hWidge)) { return; }

	DrawSetArea(hWidge);

	progressing = (hWidge->currentVal / (float)(hWidge->maxVal));
	/*����ǰ���*/
	xRect.x = hWidge->widgeBase.rect.x;
	xRect.y = hWidge->widgeBase.rect.y;
	xRect.w = (uint16)(hWidge->widgeBase.rect.w * progressing);
	xRect.h = hWidge->widgeBase.rect.h;
	DrawCutRect(hWidge,&xRect);
	/*���ƺ���*/
	color = hWidge->widgeBase.pencil.DrawColor;
	hWidge->widgeBase.pencil.DrawColor = hWidge->widgeBase.pencil.DrawBkColor;

	xRect.x = hWidge->widgeBase.rect.x + (uint16)(hWidge->widgeBase.rect.w * progressing);
	xRect.w = hWidge->widgeBase.rect.w - (uint16)(hWidge->widgeBase.rect.w * progressing);
	DrawCutRect(hWidge, &xRect);
	hWidge->widgeBase.pencil.DrawColor = color;

}
/*�¼��ص�*/
PRIVATE int8 PROGRESS_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (!_GetVisable(hWidgeBase)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, hWidgeBase->pencil.x, hWidgeBase->pencil.y, hWidgeBase->pencil.w, hWidgeBase->pencil.h)) {
			return 0;
		}
	}
	return -1;
}

