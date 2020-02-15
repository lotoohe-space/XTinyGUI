
#ifndef _CHECK_BOX_WIDGE_H__
#define _CHECK_BOX_WIDGE_H__

//#include "widge.h"
#include "bitmap.h"
//#include "xwindows.h"
#include "text_widge.h"
#define CHECKBOX_MARK_HEAD(a) CheckBoxWidge##a

typedef struct {
	TEXT_WIDGE textWidge;
	//WIDGE_BASE widgeBase;
	
	//const char* text;
	//HFONTF hFONTF;
	uint8 flag;/*0，1bit：当前状态*/
	XBITMAP checkedImg;/*选中后的图片*/

}*HCHECKBOX_WIDGE, CHECKBOX_WIDGE;


PUBLIC HCHECKBOX_WIDGE	CHECKBOX_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, const char* text);
PUBLIC	void			CHECKBOX_MARK_HEAD(MoveTo)(HCHECKBOX_WIDGE hObject, int16 x, int16 y);
PUBLIC	void			CHECKBOX_MARK_HEAD(SetParentWin)(HCHECKBOX_WIDGE hObject, HWIDGE_BASE parentWidge);
PUBLIC	void			CHECKBOX_MARK_HEAD(SetColor)(HCHECKBOX_WIDGE hObject, uintColor color);
PUBLIC	void			CHECKBOX_MARK_HEAD(Paint)(void* hObject);
PUBLIC	int8			CHECKBOX_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
PUBLIC	void			CHECKBOX_MARK_HEAD(SetClickBack)(HCHECKBOX_WIDGE hObject, void* arg, ViewClickCallBack viewClickCallBack);

#endif
