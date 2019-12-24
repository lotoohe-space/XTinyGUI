
#ifndef _WINDOWS_HEAD_H__
#define _WINDOWS_HEAD_H__
#include "type.h"
#include "widge_define.h"
#include "list.h"
#include "font.h"
#include "widge.h"
typedef struct {
	WIDGE_BASE headWidge;

	HLIST	widgetList;//控件列表
	HFONTF hFont;//字体
	char *title;
}*HWIN_HEAD, WIN_HEAD;

#define _DefaultHeadColor 0xc100
#define _DefaultFontColor 0xffff

//默认的窗口名
#define _DefaultWinHeadName "window"

HWIN_HEAD WindowsHeadCreate(char *title, int16 x, int16 y, int16 w, int16 h);
void WindowsHeadPaint(void *hObject);
void WindowsHeadMoveTo(HWIN_HEAD hObject, int16 x, int16 y);
int8 WindowsHeadsCallBack(void *hObject, HMSGE hMsg);
void WindowsHeadSetArea(HWIN_HEAD hObject, int16 x, int16 y, int16 w, int16 h);
int8 WindowsHeadAdd(HWIN_HEAD hWinHead, void *widge);
void WindowsHeadSetVisable(void* hObject, int8 isVisable);

#endif

