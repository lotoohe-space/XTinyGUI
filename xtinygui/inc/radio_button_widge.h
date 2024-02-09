#ifndef _RADIO_BUTTON_WIDGE_H__
#define _RADIO_BUTTON_WIDGE_H__

#include "bitmap.h"
#include "text_widge.h"

#define RADIO_BUTTON_MARK_HEAD(a) RadioButtonWidge##a

typedef struct
{
	TEXT_WIDGE textWidge;
	uint8 flag;			/*0，1bit：当前状态*/
	XBITMAP checkedImg; /*选中后的图片*/

} *HRADIO_BUTTON_WIDGE, RADIO_BUTTON_WIDGE;

PUBLIC HRADIO_BUTTON_WIDGE RADIO_BUTTON_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, const char *text);
PUBLIC void RADIO_BUTTON_MARK_HEAD(MoveTo)(HRADIO_BUTTON_WIDGE hObject, int16 x, int16 y);
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetParentWin)(HRADIO_BUTTON_WIDGE hObject, HWIDGET_BASE parentWidge);
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetStatus)(HRADIO_BUTTON_WIDGE hObject, uint8 status);
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetColor)(HRADIO_BUTTON_WIDGE hObject, uintColor color);
PUBLIC void RADIO_BUTTON_MARK_HEAD(Paint)(void *hObject);
PUBLIC int8 RADIO_BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);
PUBLIC void RADIO_BUTTON_MARK_HEAD(SetClickBack)(HRADIO_BUTTON_WIDGE hObject, void *arg, ViewClickCallBack viewClickCallBack);

#endif
