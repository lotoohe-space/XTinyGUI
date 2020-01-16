#ifndef _BUTTON_WIDGE_H__
#define _BUTTON_WIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "font.h"

#define BUTTON_MARK_HEAD(a) ButtonWidge##a

/*按钮的管理结构体*/
typedef struct {
	WIDGE_BASE buttonWidge;
	

	HFONTF hFont;//字体
	char *text;
	uint8 flag;//0:bit 按钮状态
	
	uintColor downColor;
	uintColor upColor;
	
}*HXBUTTON, XBUTTON;



//#define _SetBtnPress(a)		_SET_BIT((a)->flag,0)
//#define _SetBtnRelease(a)	_CLR_BIT((a)->flag,0)
//#define _GetBtnStatus(a)	_GET_BIT((a)->flag,0)
//
#define _DefaultButtonDownColor RGB565(0xff,0xff,0xff)
#define _DefaultButtonUpColor RGB565(20,40,20)
//
#define _DefaultButtonFontColor 0x0000

PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(char *text, int16 x, int16 y, int16 w, int16 h);
PUBLIC void BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y);
PUBLIC void BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, ViewClickCallBack viewClickCallBack);

PRIVATE void BUTTON_MARK_HEAD(Paint)(void *hObject);
PRIVATE int8 BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
