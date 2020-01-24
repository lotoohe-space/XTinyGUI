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
	________,  ___XX___,//8
	_XX_____,  ___XX___,//9
	__XX____,  __XX____,//10
	___XX___,  __XX____,//11
	____XX__,  _XX_____,//12
	_____XX_,  _XX_____,//13
	______XX,  XX______,//14
	_______X,  X_______,//15
};

/*创建函数*/
PUBLIC HCHECKBOX_WIDGE CHECKBOX_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h,const char *text) {
	HCHECKBOX_WIDGE hWidge = (HCHECKBOX_WIDGE)(xMalloc(sizeof(CHECKBOX_WIDGE)));
	if (hWidge == NULL) {
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hWidge, x, y, w, h);

	/*设置三个回调函数*/
	hWidge->widgeBase.paintFun = CHECKBOX_MARK_HEAD(Paint);
	hWidge->widgeBase.moveToFun = CHECKBOX_MARK_HEAD(MoveTo);
	hWidge->widgeBase.widgeCallBackFun = CHECKBOX_MARK_HEAD(CallBack);
	//hWidge->widgeBase.widgeCloseFun = CHECKBOX_MARK_HEAD(Close);

	/*设置颜色*/

	/*初始化图片*/
	InitBitmap(&(hWidge->checkedImg), (uint8*)checkBoxDefuImg0, 16, 16, 1);

	hWidge->flag = 0;
	hWidge->text = text;
	hWidge->hFONTF = &fontGB231216_16;
	return hWidge;
}
/*设置点击回调*/
void CHECKBOX_MARK_HEAD(SetClickBack)(HCHECKBOX_WIDGE hObject, void* arg,ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)((HWIDGE_BASE)hObject, arg, viewClickCallBack);
	/*if (!hObject) { return; }
	hObject->viewClickCallBack = viewClickCallBack;*/
}
/*设置状态，内部调用的函数*/
PRIVATE void CHECKBOX_MARK_HEAD(SetStatus)(HCHECKBOX_WIDGE hObject, uint8 status) {
	if (!hObject) { return; }
	hObject->flag = status & 0x3;
	WindowsInvaildRect(hObject->widgeBase.parentHWIN, (HXRECT)hObject);
}
/*过去状态*/
PUBLIC uint8 CHECKBOX_MARK_HEAD(GetStatus)(HCHECKBOX_WIDGE hObject){
	if (!hObject) { return 0; }
	return (hObject->flag) & 0x3;
}
/*移动函数*/
PUBLIC void CHECKBOX_MARK_HEAD(MoveTo)(HCHECKBOX_WIDGE hObject, int16 x, int16 y) {
	if (!hObject) { return; }
	hObject->widgeBase.rect.x = x;
	hObject->widgeBase.rect.y = y;
	WindowsInvaildRect(hObject->widgeBase.parentHWIN, (HXRECT)hObject);
}
/*设置父窗口*/
PUBLIC void CHECKBOX_MARK_HEAD(SetParentWin)(HCHECKBOX_WIDGE hObject, HWIN hWIN) {
	if (!hObject) { return; }
	hObject->widgeBase.parentHWIN = hWIN;
}
/*设置颜色*/
PUBLIC void CHECKBOX_MARK_HEAD(SetColor)(HCHECKBOX_WIDGE hObject, uintColor color) {
	if (!hObject) { return; }
	hObject->widgeBase.pencil.DrawColor = color;
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
//	if (!isGUINeedCut(hWidge)) { return; }

	DrawSetArea(hWidge);

	drawW = hWidge->widgeBase.rect.h;

	/*画边框*/
	xRect.x = hWidge->widgeBase.rect.x;
	xRect.y = hWidge->widgeBase.rect.y;
	xRect.w = drawW;
	xRect.h = 1;
	DrawCutRect(hWidge, &xRect);

	xRect.x = hWidge->widgeBase.rect.x;
	xRect.y = hWidge->widgeBase.rect.y + hWidge->widgeBase.rect.h - 1;
	xRect.w = drawW;
	xRect.h = 1;
	DrawCutRect(hWidge, &xRect);

	xRect.x = hWidge->widgeBase.rect.x;
	xRect.y = hWidge->widgeBase.rect.y;
	xRect.w = 1;
	xRect.h = hWidge->widgeBase.rect.h;
	DrawCutRect(hWidge, &xRect);

	xRect.x = hWidge->widgeBase.rect.x + drawW - 1;
	xRect.y = hWidge->widgeBase.rect.y;
	xRect.w = 1;
	xRect.h = hWidge->widgeBase.rect.h;
	DrawCutRect(hWidge, &xRect);
	
	/*画勾*/
	xRect.x = hWidge->widgeBase.rect.x + 1;
	xRect.y = hWidge->widgeBase.rect.y + 1;
	xRect.w = drawW - 2;
	xRect.h = hWidge->widgeBase.rect.h - 2;
	switch (CHECKBOX_MARK_HEAD(GetStatus)(hWidge)) {
	case 0:/*未选择状态*/
	{
		uintColor color;
		color = hWidge->widgeBase.pencil.DrawColor;
		hWidge->widgeBase.pencil.DrawColor = 0xffff;
		DrawCutRect(hWidge, &xRect);
		hWidge->widgeBase.pencil.DrawColor = color;
		XPOINT xPoint;
		xPoint.x = 0;
		xPoint.y = 0;
		XRECT xRect;
		xRect.x = hWidge->widgeBase.rect.x+ drawW;
		xRect.y = hWidge->widgeBase.rect.y;
		xRect.w = hWidge->widgeBase.rect.w- drawW;
		xRect.h = hWidge->widgeBase.rect.h;
		//DrawCutChar(hWidge, hWidge->hFONTF, &xRect, &xPoint, 'S');
		DrawCutString(hWidge, hWidge->hFONTF, &xRect, &xPoint, hWidge->text);
	}
		break;
	case 1:/*点击状态*/
		DrawCutBitmap(hWidge, &xRect, &(hWidge->checkedImg));
		XPOINT xPoint;
		xPoint.x = 0;
		xPoint.y = 0;
		XRECT xRect;
		xRect.x = hWidge->widgeBase.rect.x + drawW;
		xRect.y = hWidge->widgeBase.rect.y;
		xRect.w = hWidge->widgeBase.rect.w - drawW;
		xRect.h = hWidge->widgeBase.rect.h;
		//DrawCutChar(hWidge, hWidge->hFONTF, &xRect, &xPoint, 'S');
		DrawCutString(hWidge, hWidge->hFONTF, &xRect, &xPoint, hWidge->text);
		break;
	case 2:
		break;
	}
	
}
/*事件回调*/
int8 CHECKBOX_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HWIDGE_BASE hWidgeBase = hObject;
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
					if (((HCHECKBOX_WIDGE)hWidgeBase)->widgeBase.viewClickCallBack != NULL) {
						((HCHECKBOX_WIDGE)hWidgeBase)->widgeBase.viewClickCallBack(hWidgeBase, hWidgeBase->arg,
							CHECKBOX_MARK_HEAD(GetStatus)((HCHECKBOX_WIDGE)hWidgeBase));
					}
				}
				else if (CHECKBOX_MARK_HEAD(GetStatus)((HCHECKBOX_WIDGE)hWidgeBase) == 0) {
					CHECKBOX_MARK_HEAD(SetStatus)((HCHECKBOX_WIDGE)hWidgeBase,1);
					if (((HCHECKBOX_WIDGE)hWidgeBase)->widgeBase.viewClickCallBack != NULL) {
						((HCHECKBOX_WIDGE)hWidgeBase)->widgeBase.viewClickCallBack(hWidgeBase, hWidgeBase->arg,
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
