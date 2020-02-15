#ifndef _BUTTON_WIDGE_H__
#define _BUTTON_WIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "font.h"
#include "text_widge.h"

#define BUTTON_MARK_HEAD(a) ButtonWidge##a

/*按钮的管理结构体*/
typedef struct {
	TEXT_WIDGE	textWidge;			/*继承文本控件*/

	uint8		flag;				/*0:bit 文字模式/图片模式 */
	
	HXBITMAP	downBitmap;			/*按下图片*/
	HXBITMAP	upBitmap;			/*松开图片*/

	uintColor	downColor;			/*按下颜色*/
	uintColor	upColor;			/*松开颜色*/
	
}*HXBUTTON, XBUTTON;

/*button模式*/
#define _SetTextMode(a)		(_CLR_BIT(((HXBUTTON)(a))->flag, 0))
#define _SetBMPMode(a)		(_SET_BIT(((HXBUTTON)(a))->flag, 0))
#define _GetBtnMode(a)		(_GET_BIT(((HXBUTTON)(a))->flag, 0))

/*按下与松开颜色*/
#define _DefaultButtonDownColor RGB565(0xff,0xff,0xff)
#define _DefaultButtonUpColor RGB565(200,200,200)
/*字体颜色*/
#define _DefaultButtonFontColor 0x0000

PUBLIC HXBUTTON BUTTON_MARK_HEAD(Create)(const char *text, int16 x, int16 y, int16 w, int16 h);
PUBLIC void		BUTTON_MARK_HEAD(MoveTo)(HXBUTTON hObject, int16 x, int16 y);
PUBLIC void		BUTTON_MARK_HEAD(SetClickBack)(HXBUTTON hObject, void* arg, ViewClickCallBack viewClickCallBack);

PUBLIC void		BUTTON_MARK_HEAD(SetDownBitmap)(HXBUTTON hObject, HXBITMAP hBitmap);
PUBLIC void		BUTTON_MARK_HEAD(SetUpBitmap)(HXBUTTON hObject, HXBITMAP hBitmap);

PRIVATE void	BUTTON_MARK_HEAD(Paint)(void *hObject);
PRIVATE int8	BUTTON_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
