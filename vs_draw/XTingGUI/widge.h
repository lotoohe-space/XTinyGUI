#ifndef _XWIDGE_H__
#define _XWIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "tool.h"

#define _WIDGET(a) Widget##a

typedef struct {
	XRECT	rect;/*控件大小*/
	PENCIL	pencil;/*画笔，设置有效绘图大小，以及颜色*/

	/*内部调用使用*/
	PaintFunction paintFun;/*控件刷新函数*/
	WidgeMoveToFunction moveToFun;/*控件移动函数*/
	WidgeCallBackFunction widgeCallBackFun;/*控件事件回调函数*/
	WidgeCloseFunction widgeCloseFun;/*控件关闭函数*/
	WidgeResizeFunction widgeResizeFun;/*控件重设大小函数*/

	/*外部调用，点击事件回调函数，所有控件共有的特性*/
	ViewClickCallBack viewClickCallBack;
	/*可以传递一个参数*/
	void* arg;

	void* parentHWIN;/*父控件*/
	uint8 flag;/*
			   0:bit 是否为窗口 
			   1:bit 是否被点击 
			   2:bit 是否显示 
			   3:bit 是否有焦点(还未实现)
			   4:bit 是否需要进行透明处理
			   5:bit 是否(在group_widge中需要特殊处理)
			   */

}*HWIDGET_BASE, WIDGET_BASE;

/*控件是否需要开关联动*/
#define _SET_GROUP_CTRL(a)		_SET_BIT((((HWIDGET_BASE)(a))->flag),5)
#define _CLR_GROUP_CTRL(a)		_CLR_BIT((((HWIDGET_BASE)(a))->flag),5)
#define _GET_GROUP_CTRL(a)		_GET_BIT((((HWIDGET_BASE)(a))->flag),5)

/*控件是否需要进行透明处理*/
#define _SET_IS_DPY(a)			_SET_BIT((((HWIDGET_BASE)(a))->flag),4)
#define _CLR_IS_DPY(a)			_CLR_BIT((((HWIDGET_BASE)(a))->flag),4)
#define _GET_IS_DPY(a)			_GET_BIT((((HWIDGET_BASE)(a))->flag),4)

/*该widge是不是窗口*/
#define _SET_IS_WIN(a)			_SET_BIT((((HWIDGET_BASE)(a))->flag),0)
#define _CLR_IS_WIN(a)			_CLR_BIT((((HWIDGET_BASE)(a))->flag),0)
#define _GET_IS_WIN(a)			_GET_BIT((((HWIDGET_BASE)(a))->flag),0)

/*是否被点击*/
#define _SetBtnPress(a)			_SET_BIT(((HWIDGET_BASE)(a))->flag,1)
#define _SetBtnRelease(a)		_CLR_BIT(((HWIDGET_BASE)(a))->flag,1)
#define _GetBtnStatus(a)		_GET_BIT(((HWIDGET_BASE)(a))->flag,1)

/*是否显示*/
#define _SetVisable(a)			_SET_BIT(((HWIDGET_BASE)(a))->flag,2)
#define _ClrVisable(a)			_CLR_BIT(((HWIDGET_BASE)(a))->flag,2)
#define _GetVisable(a)			_GET_BIT(((HWIDGET_BASE)(a))->flag,2)

#define _PToHWidgeBaseType(a) ((HWIDGET_BASE)a)	/*转换为HWIDGE_BASE对象*/

PUBLIC	HWIDGET_BASE	_WIDGET(Create)(int16 x, int16 y, int16 w, int16 h);
PUBLIC	HWIDGET_BASE	_WIDGET(CreateEx)(HXRECT hXRect);
PUBLIC	void			_WIDGET(Close)(HWIDGET_BASE hObject);
PUBLIC	void			_WIDGET(Init)(HWIDGET_BASE hWidgeBase, int16 x, int16 y, int16 w, int16 h);
PUBLIC	void			_WIDGET(SetClickBack)(HWIDGET_BASE hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC	void			_WIDGET(SetVisable)(HWIDGET_BASE hObject, uint8 isVisable);
PUBLIC	void			_WIDGET(Resize)(HWIDGET_BASE hObject, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC	void			_WIDGET(SetParentWin)(HWIDGET_BASE hObject, void* hWIN);
PUBLIC	void			_WIDGET(SetColor)(HWIDGET_BASE hObject, uintColor color);
PUBLIC	void			_WIDGET(MoveTo)(HWIDGET_BASE hObject, int16 x, int16 y);
PRIVATE void			_WIDGET(Paint)(void* hObject);
PUBLIC	int8			_WIDGET(CallBack)(void *hObject, HMSGE hMsg);

#endif
