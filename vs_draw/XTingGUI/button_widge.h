#ifndef _BUTTON_WIDGE_H__
#define _BUTTON_WIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "font.h"
#include "text_widge.h"

#define BUTTON_MARK_HEAD(a) ButtonWidge##a

/*按钮的管理结构体*/
typedef struct {
	TEXT_WIDGE textWidge;	/*继承文本控件*/

	uint8 flag;				/*0:bit 按钮状态*/
	
	uintColor downColor;
	uintColor upColor;
	
}*HXBUTTON, XBUTTON;

/*按下与松开颜色*/
#define _DefaultButtonDownColor RGB565(0xff,0xff,0xff)
#define _DefaultButtonUpColor RGB565(200,200,200)
/*字体颜色*/
#define _DefaultButtonFontColor 0x0000

PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(const char *text, int16 x, int16 y, int16 w, int16 h);
PUBLIC void		BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y);
PUBLIC void		BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, void* arg, ViewClickCallBack viewClickCallBack);

PRIVATE void	BUTTON_MARK_HEAD(Paint)(void *hObject);
PRIVATE int8	BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
