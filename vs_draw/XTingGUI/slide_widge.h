#ifndef _SLIDE_WIDGE_H__
#define _SLIDE_WIDGE_H__
#include "widge.h"

#define SLIDE_MARK_HEAD(a) SlideWidge##a

/*按钮的管理结构体*/
typedef struct {
	WIDGE_BASE	baseWidge;	/*继承WIDGE_BASE控件*/

	int16		maxVal;		/*最大值*/
	uint16		currentVal;	/*当前值*/

}*HSLIDE_WIDGE, SLIDE_WIDGE;

PUBLIC HSLIDE_WIDGE SLIDE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h);
PUBLIC void SLIDE_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 SLIDE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

#endif
