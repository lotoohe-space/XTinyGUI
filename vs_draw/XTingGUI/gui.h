
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"
#include "mem_dev.h"

/*是否开启透明通道,开启透明通道会降低运行速度*/
#define USE_ALPHA			0
#define USE_MEM_DEV			1
/*显示屏的大小*/
#define LCD_SCREEN_W		800
#define LCD_SCREEN_H		480

/*********返回的事件说明********/
#define RES_ASSERT_ERR		-1
#define RES_OK				0
#define RES_FAIL			1
#define RES_USED			10
/*******************************/

typedef struct {
	HWIN desktopWin;/*继承HWIN 桌面也是一个窗口，不过没有窗头控件*/
	/*最顶上的窗体*/
	HWIN topWin;
	/*需要更新的区域*/
	XRECT updateArea;
	/*更新区域的父控件*/
	HWIDGE_BASE updateParentWidge;

#if USE_MEM_DEV
	/*存储设备*/
	HMEM_DEV	hMemDev;
#endif

}*HXDESKTOP, XDESKTOP;
extern HXDESKTOP hXDesktop;

/*获取一个控件的绘图区域*/
#define DESKTOP_DRAW_AREA(a)	((HXRECT)((&((a)->desktopWin->groupWidge.widgeBase.pencil))))

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

HXDESKTOP	GUIInit(void);
void		GUIUpdate(HWIDGE_BASE hWidgeBase, HXRECT hXRect);
void		GUIExec(void);
void		GUIEvent(void);
uint32		GUIGetTick(void);
int8		WinListAdd(HWIDGE_BASE hWidgeBase);
HWIDGE_BASE WinGetTop(void);
void		WinMoveTop(void* hObject);

BOOL		IsGUINeedCut(HWIDGE_BASE hWidgeBase);
void		SetUpdateArea(HWIDGE_BASE hWidgeBase, HXRECT hXRect);

uint8		DrawSetArea(HWIDGE_BASE hWidgeBase);/*设置绘图区域*/
void		DrawResetArea(HWIDGE_BASE hWidgeBase);/*取消设置*/

#endif

