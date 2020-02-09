
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"

/*是否开启透明通道,开启透明通道会降低运行速度*/
#define USE_ALPHA		0
/*显示屏的大小*/
#define LCD_SCREEN_W	800
#define LCD_SCREEN_H	480

/*********返回的事件说明********/
#define RES_ASSERT_ERR		-1
#define RES_OK				0
#define RES_FAIL			1
#define RES_USED			10
/*******************************/

typedef struct {
	/*桌面也是一个窗口，不过没有窗头控件*/
	HWIN desktopWin;
	/*绘图区域*/
	XRECT drawArea;
	/*最顶上的窗体*/
	HWIN topWin;
	/*正在移动的控件*/
	XRECT movingRect;
}*HXDESKTOP, XDESKTOP;
extern HXDESKTOP hXDesktop;

/*当前控件坐标转换为相对坐标*/
/*即当前坐标相对于父控件的偏移*/
#define CDE2OPPOSITE_X(a)		(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->x - ((HXRECT)(a))->x)
#define CDE2OPPOSITE_Y(a)		(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->y - ((HXRECT)(a))->y)

/*得到当前坐标相对于控件坐标的相对坐标*/
#define P_CDE2OPPOSITE_X(a,b)	((b) - ((HXRECT)((HWIDGE_BASE)(a)))->x)
#define P_CDE2OPPOSITE_Y(a,b)	((b) - ((HXRECT)((HWIDGE_BASE)(a)))->y)

/*相对坐标转换为绝对坐标*/
#define _OPPOSITE2CDE_X(a,b)	(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->x - (b))
#define _OPPOSITE2CDE_Y(a,b)	(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->y - (b))

/*得到控件的坐标与大小*/
#define WIDGE_X(a)				(((HXRECT)(a))->x)
#define WIDGE_Y(a)				(((HXRECT)(a))->y)
#define WIDGE_W(a)				(((HXRECT)(a))->w)
#define WIDGE_H(a)				(((HXRECT)(a))->h)

/*获得控件的父控件*/
#define WIDGE_PARENT(a)			((HWIDGE_BASE)(((HWIDGE_BASE)(a))->parentHWIN))

void GUIExec(void);
int8 WinListAdd(HWIDGE_BASE hWidgeBase);
HWIDGE_BASE WinGetTop(void);
HXDESKTOP GUIInit(void);
void GUIEvent(void);
void WinMoveTop(void* hObject);
BOOL IsGUINeedCut(HWIDGE_BASE hWidgeBase);
BOOL IsGUINeedCutEx(HWIN hWin);
void SetMovingRect(HXRECT hXRect);

/*设置绘图区域*/
uint8 DrawSetArea(HWIDGE_BASE hWidgeBase);
/*取消设置*/
void DrawResetArea(HWIDGE_BASE hWidgeBase);

#endif

