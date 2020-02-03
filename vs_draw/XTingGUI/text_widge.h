
#ifndef _TEXT_WIDGE_H__
#define _TEXT_WIDGE_H__

#include "type.h"
#include "paint.h"
#include "widge.h"

#define TEXT_MARK_HEAD(a) TextWidge##a

/*文字的显示模式*/
typedef enum {
	TEXT_TOP_LEFT_ALIGN = 0,/*顶部左对齐*/
	TEXT_TOP_CENTER_ALIGN,/*顶部居中对齐*/
	TEXT_TOP_RIGHT_ALIGN,/*顶部右对齐*/
	TEXT_BOTTOM_LEFT_ALIGN,/*底部左边对齐*/
	TEXT_BOTTOM_CENTER_ALIGN,/*底部居中对齐*/
	TEXT_BOTTOM_RIGHT_ALIGN,/*底部右对齐*/
	TEXT_LEFT_CENTER_ALIGN,/*居中左对齐*/
	TEXT_RIGHT_CENTER_ALIGN,/*居中右对齐*/
	TEXT_CENTER_ALIGN/*居中对齐*/
}TextMode;

typedef struct {
	WIDGE_BASE baseWidge;

	HFONTF hFont;			/*字体*/
	char *text;

	TextMode textMode;			/*文字显示模式 0:左对齐 1:居中对齐 2:右对齐*/

}*HTEXT_WIDGE, TEXT_WIDGE;

#define _PToHTextWidgeType(a) ((HTEXT_WIDGE)a)

PUBLIC HTEXT_WIDGE	TEXT_MARK_HEAD(Create)(char* text, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC void			TEXT_MARK_HEAD(Init)(HTEXT_WIDGE hObject, char* text, int16 x, int16 y, uint16 w, uint16 h);
/*根据模式得到文字绘制的起始位置*/
PUBLIC HXPOINT		TEXT_MARK_HEAD(GetPOIByTextMode)(HTEXT_WIDGE hTextWidge, HXPOINT startPoint);
PUBLIC void			TEXT_MARK_HEAD(SetTextMode)(HTEXT_WIDGE hOjbect, uint8 textMode);
PUBLIC void			TEXT_MARK_HEAD(SetFont)(HTEXT_WIDGE hObject, HFONTF hFont);
PUBLIC void			TEXT_MARK_HEAD(SetText)(HTEXT_WIDGE hObject, const char* text);
PUBLIC void			TEXT_MARK_HEAD(MoveTo)(HTEXT_WIDGE hObject, int16 x, int16 y);
PUBLIC void			TEXT_MARK_HEAD(Paint)(void * hObject);
PUBLIC int8			TEXT_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif


