#ifndef _GUI_CURSOR_H__
#define _GUI_CURSOR_H__
#include "type.h"
#include "bitmap.h"
#include "tool.h"
#include "widge_define.h"

typedef struct
{
	const XBITMAP GUICursorPTBitmap;
	uint8 *GUICursorMem;
	uint8 *GUICursorLastPOIMem; /*上次位置的图片*/
	XPOINT lastCursorPOI;		/*之前游标的位置*/
	XPOINT cursorPOI;			/*游标的位置*/
	uint8 flag;					/*0bit是否显示 1bit是否关闭 2bit是否第一次刷新 3bit是否需要刷新缓存*/
} *HGUI_CURSOR, GUI_CURSOR;

/*是否显示*/
#define _SET_CURSOR_SHOW(a) _SET_BIT((((HGUI_CURSOR)(a))->flag), 0)
#define _SET_CURSOR_HIDE(a) _CLR_BIT((((HGUI_CURSOR)(a))->flag), 0)
#define _GET_CURSOR_USE_STATUS(a) _GET_BIT((((HGUI_CURSOR)(a))->flag), 0)

/*是否开启与关闭*/
#define _SET_CURSOR_OPEN(a) _SET_BIT((((HGUI_CURSOR)(a))->flag), 1)
#define _SET_CURSOR_CLOSE(a) _CLR_BIT((((HGUI_CURSOR)(a))->flag), 1)
#define _GET_CURSOR_STATUS(a) _GET_BIT((((HGUI_CURSOR)(a))->flag), 1)

/*第一次标志*/
#define _SET_CURSOR_FIRST(a) _SET_BIT((((HGUI_CURSOR)(a))->flag), 2)
#define _CLR_CURSOR_FIRST(a) _CLR_BIT((((HGUI_CURSOR)(a))->flag), 2)
#define _GET_CURSOR_FIRST(a) _GET_BIT((((HGUI_CURSOR)(a))->flag), 2)

/*是否需要刷新*/
#define _SET_CURSOR_UPT(a) _SET_BIT((((HGUI_CURSOR)(a))->flag), 3)
#define _CLR_CURSOR_UPT(a) _CLR_BIT((((HGUI_CURSOR)(a))->flag), 3)
#define _GET_CURSOR_UPT(a) _GET_BIT((((HGUI_CURSOR)(a))->flag), 3)

uint8 GUICursorInit(void);
void GUICursorLastLCDCopyToMem();
uint8 GUICursorDrawLastMem(int16 x, int16 y, uintColor color);
BOOL GUICursorSetPOI(int16 x, int16 y);
uint8 GUICursorDrawMem(int16 x, int16 y, uintColor color);
void GUICursorDrawLCD(void);

#endif
