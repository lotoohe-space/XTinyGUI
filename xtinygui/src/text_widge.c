
#include "text_widge.h"
#include "x_malloc.h"
#include "x_tool.h"
#include "gui.h"
#include "paint_cut.h"

PUBLIC p_text_widget_t TEXT_MARK_HEAD(Create)(const char *text, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	p_text_widget_t hObject = (p_text_widget_t)(xMalloc(sizeof(text_widget_t)));
	if (hObject == NULL)
	{
		return NULL;
	}
	TEXT_MARK_HEAD(Init)
	(hObject, text, x, y, w, h);
	return hObject;
}
PUBLIC void TEXT_MARK_HEAD(Init)(p_text_widget_t hObject, const char *text, int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	if (hObject == NULL)
	{
		return;
	}
	WidgetInit((p_widget_base_t)hObject, x, y, w, h);

	hObject->baseWidge.paintFun = TEXT_MARK_HEAD(Paint);
	hObject->baseWidge.moveToFun = TEXT_MARK_HEAD(MoveTo);
	hObject->baseWidge.widgeCallBackFun = TEXT_MARK_HEAD(CallBack);

	hObject->hFont = (p_font_t)&fontASCII16_16;
	hObject->text = text ? text : "None";
	hObject->textMode = TEXT_CENTER_ALIGN;
}
PUBLIC void TEXT_MARK_HEAD(SetTextMode)(p_text_widget_t hOjbect, uint8_t textMode)
{
	if (hOjbect == NULL)
	{
		return;
	}
	hOjbect->textMode = textMode;
	WindowsInvaildRect((p_widget_base_t)hOjbect, NULL);
}
PUBLIC void TEXT_MARK_HEAD(SetFont)(p_text_widget_t hObject, p_font_t hFont)
{
	if (hObject == NULL)
	{
		return;
	}
	hObject->hFont = hFont;
	WindowsInvaildRect((p_widget_base_t)hObject, NULL);
}
PUBLIC void TEXT_MARK_HEAD(SetText)(p_text_widget_t hObject, const char *text)
{
	if (hObject == NULL)
	{
		return;
	}
	hObject->text = text;
	WindowsInvaildRect((p_widget_base_t)hObject, NULL);
}
PUBLIC void TEXT_MARK_HEAD(MoveTo)(p_text_widget_t hObject, int16_t x, int16_t y)
{
	if (!hObject)
	{
		return;
	}
	hObject->baseWidge.rect.x = x;
	hObject->baseWidge.rect.y = y;
}
/*根据模式得到文字绘制的起始位置*/
PUBLIC p_xpoint_t TEXT_MARK_HEAD(GetPOIByTextMode)(p_text_widget_t hTextWidge, p_xpoint_t startPoint)
{
	uint16_t textW = 0;
	uint16_t textH = 0;

	if (startPoint == NULL || hTextWidge == NULL)
	{
		return NULL;
	}

	startPoint->x = 0;
	startPoint->y = 0;

	if (hTextWidge->hFont->fontType == UNICODE_TYPE)
	{
		textW = UNI_Strlen((uint16_t *)(hTextWidge->text)) * hTextWidge->hFont->fontInfo.w;
	}
	else
	{
		textW = GBK_Strlen(hTextWidge->text) * hTextWidge->hFont->fontInfo.w;
	}
	textH = hTextWidge->hFont->fontInfo.h;

	switch (hTextWidge->textMode)
	{
	case TEXT_TOP_LEFT_ALIGN:
	{
		startPoint->x = 0;
		startPoint->y = 0;
	}
	break;
	case TEXT_TOP_CENTER_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW) / 2;
	}
	break;
	case TEXT_TOP_RIGHT_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW);
	}
	break;
	case TEXT_BOTTOM_LEFT_ALIGN:
	{
		startPoint->y = WIDGE_H(hTextWidge) - textH;
	}
	break;
	case TEXT_BOTTOM_CENTER_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW) / 2;
		startPoint->y = WIDGE_H(hTextWidge) - textH;
	}
	break;
	case TEXT_BOTTOM_RIGHT_ALIGN:
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW);
		startPoint->y = WIDGE_H(hTextWidge) - textH;
	}
	break;
	case TEXT_LEFT_CENTER_ALIGN:
		startPoint->y = (WIDGE_H(hTextWidge) - textH) / 2;
		break;
	case TEXT_RIGHT_CENTER_ALIGN:
		startPoint->x = (WIDGE_W(hTextWidge) - textW);
		startPoint->y = (WIDGE_H(hTextWidge) - textH) / 2;
		break;
	case TEXT_CENTER_ALIGN: /*居中对齐*/
	{
		startPoint->x = (WIDGE_W(hTextWidge) - textW) / 2;
		startPoint->y = (WIDGE_H(hTextWidge) - textH) / 2;
	}
	break;
	}

	return startPoint;
}
PUBLIC void TEXT_MARK_HEAD(Paint)(void *hObject)
{
	xpoint_t startPoint;
	p_text_widget_t hTextWidge;
	hTextWidge = hObject;
	if (!hObject)
	{
		return;
	}
	if (!_GetVisable(hTextWidge))
	{
		return;
	}
	// if (!isGUINeedCut((p_xrect_t)hTextWidge)) { return; }

	if (!DrawSetArea((p_widget_base_t)hTextWidge))
	{
		return;
	}
	TEXT_MARK_HEAD(GetPOIByTextMode)
	(hTextWidge, &startPoint);
	/*绘制剪裁字符串*/
	DrawCutString(hTextWidge, hTextWidge->hFont,
				  &(hTextWidge->baseWidge.rect),
				  &startPoint,
				  (uint8_t *)(hTextWidge->text));

	/*恢复绘图区域*/
	DrawResetArea((p_widget_base_t)hTextWidge);
}
PUBLIC int8_t TEXT_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg)
{
	p_text_widget_t hTextWidge = hObject;
	if (!hTextWidge || !hMsg)
	{
		return -1;
	}
	if (!_GetVisable(hTextWidge))
	{
		return -1;
	}
	if (hMsg->msgType == MSG_TOUCH)
	{
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
							   &(hTextWidge->baseWidge.rect)))
		{
			return 0;
		}
	}
	return -1;
}
