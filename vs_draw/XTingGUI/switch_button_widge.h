
#ifndef _SWITCH_BUTTON_WIDGE_H__
#define _SWITCH_BUTTON_WIDGE_H__

//#include "widge.h"
#include "bitmap.h"
//#include "xwindows.h"
#include "text_widge.h"
#define SWITCH_BUTTON_WIDGE_MARK_HEAD(a) SwitchButtonWidge##a

typedef struct {
	TEXT_WIDGE textWidge;

	const char* offText;
	const char* onText;
	HXBITMAP offBitmap;/*关闭的图片*/
	HXBITMAP onBitmap;/*打开的图片*/

	uint8 flag;/*0，1bit：当前状态 2:bit 文字模式/图片模式 */

}*HSWITCH_BUTTON_WIDGE, SWITCH_BUTTON_WIDGE;

/*控件是否需要进行透明处理*/
#define _SET_SW_TEXT_MODE(a)		_CLR_BIT((((HSWITCH_BUTTON_WIDGE)(a))->flag),2)
#define _SET_SW_BMP_MODE(a)			_SET_BIT((((HSWITCH_BUTTON_WIDGE)(a))->flag),2)
#define _GET_SW_MODE(a)				_GET_BIT((((HSWITCH_BUTTON_WIDGE)(a))->flag),2)

PUBLIC HSWITCH_BUTTON_WIDGE	SWITCH_BUTTON_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h, const char* onText, const char* offText);
PUBLIC	void				SWITCH_BUTTON_WIDGE_MARK_HEAD(MoveTo)(HSWITCH_BUTTON_WIDGE hObject, int16 x, int16 y);
PUBLIC	void				SWITCH_BUTTON_WIDGE_MARK_HEAD(SetParentWin)(HSWITCH_BUTTON_WIDGE hObject, HWIDGET_BASE parentWidge);
PUBLIC	void				SWITCH_BUTTON_WIDGE_MARK_HEAD(SetColor)(HSWITCH_BUTTON_WIDGE hObject, uintColor color);
PUBLIC	void				SWITCH_BUTTON_WIDGE_MARK_HEAD(Paint)(void* hObject);
PUBLIC	int8				SWITCH_BUTTON_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
PUBLIC	void				SWITCH_BUTTON_WIDGE_MARK_HEAD(SetClickBack)(HSWITCH_BUTTON_WIDGE hObject, void* arg, ViewClickCallBack viewClickCallBack);


PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOnText)(HSWITCH_BUTTON_WIDGE hObject, const char* text);
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOffText)(HSWITCH_BUTTON_WIDGE hObject, const char* text);
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOnBitmap)(HSWITCH_BUTTON_WIDGE hObject, HXBITMAP hBitmap);
PUBLIC void SWITCH_BUTTON_WIDGE_MARK_HEAD(SetOffBitmap)(HSWITCH_BUTTON_WIDGE hObject, HXBITMAP hBitmap);

#endif
