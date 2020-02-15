#include "slide_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"

#define SLIDE_DEFAULT_MAX_VAL 100

PUBLIC HSLIDE_WIDGE SLIDE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h) {
	HSLIDE_WIDGE hWidgeBase = (HSLIDE_WIDGE)(xMalloc(sizeof(SLIDE_WIDGE)));
	if (hWidgeBase == NULL) {
		return NULL;
	}
	WIDGE_MARK_HEAD(Init)((HWIDGE_BASE)hWidgeBase, x, y, w, h);
	
	/*设置三个回调函数*/
	_PToHWidgeBaseType(hWidgeBase)->paintFun = SLIDE_MARK_HEAD(Paint);
	//_PToHWidgeBaseType(hWidgeBase)->moveToFun = SLIDE_MARK_HEAD(MoveTo);
	_PToHWidgeBaseType(hWidgeBase)->widgeCallBackFun = SLIDE_MARK_HEAD(CallBack);
	//_PToHWidgeBaseType(hWidgeBase)->widgeCloseFun = SLIDE_MARK_HEAD(Close);
	//_PToHWidgeBaseType(hWidgeBase)->widgeResizeFun = SLIDE_MARK_HEAD(Resize);

	hWidgeBase->baseWidge.pencil.DrawColor = RGB565_BLACK;
	hWidgeBase->baseWidge.pencil.DrawFrColor = _DefaultFrColor;
	hWidgeBase->baseWidge.pencil.DrawBkColor = RGB565(230, 235, 230);

	hWidgeBase->maxVal = SLIDE_DEFAULT_MAX_VAL;
	hWidgeBase->currentVal = 50;

	return hWidgeBase;
}

/*重绘函数*/
PUBLIC void SLIDE_MARK_HEAD(Paint)(void* hObject) {
	HSLIDE_WIDGE hWidgeBase;
	hWidgeBase = hObject;
	if (!hObject) { return; }
	if (!_GetVisable(hWidgeBase)) { return; }
	//if (!IsGUINeedCut(hWidgeBase)) { return; }

	if (!DrawSetArea((HWIDGE_BASE)hWidgeBase)) { return; }

	uint16 barH;
	uint16 slideBlockH;
	uint16 slideBlockW;
	/*得到相对坐标*/
	int16 x, y;
	uint16 w, h;

	int16 drawY;
	int16 posBCK;

	barH = 6;
	slideBlockH = WIDGE_H(hWidgeBase);
	slideBlockW = 8;

	x = CDE2OPPOSITE_X(hWidgeBase);
	y = CDE2OPPOSITE_Y(hWidgeBase);
	w = WIDGE_W(hWidgeBase);
	h = WIDGE_H(hWidgeBase);

	/*横栏的位置*/
	drawY = (h - barH) >> 1;
	/*块的位置*/
	posBCK = (int16)((w - slideBlockW) *
		((float)(hWidgeBase->currentVal) / (float)(hWidgeBase->maxVal)));
	/*中间的条*/
	XRECT drawRect;
	drawRect.x = WIDGE_X(hWidgeBase);
	drawRect.y = WIDGE_Y(hWidgeBase)+ drawY;
	drawRect.w = posBCK;
	drawRect.h = barH;
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawFrColor;
	DrawCutRect(hWidgeBase, &drawRect);

	drawRect.x = WIDGE_X(hWidgeBase) + posBCK + slideBlockW;
	drawRect.y = WIDGE_Y(hWidgeBase) + drawY;
	drawRect.w = w - posBCK - slideBlockW;
	drawRect.h = barH;
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawFrColor;
	DrawCutRect(hWidgeBase, &drawRect);
	/*上面的条*/
	drawRect.x = WIDGE_X(hWidgeBase);
	drawRect.y = WIDGE_Y(hWidgeBase);
	drawRect.w = posBCK;
	drawRect.h = drawY;
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawBkColor;
	DrawCutRect(hWidgeBase, &drawRect);

	drawRect.x = WIDGE_X(hWidgeBase) + posBCK + slideBlockW;
	drawRect.y = WIDGE_Y(hWidgeBase);
	drawRect.w = w - posBCK - slideBlockW;
	drawRect.h = drawY;
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawBkColor;
	DrawCutRect(hWidgeBase, &drawRect);

	/*下面的条*/
	drawRect.x = WIDGE_X(hWidgeBase);
	drawRect.y = WIDGE_Y(hWidgeBase) + drawY + barH;
	drawRect.w = posBCK;
	drawRect.h = drawY;
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawBkColor;
	DrawCutRect(hWidgeBase, &drawRect);

	drawRect.x = WIDGE_X(hWidgeBase) + posBCK + slideBlockW;
	drawRect.y = WIDGE_Y(hWidgeBase) + drawY + barH;
	drawRect.w = w - posBCK - slideBlockW;
	drawRect.h = drawY;
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawBkColor;
	DrawCutRect(hWidgeBase, &drawRect);

	/*中间的条*/
	drawRect.x = WIDGE_X(hWidgeBase)+ posBCK;
	drawRect.y = WIDGE_Y(hWidgeBase);
	drawRect.w = slideBlockW;
	drawRect.h = WIDGE_H(hWidgeBase);
	hWidgeBase->baseWidge.pencil.DrawColor = hWidgeBase->baseWidge.pencil.DrawFrColor;
	DrawCutRect(hWidgeBase, &drawRect);

	/*恢复绘图区域*/
	DrawResetArea((HWIDGE_BASE)hWidgeBase);
}

/*事件回调*/
PUBLIC int8 SLIDE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	int8 ret;
	HSLIDE_WIDGE hBaseWidge = hObject;
	if (!hBaseWidge || !hMsg) { return -1; }
	if (!_GetVisable(hBaseWidge)) { return -1; }

	ret = WIDGE_MARK_HEAD(CallBack)(hBaseWidge, hMsg);

	if (ret == RES_OK) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, (HXRECT)hBaseWidge)) {
			switch (hMsg->msgID) {
			case MSG_TOUCH_PRESS:

				break;
			case MSG_TOUCH_MOVE:
			{
				int16 Dx;

				Dx = P_CDE2OPPOSITE_X(hBaseWidge, hMsg->msgVal.rect.x);
				//Dy = P_CDE2OPPOSITE_Y(hBaseWidge, hMsg->msgVal.rect.y);
				hBaseWidge->currentVal = (uint16)(((float)Dx / (float)WIDGE_W(hBaseWidge)) *
					(hBaseWidge->maxVal ));
				WindowsInvaildRect(hObject, NULL);

			}
				break;
			case MSG_TOUCH_RELEASE:

				break;
			}
			return RES_OK;
		}
	}

	return ret;
}
