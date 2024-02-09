#ifndef _SLIDE_WIDGE_H__
#define _SLIDE_WIDGE_H__
#include "widge.h"

#define SLIDE_MARK_HEAD(a) SlideWidge##a

/*按钮的管理结构体*/
typedef struct
{
	widget_base_t baseWidge; /*继承WIDGE_BASE控件*/
	int16_t maxVal;	   /*最大值*/
	uint16_t currentVal; /*当前值*/
} *p_slide_widget_t, slide_widget_t;

PUBLIC p_slide_widget_t SLIDE_MARK_HEAD(Create)(int16_t x, int16_t y, int16_t w, int16_t h);
PUBLIC void SLIDE_MARK_HEAD(Paint)(void *hObject);
PUBLIC int8_t SLIDE_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg);

#endif
