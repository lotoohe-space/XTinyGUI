
#ifndef _CHECK_BOX_WIDGE_H__
#define _CHECK_BOX_WIDGE_H__

#include "bitmap.h"
#include "text_widge.h"
#define CHECKBOX_MARK_HEAD(a) CheckBoxWidge##a

typedef struct
{
	text_widget_t textWidge;
	uint8_t flag;			  /*0，1bit：当前状态*/
	xbitmap_t checkedImg; /*选中后的图片*/

} *p_checkbox_widget_t, checkbox_widget_t;

PUBLIC p_checkbox_widget_t CHECKBOX_MARK_HEAD(Create)(int16_t x, int16_t y, int16_t w, int16_t h, const char *text);
PUBLIC void CHECKBOX_MARK_HEAD(MoveTo)(p_checkbox_widget_t hObject, int16_t x, int16_t y);
PUBLIC void CHECKBOX_MARK_HEAD(SetParentWin)(p_checkbox_widget_t hObject, p_widget_base_t parentWidge);
PUBLIC void CHECKBOX_MARK_HEAD(SetColor)(p_checkbox_widget_t hObject, uintColor color);
PUBLIC void CHECKBOX_MARK_HEAD(Paint)(void *hObject);
PUBLIC int8_t CHECKBOX_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg);
PUBLIC void CHECKBOX_MARK_HEAD(SetClickBack)(p_checkbox_widget_t hObject, void *arg, ViewClickCallBack viewClickCallBack);

#endif
