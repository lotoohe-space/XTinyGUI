
#include "paint.h"
#include "paint_cut.h"
#include "button_widge.h"
#include "x_malloc.h"

/*创建一个button*/
PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(const char *text, int16 x, int16 y, int16 w, int16 h) {
	HXBUTTON hXButton = xMalloc( sizeof(XBUTTON));
	if (hXButton == NULL) { return NULL; }
	TEXT_MARK_HEAD(Init)((HTEXT_WIDGE)hXButton, text, x, y, w, h);

	_PToHWidgeBaseType(hXButton)->paintFun = BUTTON_MARK_HEAD(Paint);
	_PToHWidgeBaseType(hXButton)->moveToFun = BUTTON_MARK_HEAD(MoveTo);
	_PToHWidgeBaseType(hXButton)->widgeCallBackFun = BUTTON_MARK_HEAD(CallBack);

	_PToHTextWidgeType(hXButton)->hFont = (HFONTF)& fontGB231216_16;
	hXButton->downColor = _DefaultButtonDownColor;
	hXButton->upColor = _DefaultButtonUpColor;

	_PToHTextWidgeType(hXButton)->textMode = TEXT_CENTER_ALIGN;
	//_PToHTextWidgeType(hXButton)->text = text;

	return hXButton;
}
/*设置Buttton文字*/
PUBLIC void BUTTON_MARK_HEAD(SetText)(HXBUTTON hObject,const char* text) {
	if (hObject == NULL) { return; }
	TEXT_MARK_HEAD(SetText)((HTEXT_WIDGE)hObject, text);
}
/*设置点击回调*/
PUBLIC void BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, void* arg,ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)((HWIDGE_BASE)hObject, arg, viewClickCallBack);
}
/*移动控件到指定位置*/
PUBLIC void BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	WIDGE_MARK_HEAD(MoveTo)((HWIDGE_BASE)hObject, x, y);
}
/*绘制窗口*/
PRIVATE void BUTTON_MARK_HEAD(Paint)(void *hObject) {
	HXBUTTON hXButton;
	XRECT xRECT;
	hXButton = hObject;
	if (!hObject) { return; }
	//if (!IsGUINeedCut((HXRECT)hXButton)) { return; }
	if (!DrawSetArea(hXButton)) { return; }

	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y;
	//xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w;
	//xRECT.h = 1;
	//DrawCutRect(hXButton, &xRECT);

	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y + _PToHWidgeBaseType(hXButton)->rect.h - 1;
	//xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w;
	//xRECT.h = 1;
	//DrawCutRect(hXButton, &xRECT);

	//xRECT.x =_PToHWidgeBaseType(hXButton)->rect.x;
	//xRECT.y =_PToHWidgeBaseType(hXButton)->rect.y;
	//xRECT.w = 1;
	//xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h;
	//DrawCutRect(hXButton, &xRECT);

	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x + _PToHWidgeBaseType(hXButton)->rect.w - 1;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y;
	//xRECT.w = 1;
	//xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h;
	//DrawCutRect(hXButton,&xRECT);


	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x + _PToHWidgeBaseType(hXButton)->rect.w - 2;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y + 1;
	//xRECT.w = 1;
	//xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h - 2;
	//DrawCutRect(hXButton,&xRECT);

	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x + 1;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y + _PToHWidgeBaseType(hXButton)->rect.h - 2;
	//xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w - 2;
	//xRECT.h = 1;
	//DrawCutRect(hXButton, &xRECT);

	//_PToHWidgeBaseType(hXButton)->pencil.DrawColor = RGB565(255, 255, 255);

	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x + 1;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y + 1;
	//xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w - 2;
	//xRECT.h = 1;
	//DrawCutRect(hXButton,&xRECT);

	//xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x + 1;
	//xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y + 1;
	//xRECT.w = 1;
	//xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h - 2;
	//DrawCutRect(hXButton, &xRECT);

	//if (!_GetBtnStatus(hXButton)) {
	//	XPOINT startPoint ;
	//	xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x + 2;
	//	xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y + 2;
	//	xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w - 4;
	//	xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h - 4;
	//	_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonUpColor;
	//	_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;

	//	TEXT_MARK_HEAD(GetPOIByTextMode)((HTEXT_WIDGE)hXButton, &startPoint);
	//	DrawCutString(hXButton, 
	//		_PToHTextWidgeType(hXButton)->hFont,
	//		&xRECT,
	//		&startPoint,
	//		_PToHTextWidgeType(hXButton)->text);
	//}
	//else {
	//	XPOINT startPoint;
	//	xRECT.x =_PToHWidgeBaseType(hXButton)->rect.x + 3;
	//	xRECT.y =_PToHWidgeBaseType(hXButton)->rect.y + 3;
	//	xRECT.w =_PToHWidgeBaseType(hXButton)->rect.w - 4;
	//	xRECT.h =_PToHWidgeBaseType(hXButton)->rect.h - 4;

	//	_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonDownColor;
	//	_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;

	//	TEXT_MARK_HEAD(GetPOIByTextMode)((HTEXT_WIDGE)hXButton, &startPoint);
	//	DrawCutString(hXButton, 
	//		_PToHTextWidgeType(hXButton)->hFont,
	//		&xRECT,
	//		&startPoint,
	//		_PToHTextWidgeType(hXButton)->text);
	//}

	if (!_GetBtnStatus(hXButton)) {
		XPOINT startPoint;
		/*xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x;
		xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y;
		xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w;
		xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h;*/
		_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonUpColor;
		_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;

		TEXT_MARK_HEAD(GetPOIByTextMode)((HTEXT_WIDGE)hXButton, &startPoint);
		DrawCutString(hXButton,
			_PToHTextWidgeType(hXButton)->hFont,
			&(_PToHWidgeBaseType(hXButton)->rect),
			&startPoint,
			_PToHTextWidgeType(hXButton)->text);
	}
	else {
		XPOINT startPoint;
		/*xRECT.x = _PToHWidgeBaseType(hXButton)->rect.x;
		xRECT.y = _PToHWidgeBaseType(hXButton)->rect.y;
		xRECT.w = _PToHWidgeBaseType(hXButton)->rect.w;
		xRECT.h = _PToHWidgeBaseType(hXButton)->rect.h;*/

		_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonDownColor;
		_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;

		TEXT_MARK_HEAD(GetPOIByTextMode)((HTEXT_WIDGE)hXButton, &startPoint);
		DrawCutString(hXButton,
			_PToHTextWidgeType(hXButton)->hFont,
			&(_PToHWidgeBaseType(hXButton)->rect),
			&startPoint,
			_PToHTextWidgeType(hXButton)->text);
	}
}
/*事件回调*/
PRIVATE int8 BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	int8 ret;
	HXBUTTON hXButton = hObject;
	if (!hXButton || !hMsg) { return RES_ASSERT_ERR; }
	if (!_GetVisable(hXButton)) { return RES_ASSERT_ERR; }

	ret = WIDGE_MARK_HEAD(CallBack)(hXButton, hMsg);
	if (ret == RES_OK) {
		if (_GetBtnStatus(hXButton)) {
			_PToHWidgeBaseType(hXButton)->pencil.DrawColor = hXButton->downColor;
		}
		else {
			_PToHWidgeBaseType(hXButton)->pencil.DrawColor = hXButton->upColor;
		}
	}
	return ret;
}

