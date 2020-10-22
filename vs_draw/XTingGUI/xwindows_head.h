
#ifndef _WINDOWS_HEAD_H__
#define _WINDOWS_HEAD_H__
#include "type.h"
#include "widge_define.h"
#include "list.h"
#include "font.h"
#include "widge.h"
#include "button_widge.h"
#include "group_widge.h"
#define WINDOWS_HEAD_MARK_HEAD(a) WindowsHeadWidge##a

typedef struct {
	GROUP_WIDGE groupWidge;/*继承GROUP_WIDGE控件*/

	HXBUTTON hXButtonMin;/*最小化按钮，该按钮被添加到widgetList中*/
	HXBUTTON hXButtonMax;/*最大化按钮，该按钮被添加到widgetList中*/
	HXBUTTON hXButtonClose;/*最大化按钮，该按钮被添加到widgetList中*/

}*HWIN_HEAD, WIN_HEAD;

#define _DefaultHeadColor 0xc100
#define _DefaultFontColor 0xffff

//默认的窗口名
#define _DefaultWinHeadName "window"

PUBLIC HWIN_HEAD WINDOWS_HEAD_MARK_HEAD(Create)(char *title, int16 x, int16 y, int16 w, int16 h);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Close)(HWIN_HEAD hWinHead);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetVisable)(void* hObject, int8 isVisable);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMaximumBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetCloseBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(MoveTo)(HWIN_HEAD hObject, int16 x, int16 y);
PRIVATE void WINDOWS_HEAD_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

/*需要抽象出来*/
PUBLIC HWIDGET_BASE WINDOWS_HEAD_MARK_HEAD(GetWidge)(HWIN_HEAD hObject, uint16 index);
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Resize)(HWIN_HEAD hObject, int16 x, int16 y, uint16 w, uint16 h);

PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetArea)(HWIN_HEAD hObject, int16 x, int16 y, int16 w, int16 h);
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(Add)(HWIN_HEAD hWinHead, void* widge);
#endif

