
#ifndef _WINDOWS_HEAD_H__
#define _WINDOWS_HEAD_H__
#include "type.h"
#include "widge_define.h"
#include "list.h"
#include "font.h"
#include "widge.h"
#include "button_widge.h"

#define WINDOWS_HEAD_MARK_HEAD(a) WindowsHeadWidge##a

typedef struct {
	WIDGE_BASE widgeBase;

	HLIST	widgetList;//控件列表
	HXBUTTON hXButtonMin;/*最小化按钮，该按钮被添加到widgetList中*/
	HXBUTTON hXButtonMax;/*最大化按钮，该按钮被添加到widgetList中*/
	char	*title;/*窗口的text*/

}*HWIN_HEAD, WIN_HEAD;

#define _DefaultHeadColor 0xc100
#define _DefaultFontColor 0xffff

//默认的窗口名
#define _DefaultWinHeadName "window"

PUBLIC HWIN_HEAD WINDOWS_HEAD_MARK_HEAD(Create)(char *title, int16 x, int16 y, int16 w, int16 h);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Close)(HWIN_HEAD hWinHead);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetVisable)(void* hObject, int8 isVisable);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(MoveTo)(HWIN_HEAD hObject, int16 x, int16 y);
PRIVATE void WINDOWS_HEAD_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetArea)(HWIN_HEAD hObject, int16 x, int16 y, int16 w, int16 h);
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(Add)(HWIN_HEAD hWinHead, void* widge);
#endif

