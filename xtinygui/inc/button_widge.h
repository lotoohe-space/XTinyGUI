#ifndef _BUTTON_WIDGE_H__
#define _BUTTON_WIDGE_H__
#include "x_types.h"
#include "widget_define.h"
#include "font.h"
#include "text_widge.h"

#define BUTTON_MARK_HEAD(a) ButtonWidge##a

/*按钮的管理结构体*/
typedef struct
{
	text_widget_t textWidge; /*继承文本控件*/

	uint8_t flag; /*0:bit 文字模式/图片模式 */

	p_xbitmap_t downBitmap; /*按下图片*/
	p_xbitmap_t upBitmap;	/*松开图片*/

	uintColor downColor; /*按下颜色*/
	uintColor upColor;	 /*松开颜色*/

} *p_xbutton_t, xbutton_t;

/*button模式*/
#define _SetTextMode(a) (_CLR_BIT(((p_xbutton_t)(a))->flag, 0))
#define _SetBMPMode(a) (_SET_BIT(((p_xbutton_t)(a))->flag, 0))
#define _GetBtnMode(a) (_GET_BIT(((p_xbutton_t)(a))->flag, 0))

/*按下与松开颜色*/
#define _DefaultButtonDownColor RGB565_GEN(0xff, 0xff, 0xff)
#define _DefaultButtonUpColor RGB565_GEN(200, 200, 200)
/*字体颜色*/
#define _DefaultButtonFontColor 0x0000

PUBLIC p_xbutton_t BUTTON_MARK_HEAD(Create)(const char *text, int16_t x, int16_t y, int16_t w, int16_t h);
PUBLIC void BUTTON_MARK_HEAD(MoveTo)(p_xbutton_t hObject, int16_t x, int16_t y);
PUBLIC void BUTTON_MARK_HEAD(SetClickBack)(p_xbutton_t hObject, void *arg, ViewClickCallBack viewClickCallBack);

PUBLIC void BUTTON_MARK_HEAD(SetDownBitmap)(p_xbutton_t hObject, p_xbitmap_t hBitmap);
PUBLIC void BUTTON_MARK_HEAD(SetUpBitmap)(p_xbutton_t hObject, p_xbitmap_t hBitmap);

PUBLIC void BUTTON_MARK_HEAD(SetText)(p_xbutton_t hObject, const char *text);

PRIVATE void BUTTON_MARK_HEAD(Paint)(void *hObject);
PRIVATE int8_t BUTTON_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg);

#endif
