
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

	HLIST	widgetList;//�ؼ��б�
	HXBUTTON hXButtonMin;/*��С����ť���ð�ť����ӵ�widgetList��*/
	HXBUTTON hXButtonMax;/*��󻯰�ť���ð�ť����ӵ�widgetList��*/
	char	*title;/*���ڵ�text*/

}*HWIN_HEAD, WIN_HEAD;

#define _DefaultHeadColor 0xc100
#define _DefaultFontColor 0xffff

//Ĭ�ϵĴ�����
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

