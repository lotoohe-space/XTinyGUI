#ifndef _BUTTON_WIDGE_H__
#define _BUTTON_WIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "font.h"
#include "text_widge.h"

#define BUTTON_MARK_HEAD(a) ButtonWidge##a

/*��ť�Ĺ���ṹ��*/
typedef struct {
	TEXT_WIDGE textWidge;	/*�̳��ı��ؼ�*/

	uint8 flag;				/*0:bit ��ť״̬*/
	
	uintColor downColor;
	uintColor upColor;
	
}*HXBUTTON, XBUTTON;

/*�������ɿ���ɫ*/
#define _DefaultButtonDownColor RGB565(0xff,0xff,0xff)
#define _DefaultButtonUpColor RGB565(200,200,200)
/*������ɫ*/
#define _DefaultButtonFontColor 0x0000

PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(const char *text, int16 x, int16 y, int16 w, int16 h);
PUBLIC void		BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y);
PUBLIC void		BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, void* arg, ViewClickCallBack viewClickCallBack);

PRIVATE void	BUTTON_MARK_HEAD(Paint)(void *hObject);
PRIVATE int8	BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
