#ifndef _BUTTON_WIDGE_H__
#define _BUTTON_WIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "font.h"
typedef struct {
	WIDGE_BASE buttonWidge;

	HFONTF hFont;//×ÖÌå
	char *text;
	uint8 flag;//0bit ×´Ì¬
	uintColor downColor;
	uintColor upColor;
	
}*HXBUTTON, XBUTTON;



#define _SetBtnPress(a)		_SET_BIT((a)->flag,0)
#define _SetBtnRelease(a)	_CLR_BIT((a)->flag,0)
#define _GetBtnStatus(a)	_GET_BIT((a)->flag,0)
//
#define _DefaultButtonDownColor RGB565(0xff,0xff,0xff)
#define _DefaultButtonUpColor RGB565(20,40,20)
//
#define _DefaultButtonFontColor 0x0000

HXBUTTON ButtonWidgeCreate(char *text, int16 x, int16 y, int16 w, int16 h);
void ButtonWidgeMoveTo(HXBUTTON hObject, int16 x, int16 y);
void ButtonWidgePaint(void *hObject);
int8 ButtonWidgeCallBack(void *hObject, HMSGE hMsg);

#endif
