#include "paint.h"
#include "bitmap_widge.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"

PUBLIC HBITMAP_WIDGE MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h,HXBITMAP hXBitmap) {
	HBITMAP_WIDGE hBitmapWidge = (HBITMAP_WIDGE)(xMalloc(sizeof(BITMAP_WIDGE)));
	if (hBitmapWidge == NULL) {
		return NULL;
	}

	hBitmapWidge->bitmapWidge.rect.x = x;
	hBitmapWidge->bitmapWidge.rect.y = y;
	if (hXBitmap != NULL) {
		hBitmapWidge->bitmapWidge.rect.w = hXBitmap->w;
		hBitmapWidge->bitmapWidge.rect.h = hXBitmap->h;
	}
	else {
		hBitmapWidge->bitmapWidge.rect.w = w;
		hBitmapWidge->bitmapWidge.rect.h = h;
	}

	/*���������ص�����*/
	hBitmapWidge->bitmapWidge.paintFun = MARK_HEAD(Paint);
	hBitmapWidge->bitmapWidge.moveToFun = MARK_HEAD(MoveTo);
	hBitmapWidge->bitmapWidge.widgeCallBackFun = MARK_HEAD(CallBack);

	/*������ɫ*/
	hBitmapWidge->bitmapWidge.pencil.DrawColor = _DefaultFrColor;

	hBitmapWidge->bitmapWidge.pencil.x = x;
	hBitmapWidge->bitmapWidge.pencil.y = y;
	hBitmapWidge->bitmapWidge.pencil.w = w;
	hBitmapWidge->bitmapWidge.pencil.h = h;

	/*������ʾ��ͼƬ*/
	hBitmapWidge->bitmap = hXBitmap;

	hBitmapWidge->isVisable = TRUE;

	return hBitmapWidge;
}
PUBLIC void MARK_HEAD(SetBitmap)(HBITMAP_WIDGE hObject, HXBITMAP hXBitmap) {
	if (!hObject) { return; }
	/*������ʾ��ͼƬ*/
	hObject->bitmap = hXBitmap;
	WindowsInvaildRect(hObject->bitmapWidge.parentHWIN, (HXRECT)hObject);
}
PUBLIC void MARK_HEAD(MoveTo)(HBITMAP_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->bitmapWidge.rect.x = x;
	hObject->bitmapWidge.rect.y = y;
}

PUBLIC void MARK_HEAD(SetParentWin)(HBITMAP_WIDGE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->bitmapWidge.parentHWIN = hWIN;
}
PUBLIC void MARK_HEAD(SetColor)(HBITMAP_WIDGE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->bitmapWidge.pencil.DrawColor = color;
}
PUBLIC void MARK_HEAD(Paint)(void* hObject) {
	HBITMAP_WIDGE hBitmapWidge;
	hBitmapWidge = hObject;
	if (!hObject) { return; }
	if (hBitmapWidge->isVisable == 0) { return; }
	//if (!isGUINeedCut(hWidgeBase)) { return; }

	DrawSetArea(hBitmapWidge);
	//DrawCutRect(hBitmapWidge, hBitmapWidge);
	DrawCutBitmap(hBitmapWidge, (HXRECT)hBitmapWidge, hBitmapWidge->bitmap);

}

int8 MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, hWidgeBase->pencil.x, hWidgeBase->pencil.y, hWidgeBase->pencil.w, hWidgeBase->pencil.h)) {
			return 0;
		}
	}
	return -1;
}