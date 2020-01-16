
#include "paint.h"
#include "paint_cut.h"
#include "button_widge.h"
#include "x_malloc.h"

PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(char *text,int16 x, int16 y, int16 w, int16 h) {
	HXBUTTON hXButton = xMalloc( sizeof(XBUTTON));
	if (hXButton == NULL) { return NULL; }
	WidgeInit((HWIDGE_BASE)hXButton, x, y, w, h);

	hXButton->hFont = (HFONTF)&fontASCII8_12;


	hXButton->buttonWidge.paintFun = ButtonWidgePaint;
	hXButton->buttonWidge.moveToFun = ButtonWidgeMoveTo;
	hXButton->buttonWidge.widgeCallBackFun = ButtonWidgeCallBack;
	hXButton->buttonWidge.pencil.DrawColor = _DefaultButtonUpColor;
	hXButton->buttonWidge.pencil.DrawBkColor = _DefaultButtonFontColor;

	hXButton->downColor = _DefaultButtonDownColor;
	hXButton->upColor = _DefaultButtonUpColor;

	hXButton->buttonWidge.pencil.x = x;
	hXButton->buttonWidge.pencil.y = y;
	hXButton->buttonWidge.pencil.w = w;
	hXButton->buttonWidge.pencil.h = h;

	hXButton->buttonWidge.isVisable = TRUE;

	hXButton->text = text;

	return hXButton;
}
PUBLIC void BUTTON_MARK_HEAD(SetText)(HXBUTTON hObject,const char* text) {
	if (hObject == NULL) { return; }
	hObject->text = text;
	WindowsInvaildRect(hObject->buttonWidge.parentHWIN, hObject);
}
/*设置点击回调*/
PUBLIC void BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)(hObject,viewClickCallBack);
}
PUBLIC void BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->buttonWidge.rect.x = x;
	hObject->buttonWidge.rect.y = y;
}
PRIVATE void BUTTON_MARK_HEAD(Paint)(void *hObject) {
	HXBUTTON hXButton;
	XRECT xRECT;
	hXButton = hObject;
	if (!hObject) { return; }
	if (!isGUINeedCut(hXButton)) { return; }

	DrawSetArea(hXButton);

	uintColor tempColor = hXButton->buttonWidge.pencil.DrawColor;
	hXButton->buttonWidge.pencil.DrawColor = RGB565(88,88,88);

	//xRECT.x = hXButton->buttonWidge.rect.x ;
	//xRECT.y = hXButton->buttonWidge.rect.y;
	//xRECT.w = hXButton->buttonWidge.rect.w / 2;
	//xRECT.h = hXButton->buttonWidge.rect.h;
	//DrawCutRect(hXButton,&xRECT);

	//hXButton->buttonWidge.pencil.DrawColor = RGB565(33, 33, 33);
	//xRECT.x = hXButton->buttonWidge.rect.x+ hXButton->buttonWidge.rect.w / 2;
	//xRECT.y = hXButton->buttonWidge.rect.y;
	//xRECT.w = hXButton->buttonWidge.rect.w / 2+1;
	//xRECT.h = hXButton->buttonWidge.rect.h;
	//DrawCutRect(hXButton, &xRECT);


	xRECT.x = hXButton->buttonWidge.rect.x;
	xRECT.y = hXButton->buttonWidge.rect.y;
	xRECT.w = hXButton->buttonWidge.rect.w;
	xRECT.h = 1;
	DrawCutRect(hXButton, &xRECT);

	xRECT.x = hXButton->buttonWidge.rect.x;
	xRECT.y = hXButton->buttonWidge.rect.y + hXButton->buttonWidge.rect.h - 1;
	xRECT.w = hXButton->buttonWidge.rect.w;
	xRECT.h = 1;
	DrawCutRect(hXButton, &xRECT);

	xRECT.x = hXButton->buttonWidge.rect.x;
	xRECT.y = hXButton->buttonWidge.rect.y;
	xRECT.w = 1;
	xRECT.h = hXButton->buttonWidge.rect.h;
	DrawCutRect(hXButton, &xRECT);

	xRECT.x = hXButton->buttonWidge.rect.x + hXButton->buttonWidge.rect.w - 1;
	xRECT.y = hXButton->buttonWidge.rect.y;
	xRECT.w = 1;
	xRECT.h = hXButton->buttonWidge.rect.h;
	DrawCutRect(hXButton,&xRECT);

	if (_GetBtnStatus(hXButton)) {
		hXButton->buttonWidge.pencil.DrawColor = RGB565(255, 255, 255);
	} else {
		hXButton->buttonWidge.pencil.DrawColor = RGB565(0, 0, 0);
	}


	xRECT.x = hXButton->buttonWidge.rect.x + hXButton->buttonWidge.rect.w - 2;
	xRECT.y = hXButton->buttonWidge.rect.y + 1;
	xRECT.w = 1;
	xRECT.h = hXButton->buttonWidge.rect.h - 2;
	DrawCutRect(hXButton,&xRECT);

	xRECT.x = hXButton->buttonWidge.rect.x + 1;
	xRECT.y = hXButton->buttonWidge.rect.y + hXButton->buttonWidge.rect.h - 2;
	xRECT.w = hXButton->buttonWidge.rect.w - 2;
	xRECT.h = 1;
	DrawCutRect(hXButton, &xRECT);

	hXButton->buttonWidge.pencil.DrawColor = RGB565(255, 255, 255);

	xRECT.x = hXButton->buttonWidge.rect.x + 1;
	xRECT.y = hXButton->buttonWidge.rect.y + 1;
	xRECT.w = hXButton->buttonWidge.rect.w - 2;
	xRECT.h = 1;
	DrawCutRect(hXButton,&xRECT);

	xRECT.x = hXButton->buttonWidge.rect.x + 1;
	xRECT.y = hXButton->buttonWidge.rect.y + 1;
	xRECT.w = 1;
	xRECT.h = hXButton->buttonWidge.rect.h - 2;
	DrawCutRect(hXButton, &xRECT);

	hXButton->buttonWidge.pencil.DrawColor = tempColor;

	xRECT.x = hXButton->buttonWidge.rect.x + 2;
	xRECT.y = hXButton->buttonWidge.rect.y + 2;
	xRECT.w = hXButton->buttonWidge.rect.w - 4;
	xRECT.h = hXButton->buttonWidge.rect.h - 4;
	DrawCutRect(hXButton, &xRECT);

	tempColor = hXButton->buttonWidge.pencil.DrawColor;
	
	hXButton->buttonWidge.pencil.DrawBkColor = tempColor;

	if (!_GetBtnStatus(hXButton)) {
		hXButton->buttonWidge.pencil.DrawColor = _DefaultButtonUpColor;
		//DrawCutRect(hXButton,hXButton);
		xRECT.x = hXButton->buttonWidge.rect.x + 2;
		xRECT.y = hXButton->buttonWidge.rect.y + 2;
		xRECT.w = hXButton->buttonWidge.rect.w - 4;
		xRECT.h = hXButton->buttonWidge.rect.h - 4;
		hXButton->buttonWidge.pencil.DrawBkColor = _DefaultButtonUpColor;
		hXButton->buttonWidge.pencil.DrawColor = _DefaultButtonFontColor;
		DrawCutString(hXButton, 
			hXButton->hFont,
			&xRECT,
			hXButton->text);
	}
	else {
		hXButton->buttonWidge.pencil.DrawColor = _DefaultButtonDownColor;
		//DrawCutRect(hXButton, hXButton);
		xRECT.x = hXButton->buttonWidge.rect.x + 3;
		xRECT.y = hXButton->buttonWidge.rect.y + 3;
		xRECT.w = hXButton->buttonWidge.rect.w - 4;
		xRECT.h = hXButton->buttonWidge.rect.h - 4;
		hXButton->buttonWidge.pencil.DrawBkColor = _DefaultButtonDownColor;
		hXButton->buttonWidge.pencil.DrawColor = _DefaultButtonFontColor;
		DrawCutString(hXButton, 
			hXButton->hFont,
			&xRECT,
			hXButton->text);
	}
	hXButton->buttonWidge.pencil.DrawColor = tempColor;
}
PRIVATE int8 BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	int8 ret;
	HXBUTTON hXButton = hObject;
	if (!hXButton || !hMsg) { return -1; }
	if (!(hXButton->buttonWidge.isVisable)) { return -1; }

	ret = WIDGE_MARK_HEAD(CallBack)(hXButton, hMsg);
	if (ret == 0) {
		if (_GetBtnStatus(hXButton)) {
			hXButton->buttonWidge.pencil.DrawColor = hXButton->downColor;
		}
		else {
			hXButton->buttonWidge.pencil.DrawColor = hXButton->upColor;
		}
	}
	return ret;
}

