#ifndef _GUI_CURSOR_H__
#define _GUI_CURSOR_H__
#include "type.h"
#include "bitmap.h"
#include "tool.h"
#include "widge_define.h"

typedef struct {
	const XBITMAP	GUICursorPTBitmap;
	uint8*			GUICursorMem;
	uint8*			GUICursorLastPOIMem;/*�ϴ�λ�õ�ͼƬ*/
	XPOINT			lastCursorPOI;/*֮ǰ�α��λ��*/
	XPOINT			cursorPOI;/*�α��λ��*/
	uint8			flag;/*0bit�Ƿ���ʾ 1bit�Ƿ�ر� 2bit�Ƿ��һ��ˢ�� 3bit�Ƿ���Ҫˢ�»���*/
}*HGUI_CURSOR, GUI_CURSOR;

/*�Ƿ���ʾ*/
#define _SET_CURSOR_SHOW(a)				_SET_BIT((((HGUI_CURSOR)(a))->flag),0)
#define _SET_CURSOR_HIDE(a)				_CLR_BIT((((HGUI_CURSOR)(a))->flag),0)
#define _GET_CURSOR_USE_STATUS(a)		_GET_BIT((((HGUI_CURSOR)(a))->flag),0)

/*�Ƿ�����ر�*/
#define _SET_CURSOR_OPEN(a)				_SET_BIT((((HGUI_CURSOR)(a))->flag),1)
#define _SET_CURSOR_CLOSE(a)			_CLR_BIT((((HGUI_CURSOR)(a))->flag),1)
#define _GET_CURSOR_STATUS(a)			_GET_BIT((((HGUI_CURSOR)(a))->flag),1)

/*��һ�α�־*/
#define _SET_CURSOR_FIRST(a)			_SET_BIT((((HGUI_CURSOR)(a))->flag),2)
#define _CLR_CURSOR_FIRST(a)			_CLR_BIT((((HGUI_CURSOR)(a))->flag),2)
#define _GET_CURSOR_FIRST(a)			_GET_BIT((((HGUI_CURSOR)(a))->flag),2)

/*�Ƿ���Ҫˢ��*/
#define _SET_CURSOR_UPT(a)				_SET_BIT((((HGUI_CURSOR)(a))->flag),3)
#define _CLR_CURSOR_UPT(a)				_CLR_BIT((((HGUI_CURSOR)(a))->flag),3)
#define _GET_CURSOR_UPT(a)				_GET_BIT((((HGUI_CURSOR)(a))->flag),3)

uint8	GUICursorInit(void);
void	GUICursorLastLCDCopyToMem();
uint8	GUICursorDrawLastMem(int16 x, int16 y, uintColor color);
BOOL	GUICursorSetPOI(int16 x, int16 y);
uint8	GUICursorDrawMem(int16 x, int16 y, uintColor color);
void	GUICursorDrawLCD(void);

#endif
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						