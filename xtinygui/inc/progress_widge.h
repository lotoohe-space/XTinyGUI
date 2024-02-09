#pragma once
#ifndef _PROGRESS_WIDGE_H__
#define _PROGRESS_WIDGE_H__
#include "widge.h"
#include "bitmap.h"
#include "xwindows.h"
#define PROGRESS_MARK_HEAD(a) ProgressWidge##a

typedef struct
{

	WIDGET_BASE widgeBase;

	uint16 maxVal;	   /*最大值*/
	uint16 currentVal; /*当前值*/

} *HPROGRESS_WIDGE, PROGRESS_WIDGE;

PUBLIC HPROGRESS_WIDGE PROGRESS_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, uint16 maxVal);
PUBLIC void PROGRESS_MARK_HEAD(SetParentWin)(HPROGRESS_WIDGE hObject, HWIN hWIN);
PUBLIC void PROGRESS_MARK_HEAD(SetColor)(HPROGRESS_WIDGE hObject, uintColor color);
PUBLIC void PROGRESS_MARK_HEAD(MoveTo)(HPROGRESS_WIDGE hObject, int16 x, int16 y);
PUBLIC void PROGRESS_MARK_HEAD(SetProgressVal)(HPROGRESS_WIDGE hObject, uint16 val);

PRIVATE void PROGRESS_MARK_HEAD(Paint)(void *hObject);
PRIVATE int8 PROGRESS_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
