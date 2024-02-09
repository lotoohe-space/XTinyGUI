
#ifndef _TEXT_WIDGE_H__
#define _TEXT_WIDGE_H__

#include "type.h"
#include "paint.h"
#include "widge.h"

#define TEXT_MARK_HEAD(a) TextWidge##a

/*文字的显示模式*/
typedef enum
{
	TEXT_TOP_LEFT_ALIGN = 0,  /*顶部左对齐*/
	TEXT_TOP_CENTER_ALIGN,	  /*顶部居中对齐*/
	TEXT_TOP_RIGHT_ALIGN,	  /*顶部右对齐*/
	TEXT_BOTTOM_LEFT_ALIGN,	  /*底部左边对齐*/
	TEXT_BOTTOM_CENTER_ALIGN, /*底部居中对齐*/
	TEXT_BOTTOM_RIGHT_ALIGN,  /*底部右对齐*/
	TEXT_LEFT_CENTER_ALIGN,	  /*居中左对齐*/
	TEXT_RIGHT_CENTER_ALIGN,  /*居中右对齐*/
	TEXT_CENTER_ALIGN		  /*居中对齐*/
} TextMode;

typedef struct
{
	widget_base_t baseWidge; /*继承widget*/
	p_font_t hFont;			 /*字体*/
	const char *text;		 /*显示的文字内容*/
	TextMode textMode;		 /*文字显示模式 0:左对齐 1:居中对齐 2:右对齐*/
} *p_text_widget_t, text_widget_t;

#define _PToHTextWidgeType(a) ((p_text_widget_t)a)

PUBLIC p_text_widget_t TEXT_MARK_HEAD(Create)(const char *text, int16_t x, int16_t y, uint16_t w, uint16_t h);
PUBLIC void TEXT_MARK_HEAD(Init)(p_text_widget_t hObject, const char *text, int16_t x, int16_t y, uint16_t w, uint16_t h);
/*根据模式得到文字绘制的起始位置*/
PUBLIC p_xpoint_t TEXT_MARK_HEAD(GetPOIByTextMode)(p_text_widget_t hTextWidge, p_xpoint_t startPoint);
PUBLIC void TEXT_MARK_HEAD(SetTextMode)(p_text_widget_t hOjbect, uint8_t textMode);
PUBLIC void TEXT_MARK_HEAD(SetFont)(p_text_widget_t hObject, p_font_t hFont);
PUBLIC void TEXT_MARK_HEAD(SetText)(p_text_widget_t hObject, const char *text);
PUBLIC void TEXT_MARK_HEAD(MoveTo)(p_text_widget_t hObject, int16_t x, int16_t y);
PUBLIC void TEXT_MARK_HEAD(Paint)(void *hObject);
PUBLIC int8_t TEXT_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg);

#endif
