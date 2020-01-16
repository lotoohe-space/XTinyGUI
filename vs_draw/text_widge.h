
#ifndef _TEXT_WIDGE_H__
#define _TEXT_WIDGE_H__

#include "type.h"
#include "paint.h"
#include "widge.h"
typedef struct {
	WIDGE_BASE textWidge;


	HFONTF hFont;//����
	char *title;

}*HTEXT_WIDGE, TEXT_WIDGE;


HTEXT_WIDGE TextWidegeCreate(char *text, int16 x, int16 y);
void		TextSetText(HTEXT_WIDGE hObject, const char* title);
void		TextWidegeMoveTo(HTEXT_WIDGE hObject, int16 x, int16 y);
void		TextWidegePaint(void * hObject);
int8		TextWidegeCallBack(void *hObject, HMSGE hMsg);

#endif


