
#include "text_widge.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"

HTEXT_WIDGE TextWidegeCreate(char *text, int16 x, int16 y) {
	HTEXT_WIDGE hObject = (HTEXT_WIDGE)(xMalloc( sizeof(TEXT_WIDGE)));
	if (hObject == NULL) {
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hObject, x, y, 0, 0);
	hObject->hFont = (HFONTF)& fontASCII8_12;
	hObject->textWidge.rect.x = x;
	hObject->textWidge.rect.y = y;
	
	hObject->textWidge.paintFun = TextWidegePaint;
	hObject->textWidge.moveToFun = TextWidegeMoveTo;
	hObject->textWidge.widgeCallBackFun = TextWidegeCallBack;
	hObject->title = text ? text : _DefaultWinHeadName;
	hObject->textWidge.rect.w = hObject->hFont->fontInfo.w * TStrlen(hObject->title);;
	hObject->textWidge.rect.h = hObject->hFont->fontInfo.h;


	hObject->textWidge.pencil.DrawColor = _DefaultFontColor;
	hObject->textWidge.pencil.DrawBkColor = _DefaultHeadColor;
	hObject->textWidge.pencil.x = x;
	hObject->textWidge.pencil.y = y;
	hObject->textWidge.pencil.w = hObject->textWidge.rect.w;
	hObject->textWidge.pencil.h = hObject->textWidge.rect.h;

	hObject->textWidge.isVisable = TRUE;

	return hObject;
}
void TextSetText(HTEXT_WIDGE hObject, const char* title) {
	if (hObject == NULL) { return; }
	hObject->title = title;
	WindowsInvaildRect(hObject->textWidge.parentHWIN, (HXRECT)hObject);
}
void TextWidegeMoveTo(HTEXT_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->textWidge.rect.x = x;
	hObject->textWidge.rect.y = y;
}
void TextWidegePaint(void * hObject) {
	HTEXT_WIDGE hTextWidge;
	hTextWidge = hObject;
	if (!hObject) { return; }
	if (!(hTextWidge->textWidge.isVisable)) { return ; }
	//if (!isGUINeedCut((HXRECT)hTextWidge)) { return; }

	DrawSetArea(hTextWidge);
	//DrawCutRect(hTextWidge,
	//	&(hTextWidge->textWidge.rect));
	XPOINT xPoint;
	xPoint.x = 0;
	xPoint.y = 0;
//	DrawChar(&(hTextWidge->textWidge.pencil), hTextWidge->hFont, hTextWidge, hTextWidge, '0');
//	DrawCutStringEx(hTextWidge, &xPoint);
	DrawCutString(hTextWidge, hTextWidge->hFont, 
		&(hTextWidge->textWidge.rect),
		NULL,
		hTextWidge->title);
}
int8 TextWidegeCallBack(void *hObject, HMSGE hMsg) {
	HTEXT_WIDGE hTextWidge = hObject;
	if (!hTextWidge || !hMsg) { return -1; }
	if (!(hTextWidge->textWidge.isVisable)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, 
			hTextWidge->textWidge.rect.x, 
			hTextWidge->textWidge.rect.y,
			hTextWidge->textWidge.rect.w,
			hTextWidge->textWidge.rect.h)) {
			return 0;
		}
	}
	return -1;
}
