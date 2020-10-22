#ifndef _SLIDE_WIDGE_H__
#define _SLIDE_WIDGE_H__
#include "widge.h"

#define SLIDE_MARK_HEAD(a) SlideWidge##a

/*��ť�Ĺ���ṹ��*/
typedef struct {
	WIDGET_BASE	baseWidge;	/*�̳�WIDGE_BASE�ؼ�*/

	int16		maxVal;		/*���ֵ*/
	uint16		currentVal;	/*��ǰֵ*/

}*HSLIDE_WIDGE, SLIDE_WIDGE;

PUBLIC HSLIDE_WIDGE SLIDE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h);
PUBLIC void SLIDE_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 SLIDE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

#endif
