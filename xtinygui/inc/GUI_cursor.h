#ifndef _GUI_CURSOR_H__
#define _GUI_CURSOR_H__
#include "type.h"
#include "bitmap.h"
#include "x_tool.h"
#include "widge_define.h"

typedef struct
{
	const xbitmap_t GUICursorPTBitmap;
	uint8_t *GUICursorMem;
	uint8_t *GUICursorLastPOIMem; /*上次位置的图片*/
	xpoint_t lastCursorPOI;		/*之前游标的位置*/
	xpoint_t cursorPOI;			/*游标的位置*/
	uint8_t flag;					/*0bit是否显示 1bit是否关闭 2bit是否第一次刷新 3bit是否需要刷新缓存*/
} *p_gui_cursor_t, gui_cursor_t;

/*是否显示*/
#define _SET_CURSOR_SHOW(a) _SET_BIT((((p_gui_cursor_t)(a))->flag), 0)
#define _SET_CURSOR_HIDE(a) _CLR_BIT((((p_gui_cursor_t)(a))->flag), 0)
#define _GET_CURSOR_USE_STATUS(a) _GET_BIT((((p_gui_cursor_t)(a))->flag), 0)

/*是否开启与关闭*/
#define _SET_CURSOR_OPEN(a) _SET_BIT((((p_gui_cursor_t)(a))->flag), 1)
#define _SET_CURSOR_CLOSE(a) _CLR_BIT((((p_gui_cursor_t)(a))->flag), 1)
#define _GET_CURSOR_STATUS(a) _GET_BIT((((p_gui_cursor_t)(a))->flag), 1)

/*第一次标志*/
#define _SET_CURSOR_FIRST(a) _SET_BIT((((p_gui_cursor_t)(a))->flag), 2)
#define _CLR_CURSOR_FIRST(a) _CLR_BIT((((p_gui_cursor_t)(a))->flag), 2)
#define _GET_CURSOR_FIRST(a) _GET_BIT((((p_gui_cursor_t)(a))->flag), 2)

/*是否需要刷新*/
#define _SET_CURSOR_UPT(a) _SET_BIT((((p_gui_cursor_t)(a))->flag), 3)
#define _CLR_CURSOR_UPT(a) _CLR_BIT((((p_gui_cursor_t)(a))->flag), 3)
#define _GET_CURSOR_UPT(a) _GET_BIT((((p_gui_cursor_t)(a))->flag), 3)

uint8_t GUICursorInit(void);
void GUICursorLastLCDCopyToMem();
uint8_t GUICursorDrawLastMem(int16_t x, int16_t y, uintColor color);
BOOL GUICursorSetPOI(int16_t x, int16_t y);
uint8_t GUICursorDrawMem(int16_t x, int16_t y, uintColor color);
void GUICursorDrawLCD(void);

#endif
