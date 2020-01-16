
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"

#define LCD_SCREEN_W 800
#define LCD_SCREEN_H 480

typedef struct {
	/*桌面也是一个窗口，不过没有窗头控件*/
	HWIN desktopWin;

	/*绘图区域*/
	XRECT drawArea;

	/*最顶上的窗体*/
	HWIN topWin;

	/*正在移动的窗口*/
	XRECT movingWidge;

	
	//XRECT desktopRect;
	//PENCIL pencil;//画笔
	//XRECT invaildRect;
	/*HLIST winList;*/
	
}*HXDESKTOP, XDESKTOP;
extern HXDESKTOP hXDesktop;
//extern  XRECT drawArea;//绘制区域


//void GUIInvaildRect(HXRECT hXRect);
void GUIExec(void);
int8 WinListAdd(HWIN hWin);
HWIN WinGetTop(void);
HWIN WinGetFocus(void);
HXDESKTOP GUIInit(void);
void GUIEvent(void);
void WinMoveTop(void* hObject);
//void GUIPostEvent(HMSGE hMsg);
BOOL isGUINeedCut(HXRECT hXRECT);
void SetMovingWin(HXRECT hXRect);



#endif

