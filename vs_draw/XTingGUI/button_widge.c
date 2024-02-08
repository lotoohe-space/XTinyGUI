
#include "paint.h"
#include "paint_cut.h"
#include "button_widge.h"
#include "x_malloc.h"

/*����һ��button*/
PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(const char* text, int16 x, int16 y, int16 w, int16 h) {
	HXBUTTON hXButton = xMalloc(sizeof(XBUTTON));
	if (hXButton == NULL) { return NULL; }
	TEXT_MARK_HEAD(Init)((HTEXT_WIDGE)hXButton, text, x, y, w, h);

	_PToHWidgeBaseType(hXButton)->paintFun = BUTTON_MARK_HEAD(Paint);
	_PToHWidgeBaseType(hXButton)->moveToFun = BUTTON_MARK_HEAD(MoveTo);
	_PToHWidgeBaseType(hXButton)->widgeCallBackFun = BUTTON_MARK_HEAD(CallBack);

	//_PToHTextWidgeType(hXButton)->hFont = (HFONTF)& fontGB231216_16;
	hXButton->flag = 0;

	hXButton->downBitmap = NULL;
	hXButton->upBitmap = NULL;

	hXButton->downColor = _DefaultButtonDownColor;
	hXButton->upColor = _DefaultButtonUpColor;

	_PToHTextWidgeType(hXButton)->textMode = TEXT_CENTER_ALIGN;

	return hXButton;
}
PUBLIC void BUTTON_MARK_HEAD(SetDownBitmap)(HXBUTTON hObject, HXBITMAP hBitmap) {
	if (hObject == NULL) { return; }
	hObject->downBitmap = hBitmap;
}
PUBLIC void BUTTON_MARK_HEAD(SetUpBitmap)(HXBUTTON hObject, HXBITMAP hBitmap) {
	if (hObject == NULL) { return; }
	hObject->upBitmap = hBitmap;
}
/*����Buttton����*/
PUBLIC void BUTTON_MARK_HEAD(SetText)(HXBUTTON hObject, const char* text) {
	if (hObject == NULL) { return; }
	TEXT_MARK_HEAD(SetText)((HTEXT_WIDGE)hObject, text);
}
/*���õ���ص�*/
PUBLIC void BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, void* arg, ViewClickCallBack viewClickCallBack) {
	_WIDGET(SetClickBack)((HWIDGET_BASE)hObject, arg, viewClickCallBack);
}
/*�ƶ��ؼ���ָ��λ��*/
PUBLIC void BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	_WIDGET(MoveTo)((HWIDGET_BASE)hObject, x, y);
}
/*���ƴ���*/
PRIVATE void BUTTON_MARK_HEAD(Paint)(void* hObject) {
	HXBUTTON hXButton;
	//	XRECT xRECT;
	hXButton = hObject;
	if (!hObject) { return; }
	if (!DrawSetArea((HWIDGET_BASE)hXButton)) { return; }
	//if (!IsGUINeedCut(hXButton)) { return ; }

	if (!_GetBtnStatus(hXButton)) {/*�����ɿ�*/
		XPOINT startPoint;
		_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonUpColor;
		_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;
		if (!_GetBtnMode(hXButton)) {/*�ı�ģʽ*/
			TEXT_MARK_HEAD(GetPOIByTextMode)((HTEXT_WIDGE)hXButton, &startPoint);
			/*����ģʽȷ�����*/
			DrawCutString(hXButton,
				_PToHTextWidgeType(hXButton)->hFont,
				&(_PToHWidgeBaseType(hXButton)->rect),
				&startPoint,
				_PToHTextWidgeType(hXButton)->text);
		}
		else {/*ͼƬģʽ*/
			if (hXButton->upBitmap == NULL) {
				_PToHWidgeBaseType(hXButton)->pencil.DrawColor = _DefaultButtonUpColor;
				DrawCutRect(hXButton, _PToHXRECTType(hXButton));
			}
			else {
				DrawCutBitmap(hXButton, _PToHXRECTType(hXButton), hXButton->upBitmap);
			}
		}
	}
	else {/*��������*/
		XPOINT startPoint;
		_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonDownColor;
		_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;
		if (!_GetBtnMode(hXButton)) {/*�ı�ģʽ*/
			/*����ģʽȷ�����*/
			TEXT_MARK_HEAD(GetPOIByTextMode)((HTEXT_WIDGE)hXButton, &startPoint);
			DrawCutString(hXButton,
				_PToHTextWidgeType(hXButton)->hFont,
				&(_PToHWidgeBaseType(hXButton)->rect),
				&startPoint,
				_PToHTextWidgeType(hXButton)->text);
		}
		else {/*ͼƬģʽ``*/
			if (hXButton->upBitmap == NULL) {
				_PToHWidgeBaseType(hXButton)->pencil.DrawColor = _DefaultButtonDownColor;
				DrawCutRect(hXButton, _PToHXRECTType(hXButton));
			}
			else {
				DrawCutBitmap(hXButton, _PToHXRECTType(hXButton), hXButton->downBitmap);
			}
		}
	}
	/*�ָ���ͼ����*/
	DrawResetArea((HWIDGET_BASE)hXButton);
}
/*�¼��ص�*/
PRIVATE int8 BUTTON_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	int8 ret;
	HXBUTTON hXButton = hObject;
	if (!hXButton || !hMsg) { return RES_ASSERT_ERR; }
	if (!_GetVisable(hXButton)) { return RES_ASSERT_ERR; }

	ret = _WIDGET(CallBack)(hXButton, hMsg);
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

