#include "paint.h"
#include "bitmap_widge.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"

PUBLIC HBITMAP_WIDGE BITMAP_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h,HXBITMAP hXBitmap) {
	HBITMAP_WIDGE hBitmapWidge = (HBITMAP_WIDGE)(xMalloc(sizeof(BITMAP_WIDGE)));
	if (hBitmapWidge == NULL) {
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hBitmapWidge, x, y, w, h);
	//hBitmapWidge->widgeBase.rect.x = x;
	//hBitmapWidge->widgeBase.rect.y = y;
	//hBitmapWidge->widgeBase.rect.w = w;
	//hBitmapWidge->widgeBase.rect.h = h;

	/*���������ص�����*/
	hBitmapWidge->widgeBase.paintFun = BITMAP_MARK_HEAD(Paint);
	hBitmapWidge->widgeBase.moveToFun = BITMAP_MARK_HEAD(MoveTo);
	hBitmapWidge->widgeBase.widgeCallBackFun = BITMAP_MARK_HEAD(CallBack);


	/*������ʾ��ͼƬ*/
	hBitmapWidge->bitmap = hXBitmap;

	return hBitmapWidge;
}
PUBLIC void BITMAP_MARK_HEAD(SetBitmap)(HBITMAP_WIDGE hObject, HXBITMAP hXBitmap) {
	if (!hObject) { return; }
	/*������ʾ��ͼƬ*/
	hObject->bitmap = hXBitmap;
	WindowsInvaildRect((HWIDGE_BASE)hObject, NULL);
}
PUBLIC void BITMAP_MARK_HEAD(MoveTo)(HBITMAP_WIDGE hObject, int16 x, int16 y) {
	WIDGE_MARK_HEAD(MoveTo)((HWIDGE_BASE)hObject, x, y);
	//if (!hObject) { return; }
	//hObject->widgeBase.rect.x = x;
	//hObject->widgeBase.rect.y = y;
}

PUBLIC void BITMAP_MARK_HEAD(SetParentWin)(HBITMAP_WIDGE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->widgeBase.parentHWIN = hWIN;
}
PUBLIC void BITMAP_MARK_HEAD(SetColor)(HBITMAP_WIDGE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->widgeBase.pencil.DrawColor = color;
}
PUBLIC void BITMAP_MARK_HEAD(Paint)(void* hObject) {
	HBITMAP_WIDGE hBitmapWidge;
	hBitmapWidge = hObject;
	if (!hObject) { return; }
	if (!_GetVisable(hBitmapWidge)) { return; }
	//if (!IsGUINeedCut(hBitmapWidge)) { return; }
	if (!DrawSetArea(hBitmapWidge)) { return; }
	DrawCutBitmap(hBitmapWidge, (HXRECT)hBitmapWidge, hBitmapWidge->bitmap);
}

int8 BITMAP_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (!_GetVisable(hWidgeBase)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, hWidgeBase->pencil.x, hWidgeBase->pencil.y, hWidgeBase->pencil.w, hWidgeBase->pencil.h)) {
			return 1;
		}
	}
	return -1;
}
