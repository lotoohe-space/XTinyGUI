
#include "paint.h"
#include "paint_cut.h"
#include "button_widge.h"
#include "x_malloc.h"

/*创建一个button*/
PUBLIC p_xbutton_t BUTTON_MARK_HEAD(Create)(const char *text, int16_t x, int16_t y, int16_t w, int16_t h)
{
	p_xbutton_t hXButton = xMalloc(sizeof(xbutton_t));
	if (hXButton == NULL)
	{
		return NULL;
	}
	TEXT_MARK_HEAD(Init)
	((p_text_widget_t)hXButton, text, x, y, w, h);

	_PToHWidgeBaseType(hXButton)->paintFun = BUTTON_MARK_HEAD(Paint);
	_PToHWidgeBaseType(hXButton)->moveToFun = BUTTON_MARK_HEAD(MoveTo);
	_PToHWidgeBaseType(hXButton)->widgeCallBackFun = BUTTON_MARK_HEAD(CallBack);

	//_PToHTextWidgeType(hXButton)->hFont = (p_font_t)& fontGB231216_16;
	hXButton->flag = 0;

	hXButton->downBitmap = NULL;
	hXButton->upBitmap = NULL;

	hXButton->downColor = _DefaultButtonDownColor;
	hXButton->upColor = _DefaultButtonUpColor;

	_PToHTextWidgeType(hXButton)->textMode = TEXT_CENTER_ALIGN;

	return hXButton;
}
PUBLIC void BUTTON_MARK_HEAD(SetDownBitmap)(p_xbutton_t hObject, p_xbitmap_t hBitmap)
{
	if (hObject == NULL)
	{
		return;
	}
	hObject->downBitmap = hBitmap;
}
PUBLIC void BUTTON_MARK_HEAD(SetUpBitmap)(p_xbutton_t hObject, p_xbitmap_t hBitmap)
{
	if (hObject == NULL)
	{
		return;
	}
	hObject->upBitmap = hBitmap;
}
/*设置Buttton文字*/
PUBLIC void BUTTON_MARK_HEAD(SetText)(p_xbutton_t hObject, const char *text)
{
	if (hObject == NULL)
	{
		return;
	}
	TEXT_MARK_HEAD(SetText)
	((p_text_widget_t)hObject, text);
}
/*设置点击回调*/
PUBLIC void BUTTON_MARK_HEAD(SetClickBack)(p_xbutton_t hObject, void *arg, ViewClickCallBack viewClickCallBack)
{
	WidgetSetClickBack((p_widget_base_t)hObject, arg, viewClickCallBack);
}
/*移动控件到指定位置*/
PUBLIC void BUTTON_MARK_HEAD(MoveTo)(p_xbutton_t hObject, int16_t x, int16_t y)
{
	if (!hObject)
	{
		return;
	}
	WidgetMoveTo((p_widget_base_t)hObject, x, y);
}
/*绘制窗口*/
PRIVATE void BUTTON_MARK_HEAD(Paint)(void *hObject)
{
	p_xbutton_t hXButton;
	//	xrect_t xRECT;
	hXButton = hObject;
	if (!hObject)
	{
		return;
	}
	if (!DrawSetArea((p_widget_base_t)hXButton))
	{
		return;
	}
	// if (!IsGUINeedCut(hXButton)) { return ; }

	if (!_GetBtnStatus(hXButton))
	{ /*按键松开*/
		xpoint_t startPoint;
		_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonUpColor;
		_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;
		if (!_GetBtnMode(hXButton))
		{ /*文本模式*/
			TEXT_MARK_HEAD(GetPOIByTextMode)
			((p_text_widget_t)hXButton, &startPoint);
			/*根据模式确定起点*/
			DrawCutString(hXButton,
						  _PToHTextWidgeType(hXButton)->hFont,
						  &(_PToHWidgeBaseType(hXButton)->rect),
						  &startPoint,
						  _PToHTextWidgeType(hXButton)->text);
		}
		else
		{ /*图片模式*/
			if (hXButton->upBitmap == NULL)
			{
				_PToHWidgeBaseType(hXButton)->pencil.DrawColor = _DefaultButtonUpColor;
				DrawCutRect(hXButton, _PToHXRECTType(hXButton));
			}
			else
			{
				DrawCutBitmap(hXButton, _PToHXRECTType(hXButton), hXButton->upBitmap);
			}
		}
	}
	else
	{ /*按键按下*/
		xpoint_t startPoint;
		_PToHWidgeBaseType(hXButton)->pencil.DrawBkColor = _DefaultButtonDownColor;
		_PToHWidgeBaseType(hXButton)->pencil.DrawFrColor = _DefaultButtonFontColor;
		if (!_GetBtnMode(hXButton))
		{ /*文本模式*/
			/*根据模式确定起点*/
			TEXT_MARK_HEAD(GetPOIByTextMode)
			((p_text_widget_t)hXButton, &startPoint);
			DrawCutString(hXButton,
						  _PToHTextWidgeType(hXButton)->hFont,
						  &(_PToHWidgeBaseType(hXButton)->rect),
						  &startPoint,
						  _PToHTextWidgeType(hXButton)->text);
		}
		else
		{ /*图片模式``*/
			if (hXButton->upBitmap == NULL)
			{
				_PToHWidgeBaseType(hXButton)->pencil.DrawColor = _DefaultButtonDownColor;
				DrawCutRect(hXButton, _PToHXRECTType(hXButton));
			}
			else
			{
				DrawCutBitmap(hXButton, _PToHXRECTType(hXButton), hXButton->downBitmap);
			}
		}
	}
	/*恢复绘图区域*/
	DrawResetArea((p_widget_base_t)hXButton);
}
/*事件回调*/
PRIVATE int8_t BUTTON_MARK_HEAD(CallBack)(void *hObject, p_msg_t hMsg)
{
	int8_t ret;
	p_xbutton_t hXButton = hObject;
	if (!hXButton || !hMsg)
	{
		return RES_ASSERT_ERR;
	}
	if (!_GetVisable(hXButton))
	{
		return RES_ASSERT_ERR;
	}

	ret = WidgetCallBack(hXButton, hMsg);
	if (ret == RES_OK)
	{
		if (_GetBtnStatus(hXButton))
		{
			_PToHWidgeBaseType(hXButton)->pencil.DrawColor = hXButton->downColor;
		}
		else
		{
			_PToHWidgeBaseType(hXButton)->pencil.DrawColor = hXButton->upColor;
		}
	}
	return ret;
}
