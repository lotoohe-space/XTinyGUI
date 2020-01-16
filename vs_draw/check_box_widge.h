#pragma once
#ifndef _CHECK_BOX_WIDGE_H__
#define _CHECK_BOX_WIDGE_H__

#include "widge.h"
#include "bitmap.h"
#include "xwindows.h"
#define CHECKBOX_MARK_HEAD(a) CheckBoxWidge##a

typedef struct {
	WIDGE_BASE widgeBase;
	//ViewClickCallBack viewClickCallBack;
	const char* text;
	HFONTF hFONTF;
	uint8 flag;/*0��1bit����ǰ״̬*/
	XBITMAP checkedImg;/*ѡ�к��ͼƬ*/

}*HCHECKBOX_WIDGE, CHECKBOX_WIDGE;



PUBLIC HCHECKBOX_WIDGE CHECKBOX_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, const char* text);
PUBLIC	void CHECKBOX_MARK_HEAD(MoveTo)(HCHECKBOX_WIDGE hObject, int16 x, int16 y);
PUBLIC	void CHECKBOX_MARK_HEAD(SetParentWin)(HCHECKBOX_WIDGE hObject, HWIN hWIN);
PUBLIC	void CHECKBOX_MARK_HEAD(SetColor)(HCHECKBOX_WIDGE hObject, uintColor color);
PUBLIC	void CHECKBOX_MARK_HEAD(Paint)(void* hObject);
int8	CHECKBOX_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
void CHECKBOX_MARK_HEAD(SetClickBack)(HCHECKBOX_WIDGE hObject, void* arg, ViewClickCallBack viewClickCallBack);

#endif
