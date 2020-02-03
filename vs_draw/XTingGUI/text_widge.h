
#ifndef _TEXT_WIDGE_H__
#define _TEXT_WIDGE_H__

#include "type.h"
#include "paint.h"
#include "widge.h"

#define TEXT_MARK_HEAD(a) TextWidge##a

/*���ֵ���ʾģʽ*/
typedef enum {
	TEXT_TOP_LEFT_ALIGN = 0,/*���������*/
	TEXT_TOP_CENTER_ALIGN,/*�������ж���*/
	TEXT_TOP_RIGHT_ALIGN,/*�����Ҷ���*/
	TEXT_BOTTOM_LEFT_ALIGN,/*�ײ���߶���*/
	TEXT_BOTTOM_CENTER_ALIGN,/*�ײ����ж���*/
	TEXT_BOTTOM_RIGHT_ALIGN,/*�ײ��Ҷ���*/
	TEXT_LEFT_CENTER_ALIGN,/*���������*/
	TEXT_RIGHT_CENTER_ALIGN,/*�����Ҷ���*/
	TEXT_CENTER_ALIGN/*���ж���*/
}TextMode;

typedef struct {
	WIDGE_BASE baseWidge;

	HFONTF hFont;			/*����*/
	char *text;

	TextMode textMode;			/*������ʾģʽ 0:����� 1:���ж��� 2:�Ҷ���*/

}*HTEXT_WIDGE, TEXT_WIDGE;

#define _PToHTextWidgeType(a) ((HTEXT_WIDGE)a)

PUBLIC HTEXT_WIDGE	TEXT_MARK_HEAD(Create)(char* text, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC void			TEXT_MARK_HEAD(Init)(HTEXT_WIDGE hObject, char* text, int16 x, int16 y, uint16 w, uint16 h);
/*����ģʽ�õ����ֻ��Ƶ���ʼλ��*/
PUBLIC HXPOINT		TEXT_MARK_HEAD(GetPOIByTextMode)(HTEXT_WIDGE hTextWidge, HXPOINT startPoint);
PUBLIC void			TEXT_MARK_HEAD(SetTextMode)(HTEXT_WIDGE hOjbect, uint8 textMode);
PUBLIC void			TEXT_MARK_HEAD(SetFont)(HTEXT_WIDGE hObject, HFONTF hFont);
PUBLIC void			TEXT_MARK_HEAD(SetText)(HTEXT_WIDGE hObject, const char* text);
PUBLIC void			TEXT_MARK_HEAD(MoveTo)(HTEXT_WIDGE hObject, int16 x, int16 y);
PUBLIC void			TEXT_MARK_HEAD(Paint)(void * hObject);
PUBLIC int8			TEXT_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif


