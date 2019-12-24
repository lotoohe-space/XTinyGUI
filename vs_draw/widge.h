#ifndef _XWIDGE_H__
#define _XWIDGE_H__
#include "type.h"
#include "widge_define.h"

typedef struct {
	XRECT	rect;/*控件大小*/
	PENCIL	pencil;/*画笔，设置有效绘图大小，以及颜色*/
	PaintFunction paintFun;/*控件刷新函数*/
	WidgeMoveToFunction moveToFun;/*控件移动函数*/
	WidgeCallBackFunction widgeCallBackFun;/*控件事件回调函数*/

	int8 isVisable;/*是否显示*/
	void* parentHWIN;/*父控件，可以将控件加入*/
	
}*HWIDGE_BASE, WIDGE_BASE;

#define _PToHWidgeBaseType(a) ((HWIDGE_BASE)a)

HWIDGE_BASE WidgeCreate(int16 x, int16 y, int16 w, int16 h);

void WidgeSetParentWin(HWIDGE_BASE hObject, void* hWIN);
void WidgeSetColor(HWIDGE_BASE hObject, uintColor color);
void WidgePaint(void* hObject);
void WidgeMoveTo(HWIDGE_BASE hObject, int16 x, int16 y);
//void WidgeSetArea(void* hObject, int16 x, int16 y, int16 w, int16 h);
int8 WidgeCallBack(void *hObject, HMSGE hMsg);

#endif
