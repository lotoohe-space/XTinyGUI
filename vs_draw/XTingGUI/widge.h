#ifndef _XWIDGE_H__
#define _XWIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "tool.h"

#define WIDGE_MARK_HEAD(a) Widge##a

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
			   */

}*HWIDGE_BASE, WIDGE_BASE;

/*控件是否需要进行透明处理*/
#define _SET_IS_DPY(a)			_SET_BIT((((HWIDGE_BASE)(a))->flag),4)
#define _CLR_IS_DPY(a)			_CLR_BIT((((HWIDGE_BASE)(a))->flag),4)
#define _GET_IS_DPY(a)			_GET_BIT((((HWIDGE_BASE)(a))->flag),4)

/*该widge是不是窗口*/
#define _SET_IS_WIN(a)			_SET_BIT((((HWIDGE_BASE)(a))->flag),0)
#define _CLR_IS_WIN(a)			_CLR_BIT((((HWIDGE_BASE)(a))->flag),0)
#define _GET_IS_WIN(a)			_GET_BIT((((HWIDGE_BASE)(a))->flag),0)

/*是否被点击*/
#define _SetBtnPress(a)			_SET_BIT(((HWIDGE_BASE)(a))->flag,1)
#define _SetBtnRelease(a)		_CLR_BIT(((HWIDGE_BASE)(a))->flag,1)
#define _GetBtnStatus(a)		_GET_BIT(((HWIDGE_BASE)(a))->flag,1)

/*是否显示*/
#define _SetVisable(a)			_SET_BIT(((HWIDGE_BASE)(a))->flag,2)
#define _ClrVisable(a)			_CLR_BIT(((HWIDGE_BASE)(a))->flag,2)
#define _GetVisable(a)			_GET_BIT(((HWIDGE_BASE)(a))->flag,2)

#define _PToHWidgeBaseType(a) ((HWIDGE_BASE)a)

PUBLIC	HWIDGE_BASE	WIDGE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h);
PUBLIC	HWIDGE_BASE	WIDGE_MARK_HEAD(CreateEx)(HXRECT hXRect);
PUBLIC	void		WIDGE_MARK_HEAD(Close)(HWIDGE_BASE hObject);
PUBLIC	void		WIDGE_MARK_HEAD(Init)(HWIDGE_BASE hWidgeBase, int16 x, int16 y, int16 w, int16 h);
PUBLIC	void		WIDGE_MARK_HEAD(SetClickBack)(HWIDGE_BASE hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC	void		WIDGE_MARK_HEAD(SetVisable)(HWIDGE_BASE hObject, uint8 isVisable);
PUBLIC	void		WIDGE_MARK_HEAD(Resize)(HWIDGE_BASE hObject, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC	void		WIDGE_MARK_HEAD(SetParentWin)(HWIDGE_BASE hObject, void* hWIN);
PUBLIC	void		WIDGE_MARK_HEAD(SetColor)(HWIDGE_BASE hObject, uintColor color);
PUBLIC	void		WIDGE_MARK_HEAD(MoveTo)(HWIDGE_BASE hObject, int16 x, int16 y);
PRIVATE void		WIDGE_MARK_HEAD(Paint)(void* hObject);
PUBLIC	int8		WIDGE_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
