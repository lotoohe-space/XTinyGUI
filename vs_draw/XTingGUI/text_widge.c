
#include "text_widge.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"

PUBLIC HTEXT_WIDGE TEXT_MARK_HEAD(Create)(char *text, int16 x, int16 y,uint16 w, uint16 h) {
	HTEXT_WIDGE hObject = (HTEXT_WIDGE)(xMalloc( sizeof(TEXT_WIDGE)));
	if (hObject == NULL) {
		return NULL;
	}
	TEXT_MARK_HEAD(Init)(hObject, text, x, y, w, h);
	return hObject;
}
PUBLIC void TEXT_MARK_HEAD(Init)(HTEXT_WIDGE hObject, char* text, int16 x, int16 y, uint16 w, uint16 h) {
	if (hObject == NULL) {
		return ;
	}
	WidgeInit((HWIDGE_BASE)hObject, x, y, w, h);

	hObject->baseWidge.paintFun = TEXT_MARK_HEAD(Paint);
	hObject->baseWidge.moveToFun = TEXT_MARK_HEAD(MoveTo);
	hObject->baseWidge.widgeCallBackFun = TEXT_MARK_HEAD(CallBack);

	hObject->hFont = (HFONTF)& fontUNICODE16_16;
	hObject->text = text ? text : "请设置";
	hObject->textMode = TEXT_CENTER_ALIGN;
}
PUBLIC void TEXT_MARK_HEAD(SetTextMode)(HTEXT_WIDGE hOjbect, uint8 textMode) {
	if (hOjbect == NULL) { return; }
	hOjbect->textMode = textMode;
	WindowsInvaildRect((HWIDGE_BASE)hOjbect, NULL);
}
PUBLIC void TEXT_MARK_HEAD(SetFont)(HTEXT_WIDGE hObject, HFONTF hFont) {
	if (hObject == NULL) { return; }
	hObject->hFont = hFont;
	WindowsInvaildRect((HWIDGE_BASE)hObject, NULL);
}
PUBLIC void TEXT_MARK_HEAD(SetText)(HTEXT_WIDGE hObject, const char* text) {
	if (hObject == NULL) { return; }
	hObject->text = text;
	WindowsInvaildRect((HWIDGE_BASE)hObject, NULL);
}
PUBLIC void TEXT_MARK_HEAD(MoveTo)(HTEXT_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->baseWidge.rect.x = x;
	hObject->baseWidge.rect.y = y;
}
/*根据模式得到文字绘制的起始位置*/
PUBLIC HXPOINT TEXT_MARK_HEAD(GetPOIByTextMode)(HTEXT_WIDGE hTextWidge, HXPOINT startPoint) {
	uint16 textW = 0;
	uint16 textH = 0;

	if (startPoint == NULL || hTextWidge == NULL) { return NULL; }

	startPoint->x = 0;
	startPoint->y = 0;

	if (hTextWidge->hFont->fontType == UNICODE_TYPE) {
		textW = UStrlen(hTextWidge->text) * hTextWidge->hFont->fontInfo.w;
	}
	else {
		textW = TStrlen(hTextWidge->text) * hTextWidge->hFont->fontInfo.w;
	}
	textH = hTextWidge->hFont->fontInfo.h;

	switch (hTextWidge->textMode) {
	case TEXT_TOP_LEFT_ALIGN:
	{
		startPoint->x = 0;
		startPoint->y = 0;
	}
	break;
	case TEXT_TOP_CENTER_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW) / 2;
	}
	break;
	case TEXT_TOP_RIGHT_ALIGN: {
		startPoint->x = (WIDGE_W(hTextWidge) - textW);
	}
	break;
	case TEXT_BOTTOM_LEFT_ALIGN:
	{
		startPoint->y = WIDGE_H(hTextWidge) - textH;
	}
	break;
	case TEXT_BOTTOM_CENTER_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW) / 2;
		startPoint->y = WIDGE_H(hTextWidge) - textH;
	}
	break;
	case TEXT_BOTTOM_RIGHT_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW);
		startPoint->y = WIDGE_H(hTextWidge) - textH;
	}
	break;
	case TEXT_LEFT_CENTER_ALIGN:
		startPoint->y = (WIDGE_H(hTextWidge) - textH) / 2;
		break;
	case TEXT_RIGHT_CENTER_ALIGN:
		startPoint->x = (WIDGE_W(hTextWidge) - textW);
		startPoint->y = (WIDGE_H(hTextWidge) - textH) / 2;
		break;
	case TEXT_CENTER_ALIGN:/*居中对齐*/
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW) / 2;
		startPoint->y = (WIDGE_H(hTextWidge) - textH) / 2;
	}
	break;
	}

	return startPoint;
}
PUBLIC void TEXT_MARK_HEAD(Paint)(void * hObject) {
	XPOINT startPoint;
	HTEXT_WIDGE hTextWidge;
	hTextWidge = hObject;
	if (!hObject) { return; }
	if (!_GetVisable(hTextWidge)) { return ; }
	//if (!isGUINeedCut((HXRECT)hTextWidge)) { return; }

	DrawSetArea(hTextWidge);
	TEXT_MARK_HEAD(GetPOIByTextMode)(hTextWidge, &startPoint);
	/*绘制剪裁字符串*/
	DrawCutString(hTextWidge, hTextWidge->hFont, 
		&(hTextWidge->baseWidge.rect),
		&startPoint,
		hTextWidge->text);
}
PUBLIC int8 TEXT_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	HTEXT_WIDGE hTextWidge = hObject;
	if (!hTextWidge || !hMsg) { return -1; }
	if (!_GetVisable(hTextWidge)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, 
			&(hTextWidge->baseWidge.rect))) {
			return 0;
		}
	}
	return -1;
}
