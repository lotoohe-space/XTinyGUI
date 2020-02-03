#ifndef _WIDGE_CALL_DEFINE_H__
#define _WIDGE_CALL_DEFINE_H__

#include "msg.h"
//画笔的定义
typedef struct {
	/*允许被绘制的区域,全0XFFFF表示无效区域*/
	int16 x;
	int16 y;
	uint16 w;
	uint16 h;

	uintColor DrawColor;/*画笔颜色*/
	uintColor DrawFrColor;/*前景色*/
	uintColor DrawBkColor;/*背景色*/
}*HPENCIL, PENCIL;

/*矩形*/
typedef struct {
	int16 x;
	int16 y;
	int16 w;
	int16 h;
}*HXRECT, XRECT;

/*点*/
typedef struct {
	int16 x;
	int16 y;
}*HXPOINT, XPOINT;

/*横线*/
typedef struct {
	int16 x0;
	int16 y;
	int16 x1;
}*HXHLINE, XHLINE;



/*变量强转为矩形*/
#define _PToHXRECTType(a) ((HXRECT)(a))

/*刷新函数*/
typedef void (*PaintFunction)(void* hObject);
/*移动函数*/
typedef void (*WidgeMoveToFunction)(void* hObject, int16 x, int16 y);
/*事件回调函数*/
typedef int8(*WidgeCallBackFunction)(void* hObject, HMSGE hMsg);
/*关闭函数*/
typedef void (*WidgeCloseFunction)(void* hObject);
/*重设大小函数*/
typedef void (*WidgeResizeFunction)(void* hOjbect,int16 x,int16 y,uint16 w,uint16 h);

/*窗口事件回调*/
typedef uint8(*WinProcessCallBack)(void* Object, void* arg, HMSGE hMsg);
/*按钮事件回调*/
typedef void (*ViewClickCallBack)(void* Object, void* arg, uint8 status);

#endif