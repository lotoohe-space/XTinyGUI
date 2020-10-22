#include "check_box_widge.h"
#include "paint.h"
#include "gui.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"
#include "bitmap.h"


const uint8 checkBoxDefuImg0[] = {
	________,  ________,//0
	________,  ______XX,//1
	________,  ______XX,//2
	________,  _____XX_,//3
	________,  _____XX_,//4
	________,  ____XX__,//5
	________,  ____XX__,//6
	________,  ___XX___,//7
	_XX_____,  ___XX___,//8
	__XX____,  __XX____,//9
	___XX___,  __XX____,//10
	____XX__,  _XX_____,//11
	_____XX_,  _XX_____,//12
	______XX,  XX______,//13
	_______X,  X_______,//14
	_______X,  X_______,//15

};

/*创建函数*/
PUBLIC HCHECKBOX_WIDGE CHECKBOX_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h,const char *text) {
	HCHECKBOX_WIDGE hWidge = (HCHECKBOX_WIDGE)(xMalloc(sizeof(CHECKBOX_WIDGE)));
	if (hWidge == NULL) {
		return NULL;
	}
	_WIDGET(Init)((HWIDGET_BASE)hWidge, x, y, w, h);
	
	/*设置三个回调函数*/
	_PToHTextWidgeType(hWidge)->baseWidge.paintFun = CHECKBOX_MARK_HEAD(Paint);
	_PToHTextWidgeType(hWidge)->baseWidge.moveToFun = CHECKBOX_MARK_HEAD(MoveTo);
	_PToHTextWidgeType(hWidge)->baseWidge.widgeCallBackFun = CHECKBOX_MARK_HEAD(CallBack);
	//hWidge->widgeBase.widgeCloseFun = CHECKBOX_MARK_HEAD(Close);

	_PToHTextWidgeType(hWidge)->text = text;
	_PToHTextWidgeType(hWidge)->hFont = (HFONTF)& fontASCII16_16;
	hWidge->flag = 0;

	/*初始化图片*/
	InitBitmap(&(hWidge->checkedImg), (uint8*)checkBoxDefuImg0, 16, 16, 1);

	return hWidge;
}
/*设置点击回调*/
void CHECKBOX_MARK_HEAD(SetClickBack)(HCHECKBOX_WIDGE hObject, void* arg,ViewClickCallBack viewClickCallBack) {
	_WIDGET(SetClickBack)((HWIDGET_BASE)hObject, arg, viewClickCallBack);
	/*if (!hObject) { return; }
	hObject->viewClickCallBack = viewClickCallBack;*/
}
/*设置状态，内部调用的函数*/
PRIVATE void CHECKBOX_MARK_HEAD(SetStatus)(HCHECKBOX_WIDGE hObject, uint8 status) {
	if (!hObject) { return; }
	hObject->flag = status & 0x3;
	WindowsInvaildRect(WIDGE_PARENT(hObject), (HXRECT)hObject);
}
/*过去状态*/
PUBLIC uint8 CHECKBOX_MARK_HEAD(GetStatus)(HCHECKBOX_WIDGE hObject){
	if (!hObject) { return 0; }
	return (hObject->flag) & 0x3;
}
/*移动函数*/
PUBLIC void CHECKBOX_MARK_HEAD(MoveTo)(HCHECKBOX_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	_PToHTextWidgeType(hObject)->baseWidge.rect.x = x;
	_PToHTextWidgeType(hObject)->baseWidge.rect.y = y;
	//WindowsInvaildRect(hObject->widgeBase.parentHWIN, (HXRECT)hObject);
}
/*设置父窗口*/
PUBLIC void CHECKBOX_MARK_HEAD(SetParentWin)(HCHECKBOX_WIDGE hObject, HWIDGET_BASE parentWidge) {
	if (!hObject) { return; }
	_PToHTextWidgeType(hObject)->baseWidge.parentHWIN = parentWidge;
}
/*设置颜色*/
PUBLIC void CHECKBOX_MARK_HEAD(SetColor)(HCHECKBOX_WIDGE hObject, uintColor color) {
	if (!hObject) { return; }
	_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawColor = color;
}
/*绘制控件*/
PUBLIC void CHECKBOX_MARK_HEAD(Paint)(void* hObject) {
	uint16 drawW;
	XRECT xRect;
	//float progressing;
	HCHECKBOX_WIDGE hWidge;
	hWidge = hObject;
	if (!hObject) { return; }
	if (!_GetVisable(hWidge)) { return; }
	//if (!IsGUINeedCut(hWidge)) { return; }

	if (!DrawSetArea((HWIDGET_BASE)hWidge)) { return; }

	_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawColor = _PToHTextWidgeType(hObject)->baseWidge.pencil.DrawFrColor;

	drawW = WIDGE_H(hWidge);

	/*画边框*/
	xRect.x = WIDGE_X(hWidge);
	xRect.y = WIDGE_Y(hWidge);
	xRect.w = drawW;
	xRect.h = 1;
	DrawCutRect(hWidge, &xRect);

	xRect.x = WIDGE_X(hWidge);
	xRect.y = WIDGE_Y(hWidge) + WIDGE_H(hWidge) - 1;
	xRect.w = drawW;
	xRect.h = 1;
	DrawCutRect(hWidge, &xRect);

	xRect.x = WIDGE_X(hWidge);
	xRect.y = WIDGE_Y(hWidge);
	xRect.w = 1;
	xRect.h = WIDGE_H(hWidge);
	DrawCutRect(hWidge, &xRect);

	xRect.x = WIDGE_X(hWidge) + drawW - 1;
	xRect.y = WIDGE_Y(hWidge);
	xRect.w = 1;
	xRect.h = WIDGE_H(hWidge);
	DrawCutRect(hWidge, &xRect);
	
	/*画勾*/
	xRect.x = WIDGE_X(hWidge) + 1;
	xRect.y = WIDGE_Y(hWidge) + 1;
	xRect.w = drawW - 2;
	xRect.h = WIDGE_H(hWidge) - 2;
	switch (CHECKBOX_MARK_HEAD(GetStatus)(hWidge)) {
	case 0:/*未选择状态*/
	{
		_PToHTextWidgeType(hObject)->baseWidge.pencil.DrawColor = _PToHTextWidgeType(hObject)->baseWidge.pencil.DrawBkColor;
		DrawCutRect(hWidge, &xRect);
		XPOINT xPoint;
		xPoint.x = 0;
		xPoint.y = 0;
		XRECT xRect;
		xRect.x = WIDGE_X(hWidge) + drawW;
		xRect.y = WIDGE_Y(hWidge);
		xRect.w = WIDGE_W(hWidge) - drawW;
		xRect.h = WIDGE_H(hWidge);
		//DrawCutChar(hWidge, hWidge->hFONTF, &xRect, &xPoint, 'S');
		DrawCutString(hWidge, _PToHTextWidgeType(hObject)->hFont, &xRect, &xPoint, _PToHTextWidgeType(hObject)->text);
	}
		break;
	case 1:/*点击状态*/
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
		DrawCutString(hWidge, _PToHTextWidgeType(hObject)->hFont, &xRect, &xPoint, _PToHTextWidgeType(hObject)->text);
		break;
	case 2:
		break;
	}
	/*恢复绘图区域*/
	DrawResetArea((HWIDGET_BASE)hWidge);
}
/*事件回调*/
int8 CHECKBOX_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
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
				if (CHECKBOX_MARK_HEAD(GetStatus)((HCHECKBOX_WIDGE)hWidgeBase)==1) {
					CHECKBOX_MARK_HEAD(SetStatus)((HCHECKBOX_WIDGE)hWidgeBase,0);
					if (_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack != NULL) {
						_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack(hWidgeBase, hWidgeBase->arg,
							CHECKBOX_MARK_HEAD(GetStatus)((HCHECKBOX_WIDGE)hWidgeBase));
					}
				}
				else if (CHECKBOX_MARK_HEAD(GetStatus)((HCHECKBOX_WIDGE)hWidgeBase) == 0) {
					CHECKBOX_MARK_HEAD(SetStatus)((HCHECKBOX_WIDGE)hWidgeBase,1);
					if (_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack != NULL) {
						_PToHWidgeBaseType(hWidgeBase)->viewClickCallBack(hWidgeBase, hWidgeBase->arg,
							CHECKBOX_MARK_HEAD(GetStatus)((HCHECKBOX_WIDGE)hWidgeBase));
					}
				}
				break;
			}
			return 0;
		}
	}
	return -1;
}
