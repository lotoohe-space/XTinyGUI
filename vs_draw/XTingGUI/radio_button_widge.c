#include "radio_button_widge.h"
#include "paint.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"
#include "bitmap.h"


static const uint8 radioButtonImg0[] = {
	_______X,  X_______,//0
	_____XXX,  XXX_____,//1
	___XXXXX,  XXXXX___,//2
	__XXXXXX,  XXXXXX__,//3
	_XXXXXXX,  XXXXXXX_,//4
	_XXXXXXX,  XXXXXXX_,//5
	XXXXXXXX,  XXXXXXXX,//6
	XXXXXXXX,  XXXXXXXX,//7
	XXXXXXXX,  XXXXXXXX,//8
	XXXXXXXX,  XXXXXXXX,//9
	_XXXXXXX,  XXXXXXX_,//10
	_XXXXXXX,  XXXXXXX_,//11
	__XXXXXX,  XXXXXX__,//12
	___XXXXX,  XXXXX___,//13
	_____XXX,  XXX_____,//14
	_______X,  X_______,//15
};
static const uint8 radioButtonImg1[] = {
	________,  ________,//0
	________,  ________,//1
	______XX,  XX______,//2
	_____XXX,  XXX_____,//3
	____XXXX,  XXXX____,//4
	___XXXXX,  XXXXX___,//5
	___XXXXX,  XXXXX___,//6
	__XXXXXX,  XXXXXX__,//7
	__XXXXXX,  XXXXXX__,//8
	___XXXXX,  XXXXX___,//9
	___XXXXX,  XXXXX___,//10
	____XXXX,  XXXX____,//11
	_____XXX,  XXX_____,//12
	______XX,  XX______,//13
	________,  ________,//14
	________,  ________,//15
};
/*创建函数*/
PUBLIC HRADIO_BUTTON_WIDGE RADIO_BUTTON_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, const char* text) {
	HRADIO_BUTTON_WIDGE hWidge = (HRADIO_BUTTON_WIDGE)(xMalloc(sizeof(RADIO_BUTTON_WIDGE)));
	if (hWidge == NULL) {
		return NULL;
	}
	_WIDGET(Init)((HWIDGET_BASE)hWidge, x, y, w, h);

	/*设置三个回调函数*/
	_PToHTextWidgeType(hWidge)->baseWidge.paintFun = RADIO_BUTTON_MARK_HEAD(Paint);
	_PToHTextWidgeType(hWidge)->baseWidge.moveToFun = RADIO_BUTTON_MARK_HEAD(MoveTo);
	_PToHTextWidgeType(hWidge)->baseWidge.widgeCallBackFun = RADIO_BUTTON_MARK_HEAD(CallBack);
	//hWidge->widgeBase.widgeCloseFun = RADIO_BUTTON_MARK_HEAD(Close);

	_PToHTextWidgeType(hWidge)->text = text;
	_PToHTextWidgeType(hWidge)->hFont = (HFONTF)& fontASCII16_16;
	hWidge->flag = 0;

	_SET_GROUP_CTRL(hWidge);
	/*初始化图片*/
	InitBitmap(&(hWidge->checkedImg), (uint8*)radioButtonImg0, 16, 16, 1);

	return hWidge;
}
/*设置点击回调*/
void RADIO_BUTTON_MARK_HEAD(SetClickBack)(HRADIO_BUTTON_WIDGE hObject, void* arg, ViewClickCallBack viewClickCallBack) {
	_WIDGET(SetClickBack)((HWIDGET_BASE)hObject, arg, viewClickCallBack);
	/*if (!hObject) { return; }
	hObject->viewClickCallBack = viewClickCallBack;*/
}
/*设置状态，内部调用的函数*/
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetStatus)(HRADIO_BUTTON_WIDGE hObject, uint8 status) {
	if (!hObject) { return; }
	hObject->flag = status & 0x3;
	if (status == TRUE) {
		GROUP_MARK_HEAD(GroupProcess)((HGROUP_WIDGE)(WIDGE_PARENT(hObject)), (HWIDGET_BASE)hObject);
	}
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
/*过去状态*/
PUBLIC uint8 RADIO_BUTTON_MARK_HEAD(GetStatus)(HRADIO_BUTTON_WIDGE hObject) {
	if (!hObject) { return 0; }
	return (hObject->flag) & 0x3;
}
/*移动函数*/
PUBLIC void RADIO_BUTTON_MARK_HEAD(MoveTo)(HRADIO_BUTTON_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	_PToHTextWidgeType(hObject)->baseWidge.rect.x = x;
	_PToHTextWidgeType(hObject)->baseWidge.rect.y = y;
	//WindowsInvaildRect(hObject->widgeBase.parentHWIN, (HXRECT)hObject);
}
/*设置父窗口*/
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetParentWin)(HRADIO_BUTTON_WIDGE hObject, HWIDGET_BASE parentWidge) {
	if (!hObject) { return; }
	_PToHTextWidgeType(hObject)->baseWidge.parentHWIN = parentWidge;
}
/*设置颜色*/
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetColor)(HRADIO_BUTTON_WIDGE hObject, uintColor color) {
	if (!hObject) { return; }
	_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawColor = color;
}
/*绘制控件*/
PUBLIC void RADIO_BUTTON_MARK_HEAD(Paint)(void* hObject) {
	uint16 drawW;
	XRECT xRect;
	//float progressing;
	HRADIO_BUTTON_WIDGE hWidge;
	hWidge = hObject;
	if (!hObject) { return; }
	if (!_GetVisable(hWidge)) { return; }
	//if (!IsGUINeedCut(hWidge)) { return; }

	if (!DrawSetArea((HWIDGET_BASE)hWidge)) { return; }

	drawW = 16;

	/*画勾*/
	xRect.x = WIDGE_X(hWidge);
	xRect.y = WIDGE_Y(hWidge);
	xRect.w = drawW ;
	xRect.h = WIDGE_H(hWidge);
	switch (RADIO_BUTTON_MARK_HEAD(GetStatus)(hWidge)) {
	case 0:/*未选择状态*/
	{
		hWidge->checkedImg.pixels = (uint8*)radioButtonImg0;
		_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawFrColor = 0xFFFF;
		DrawCutBitmap(hWidge, &xRect, &(hWidge->checkedImg));

		XPOINT xPoint;
		xPoint.x = 0;
		xPoint.y = 0;
		XRECT xRect;
		xRect.x = WIDGE_X(hWidge) + drawW;
		xRect.y = WIDGE_Y(hWidge);
		xRect.w = WIDGE_W(hWidge) - drawW;
		xRect.h = WIDGE_H(hWidge);
		_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawFrColor = 0x0000;
		DrawCutString(hWidge, _PToHTextWidgeType(hObject)->hFont, &xRect, &xPoint, _PToHTextWidgeType(hObject)->text);
		
	}
	break;
	case 1:/*点击状态*/
		hWidge->checkedImg.pixels = (uint8*)radioButtonImg1;
		_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawFrColor = 0x0000;
		DrawCutBitmap(hWidge, &xRect, &(hWidge->checkedImg));

		XPOINT xPoint;
		xPoint.x = 0;
		xPoint.y = 0;
		XRECT xRect;
		xRect.x = WIDGE_X(hWidge) + drawW;
		xRect.y = WIDGE_Y(hWidge);
		xRect.w = WIDGE_W(hWidge) - drawW;
		xRect.h = WIDGE_H(hWidge);
		//DrawCutChar(hWidge, hWidge->hFONTF, &xRect, &xPoint, 'S');
		DrawCutString(hWidge, _PToHTextWidgeType(hObject)->hFont, &xRect, &xPoint, 
			(uint8*)(_PToHTextWidgeType(hObject)->text));
		break;
	case 2:
		break;
	}
	/*恢复绘图区域*/
	DrawResetArea((HWIDGET_BASE)hWidge);
}
/*事件回调*/
int8 RADIO_BUTTON_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIDGET_BASE hWidgeBase = hObject;
	if (!hWidgeBase || !hMsg) { return -1; }
	if (!_GetVisable(hWidgeBase)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {

		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y, &(hWidgeBase->rect))) {
			switch (hMsg->msgID) {
			case MSG_TOUCH_MOVE:
				break;
			case MSG_TOUCH_PRESS:

				break;
			case MSG_TOUCH_RELEASE:
				if (RADIO_BUTTON_MARK_HEAD(GetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase) == 1) {
					RADIO_BUTTON_MARK_HEAD(SetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase, 0);
					if (_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack != NULL) {
						_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack(hWidgeBase, hWidgeBase->arg,
							RADIO_BUTTON_MARK_HEAD(GetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase));
					}
				}
				else if (RADIO_BUTTON_MARK_HEAD(GetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase) == 0) {
					RADIO_BUTTON_MARK_HEAD(SetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase, 1);
					if (_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack != NULL) {
						_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack(hWidgeBase, hWidgeBase->arg,
							RADIO_BUTTON_MARK_HEAD(GetStatus)((HRADIO_BUTTON_WIDGE)hWidgeBase));
					}
				}
				break;
			}
			return 0;
		}
	}
	return -1;
}
