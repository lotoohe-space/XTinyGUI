#ifndef _XWINDOWS_H__
#define _XWINDOWS_H__
#include "type.h"
#include "paint.h"
#include "tool.h"
#include "color.h"
#include "list.h"
#include "widge.h"
#include "widge_define.h"
#include "xwindows_head.h"

#define WINDOWS_MARK_HEAD(a) Windows##a

/*窗口结构体*/
typedef struct {
	//WIDGE_BASE winWidge;
	GROUP_WIDGE groupWidge;

	//HLIST  widgetList;/*控件列表*/
	HWIN_HEAD hWinHead;/*标题头控件*/
	/*临时偏移量，窗口在移动的时候，需要得到一个偏移量，用这个偏移量来偏移内部的每一个控件*/
	int16 t_dx;
	int16 t_dy;

	/*之前窗口的大小*/
	XRECT lastRect;
	/*临时存储*/
	int16 t_xy;
	/*windows处理回调函数*/
	WinProcessCallBack winProcessFun;

	uint8 flag;/*
			   0bit:是否绘制标题栏 (1表示绘制) 
			   1bit:窗口移动标志位，标志窗口是否正在移动
			   2bit:是否需要重新剪裁与绘制 
			   3bit:是否处于最小化
			   4bit:是否处于最大化
			   5bit:是否正在拖动窗口横向大小
			   6bit:是否正在拖动窗口纵向大小
			   7bit:是否正在拖动窗口左横向大小
			   */
}*HWIN,WIN;

//获取是否绘制win头部标志位
#define _IsDrawWinHead(a)  (_GET_BIT(((HWIN)(a))->flag,0))
//设置显示窗口头
#define _SetDrawWinHead(a) (_SET_BIT(((HWIN)(a))->flag,0))
//不显示头
#define _ClrDrawWinHead(a) (_CLR_BIT(((HWIN)(a))->flag,0))

//是否需要重新剪裁与绘制
#define _SetDrawAllLag(a)	(_SET_BIT(((HWIN)(a))->flag,2))
#define _ClrDrawAllLag(a)	(_CLR_BIT(((HWIN)(a))->flag,2))
#define _IsDrawAllLag(a)	(_GET_BIT(((HWIN)(a))->flag,2))

//设置是否处于最小化
#define _IsMinWIN(a)  (_GET_BIT(((HWIN)(a))->flag,3))
#define _SetMinWIN(a) (_SET_BIT(((HWIN)(a))->flag,3))
#define _ClrMinWIN(a) (_CLR_BIT(((HWIN)(a))->flag,3))

//设置是否处于最大化
#define _IsMaxWIN(a)  (_GET_BIT(((HWIN)(a))->flag,4))
#define _SetMaxWIN(a) (_SET_BIT(((HWIN)(a))->flag,4))
#define _ClrMaxWIN(a) (_CLR_BIT(((HWIN)(a))->flag,4))

/*是否正在拖动窗口横向大小*/
#define _IsChangeWinRHSize(a)  (_GET_BIT(((HWIN)(a))->flag,5))
#define _SetChangeWinRHSize(a) (_SET_BIT(((HWIN)(a))->flag,5))
#define _ClrChangeWinRHSize(a) (_CLR_BIT(((HWIN)(a))->flag,5))

#define _IsChangeWinVSize(a)  (_GET_BIT(((HWIN)(a))->flag,6))
#define _SetChangeWinVSize(a) (_SET_BIT(((HWIN)(a))->flag,6))
#define _ClrChangeWinVSize(a) (_CLR_BIT(((HWIN)(a))->flag,6))

#define _IsChangeWinLHSize(a)  (_GET_BIT(((HWIN)(a))->flag,7))
#define _SetChangeWinLHSize(a) (_SET_BIT(((HWIN)(a))->flag,7))
#define _ClrChangeWinLHSize(a) (_CLR_BIT(((HWIN)(a))->flag,7))

//设置窗口正在移动
#define _SetWinMoveing(a)  (_SET_BIT((a)->flag,1))
//清除窗口正在移动
#define _ClrWinMoveing(a)  (_CLR_BIT((a)->flag,1))
//获取是否在移动
#define _IsWinMoving(a)	   (_GET_BIT((a)->flag,1))

HWIN		WINDOWS_MARK_HEAD(Create)(char *title, int16 x, int16 y, int16 w, int16 h);
void		WINDOWS_MARK_HEAD(Close)(HWIN hWin);
void		WINDOWS_MARK_HEAD(SetProcessCallBack)(HWIN hObject, WinProcessCallBack winProcessFun);
void		WINDOWS_MARK_HEAD(SetMax)(HWIN hObject);
void		WINDOWS_MARK_HEAD(SetMin)(HWIN hObject);
HWIDGE_BASE WINDOWS_MARK_HEAD(GetWidge)(HWIN hObject, uint16 index);
void		WINDOWS_MARK_HEAD(Resize)(HWIN hObject, int16 x, int16 y, uint16 w, uint16 h);
void		WINDOWS_MARK_HEAD(SetVisable)(void* hObject, int8 isVisable);
void		WINDOWS_MARK_HEAD(Paint)(void *hObject);
void		WINDOWS_MARK_HEAD(MoveTo)(HWIN hWin, int16 x, int16 y);
void		WINDOWS_MARK_HEAD(SetColor)(HWIN hWin, uintColor color);
int8		WINDOWS_MARK_HEAD(Add)(HWIN hWin, void *widge);
int8		WINDOWS_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
void		WINDOWS_MARK_HEAD(SetDrawHead)(HWIN hWin, int8 isEnable);
void		WINDOWS_MARK_HEAD(InvaildRect)(HWIDGE_BASE hWidgeBase, HXRECT hXRect);
#endif
