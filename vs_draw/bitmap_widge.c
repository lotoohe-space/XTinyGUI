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
	WidgeInit((HWIDGE_BASE)hBitmapWidge, x, y, 0, 0);
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

	/*设置三个回调函数*/
	hBitmapWidge->bitmapWidge.paintFun = BITMAP_MARK_HEAD(Paint);
	hBitmapWidge->bitmapWidge.moveToFun = BITMAP_MARK_HEAD(MoveTo);
	hBitmapWidge->bitmapWidge.widgeCallBackFun = BITMAP_MARK_HEAD(CallBack);

	/*设置颜色*/
	hBitmapWidge->bitmapWidge.pencil.DrawColor = _DefaultFrColor;

	hBitmapWidge->bitmapWidge.pencil.x = x;
	hBitmapWidge->bitmapWidge.pencil.y = y;
	hBitmapWidge->bitmapWidge.pencil.w = w;
	hBitmapWidge->bitmapWidge.pencil.h = h;

	/*设置显示的图片*/
	hBitmapWidge->bitmap = hXBitmap;

	hBitmapWidge->bitmapWidge.isVisable = TRUE;

	return hBitmapWidge;
}
PUBLIC void BITMAP_MARK_HEAD(SetBitmap)(HBITMAP_WIDGE hObject, HXBITMAP hXBitmap) {
	if (!hObject) { return; }
	/*设置显示的图片*/
	hObject->bitmap = hXBitmap;
	WindowsInvaildRect(hObject, (HXRECT)hObject);
}
PUBLIC void BITMAP_MARK_HEAD(MoveTo)(HBITMAP_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->bitmapWidge.rect.x = x;
	hObject->bitmapWidge.rect.y = y;
}

PUBLIC void BITMAP_MARK_HEAD(SetParentWin)(HBITMAP_WIDGE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->bitmapWidge.parentHWIN = hWIN;
}
PUBLIC void BITMAP_MARK_HEAD(SetColor)(HBITMAP_WIDGE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->bitmapWidge.pencil.DrawColor = color;
}
PUBLIC void BITMAP_MARK_HEAD(Paint)(void* hObject) {
	HBITMAP_WIDGE hBitmapWidge;
	hBitmapWidge = hObject;
	if (!hObject) { return; }
	if (hBitmapWidge->bitmapWidge.isVisable == 0) { return; }
	//if (!isGUINeedCut(hWidgeBase)) { return; }

	DrawSetArea(hBitmapWidge);
	//DrawCutRect(hBitmapWidge, hBitmapWidge);
	DrawCutBitmap(hBitmapWidge, (HXRECT)hBitmapWidge, hBitmapWidge->bitmap);

}

int8 BITMAP_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (!(hWidgeBase->isVisable)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, hWidgeBase->pencil.x, hWidgeBase->pencil.y, hWidgeBase->pencil.w, hWidgeBase->pencil.h)) {
			return 1;
		}
	}
	return -1;
}
