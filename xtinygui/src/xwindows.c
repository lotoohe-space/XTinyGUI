
#include "xwindows.h"
#include "x_malloc.h"
#include "msg.h"
#include "paint_cut.h"
#include "paint.h"
#include "gui.h"

static void *WINDOWS_MARK_HEAD(GetWidgeEx)(HWIN hWin, uint16 index);
static void WindowsHeadBtnCallBack(void *Object, void *arg, uint8 status);

HWIN WINDOWS_MARK_HEAD(Create)(char *title, int16 x, int16 y, int16 w, int16 h)
{
	/*创建一个窗口*/
	HWIN hWin = (HWIN)(xMalloc(sizeof(WIN)));
	if (hWin == NULL)
	{
		return NULL;
	}
	if (GROUP_MARK_HEAD(Init)(_PToHGroupWidgeType(hWin), x, y, w, h) == FALSE)
	{
		return NULL;
	}
	/*创建一个窗口头部控件*/
	hWin->hWinHead = WINDOWS_HEAD_MARK_HEAD(Create)(title, 0, 0, w, 20);
	if (hWin->hWinHead == NULL)
	{
		// xFree( hWin->widgetList);
		_PToHGroupWidgeType(hWin)->widgeBase.widgeCloseFun(_PToHGroupWidgeType(hWin));
		xFree(hWin);
		return NULL;
	}
	_WIDGET(Init)
	((HWIDGET_BASE)hWin, x, y, w, h);

	/*设置回调函数*/
	_PToHWidgeBaseType(hWin)->paintFun = WINDOWS_MARK_HEAD(Paint);
	_PToHWidgeBaseType(hWin)->moveToFun = WINDOWS_MARK_HEAD(MoveTo);
	_PToHWidgeBaseType(hWin)->widgeCallBackFun = WINDOWS_MARK_HEAD(CallBack);
	_PToHWidgeBaseType(hWin)->widgeResizeFun = WINDOWS_MARK_HEAD(Resize);
	_PToHWidgeBaseType(hWin)->widgeCloseFun = WINDOWS_MARK_HEAD(Close);

	_PToHWidgeBaseType(hWin)->pencil.DrawColor = RGB565_BLACK;
	_PToHWidgeBaseType(hWin)->pencil.DrawFrColor = _DefaultFrColor;
	_PToHWidgeBaseType(hWin)->pencil.DrawBkColor = RGB565(230, 235, 230);

	hWin->lastRect.x = x;
	hWin->lastRect.y = y;
	hWin->lastRect.w = w;
	hWin->lastRect.h = h;
	hWin->t_dx = 0;
	hWin->t_dy = 0;

	hWin->flag = 0;
	/*Windows事件处理回调函数*/
	hWin->winProcessFun = NULL;

	/*设置基础控件中的窗口标识为真*/
	_SET_IS_WIN(hWin);
	/*设置需要全部重绘*/
	_SetDrawAllLag(hWin);
	/*先设置为不显示头部*/
	_ClrDrawWinHead(hWin);

	/*窗口头部控件*/
	WindowsAdd(hWin, hWin->hWinHead);
	/*最小化的回调*/
	WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)
	(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);
	/*最大化回调接口*/
	WINDOWS_HEAD_MARK_HEAD(SetMaximumBtnClickBack)
	(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);
	WINDOWS_HEAD_MARK_HEAD(SetCloseBtnClickBack)
	(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);

	{
		HWIDGET_BASE widgeTemp;
		widgeTemp = _WIDGET(Create)(0, 0, 1, WIDGE_H(hWin));
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		/*添加边框，边框也是控件*/
		WindowsAdd(hWin, widgeTemp);

		widgeTemp = _WIDGET(Create)(0 + WIDGE_W(hWin) - 1, 0, 1, WIDGE_H(hWin));
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsAdd(hWin, widgeTemp);

		widgeTemp = _WIDGET(Create)(0,
									0 + WIDGE_H(hWin) - 1,
									WIDGE_W(hWin->hWinHead), 1);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsAdd(hWin, widgeTemp);

		widgeTemp = _WIDGET(Create)(0, 0,
									WIDGE_W(hWin->hWinHead), 1);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsAdd(hWin, widgeTemp);
	}

	/*默认显示头部*/
	_SetDrawWinHead(hWin);
	return hWin;
}

/*窗口中button点击事件的回调接口函数*/
static void WindowsHeadBtnCallBack(void *Object, void *arg, uint8 status)
{
	HWIN CentWIN = (HWIN)arg;
	if (CentWIN == NULL)
	{
		return;
	}
	if (status)
	{
		if (CentWIN->hWinHead->hXButtonMax == Object)
		{
			/*按下最大化按钮*/
			WindowsSetMax(CentWIN);
		}
		else if (CentWIN->hWinHead->hXButtonMin == Object)
		{
			/*按下最小化按钮*/
			WindowsSetMin(CentWIN);
		}
		else if (CentWIN->hWinHead->hXButtonClose == Object)
		{
			GUISendWINDelMsg(WIDGE_PARENT(CentWIN), CentWIN);
		}
	}
}
/*设置窗口处理回调事件*/
void WINDOWS_MARK_HEAD(SetProcessCallBack)(HWIN hObject, WinProcessCallBack winProcessFun)
{
	if (hObject == NULL)
	{
		return;
	}
	hObject->winProcessFun = winProcessFun;
}
/*设置窗口最小化*/
void WINDOWS_MARK_HEAD(SetMin)(HWIN hObject)
{
	if (!hObject)
	{
		return;
	}
	/*处于最大化时不能够最小化*/
	if (_IsMaxWIN(hObject))
	{
		return;
	}

	if (_IsMinWIN(hObject))
	{
		_ClrMinWIN(hObject);
		/*设置为上次的大小*/
		WindowsResize(hObject,
					  WIDGE_X(hObject),
					  WIDGE_Y(hObject),
					  hObject->lastRect.w,
					  hObject->lastRect.h);
	}
	else
	{
		_SetMinWIN(hObject);
		/*保存之前的宽高*/
		hObject->lastRect.w = WIDGE_W(hObject);
		hObject->lastRect.h = WIDGE_H(hObject);
		/*设置为标题头的大小*/
		WindowsResize(hObject,
					  WIDGE_X(hObject),
					  WIDGE_Y(hObject),
					  WIDGE_W(hObject->hWinHead),
					  WIDGE_H(hObject->hWinHead));
	}
}
/*设置窗口最大化*/
void WINDOWS_MARK_HEAD(SetMax)(HWIN hObject)
{
	if (!hObject)
	{
		return;
	}

	/*最小化时不能够最大化*/
	if (_IsMinWIN(hObject))
	{
		return;
	}

	if (_IsMaxWIN(hObject))
	{
		_ClrMaxWIN(hObject);
		/*还原位置*/
		// XRECT_COPY((HXRECT)hObject,&(hObject->lastRect));
		WindowsResize(hObject,
					  hObject->lastRect.x,
					  hObject->lastRect.y,
					  hObject->lastRect.w,
					  hObject->lastRect.h);
	}
	else
	{
		_SetMaxWIN(hObject);
		XRECT_COPY(&(hObject->lastRect), hObject);
		WindowsResize(hObject,
					  WIDGE_X(WIDGE_PARENT(hObject)),
					  WIDGE_Y(WIDGE_PARENT(hObject)),
					  WIDGE_W(WIDGE_PARENT(hObject)),
					  WIDGE_H(WIDGE_PARENT(hObject)));
	}
}

/*重新设置窗口的大小,还需要更改内部控件的大小与位置*/
void WINDOWS_MARK_HEAD(Resize)(HWIN hObject, int16 x, int16 y, uint16 w, uint16 h)
{
	HWIDGET_BASE hWidgeBase;
	HWIDGET_BASE hWidgeBaseHead;
	if (!hObject)
	{
		return;
	}

	int16 dx, dy;
	/*窗口位置改变的偏移*/
	dx = x - ((HXRECT)hObject)->x;
	dy = y - ((HXRECT)hObject)->y;

	_WIDGET(Resize)
	((HWIDGET_BASE)hObject, x, y, w, h);

	hWidgeBaseHead = WindowsGetWidge(hObject, 0);
	if (hWidgeBaseHead == NULL)
	{
		return;
	}
	WINDOWS_HEAD_MARK_HEAD(Resize)
	((HWIN_HEAD)hWidgeBaseHead, x, y, w, 20);

	hWidgeBase = WindowsGetWidge(hObject, 1);
	if (hWidgeBase == NULL)
	{
		return;
	}
	_WIDGET(Resize)
	(hWidgeBase, x, y, 1, h);

	hWidgeBase = WindowsGetWidge(hObject, 2);
	if (hWidgeBase == NULL)
	{
		return;
	}
	_WIDGET(Resize)
	(hWidgeBase, x + w - 1, y, 1, h);

	hWidgeBase = WindowsGetWidge(hObject, 3);
	if (hWidgeBase == NULL)
	{
		return;
	}
	_WIDGET(Resize)
	(hWidgeBase, x,
	 y + h - 1,
	 w, 1);
	hWidgeBase = WindowsGetWidge(hObject, 4);
	if (hWidgeBase == NULL)
	{
		return;
	}
	_WIDGET(Resize)
	(hWidgeBase, x,
	 y,
	 w, 1);
	/*迭代每一个控件*/
	_StartScanU(WindowsGetWidgeEx(hObject, 4))
	{ // 开始扫描
		_PToHWidgeBaseType(val)->widgeResizeFun(
			_PToHWidgeBaseType(val),
			_PToHWidgeBaseType(val)->rect.x + dx,
			_PToHWidgeBaseType(val)->rect.y + dy,
			_PToHWidgeBaseType(val)->rect.w,
			_PToHWidgeBaseType(val)->rect.h);
	}
	/*结束扫描*/
	_EndScanU();
	/*改变窗口大小必须刷新父控件*/
	WindowsInvaildRect(((HWIDGET_BASE)hObject)->parentHWIN, NULL);
}

/*获取控件*/
HWIDGET_BASE WINDOWS_MARK_HEAD(GetWidge)(HWIN hObject, uint16 index)
{
	if (!hObject)
	{
		return NULL;
	}
	HLIST hlist = ListGet(_PToHGroupWidgeType(hObject)->widgetList, index);
	if (hlist != NULL)
	{
		return (HWIDGET_BASE)(hlist->val);
	}
	else
	{
		return NULL;
	}
}
/*获取窗口中的某个位置的控件，得到的不是窗口而是窗口的ListItem*/
static void *WINDOWS_MARK_HEAD(GetWidgeEx)(HWIN hWin, uint16 index)
{
	if (!hWin)
	{
		return NULL;
	}
	HLIST hlist = ListGet(_PToHGroupWidgeType(hWin)->widgetList, index);
	return hlist;
}
/*往窗体中添加一个控件*/
int8 WINDOWS_MARK_HEAD(Add)(HWIN hWin, void *widge)
{
	if (!hWin || !widge)
	{
		return -1;
	}
	HLIST addItem = ListNew();
	if (!addItem)
	{
		return -1;
	}
	addItem->val = widge;

	if (ListAddLast(_PToHGroupWidgeType(hWin)->widgetList, addItem) == -1)
	{
		return -1;
	}
	HWIDGET_BASE hWidge = widge;

	hWidge->moveToFun(
		hWidge,
		hWidge->rect.x + WIDGE_X(hWin),
		hWidge->rect.y + WIDGE_Y(hWin) + (_IsDrawWinHead(hWin) ? _PToHGroupWidgeType(hWin->hWinHead)->widgeBase.rect.h : 0));
	/*设置父窗口*/
	_WIDGET(SetParentWin)
	(hWidge, hWin);

	/* 设置更新窗口区域 */
	WindowsInvaildRect((HWIDGET_BASE)(WIDGE_PARENT(hWin)), (HXRECT)(hWin));
	return 0;
}
/*设置窗口的无效区域，调用gui的无效区域设置函数*/
void WINDOWS_MARK_HEAD(InvaildRect)(HWIDGET_BASE hWidgeBase, HXRECT hXRect)
{
	HWIDGET_BASE srcWidge;
	if (!hWidgeBase)
	{
		return;
	}
	/*如果父窗口是透明窗口则源为透明窗口*/
	srcWidge = hWidgeBase;
	// while (WIDGE_PARENT(srcWidge)!=NULL && _GET_IS_DPY(WIDGE_PARENT(srcWidge))) {
	//	srcWidge = WIDGE_PARENT(srcWidge);
	// }

	if (hXRect == NULL)
	{
		GUISendDrawMsg(srcWidge, MSG_WIN_INVAILD_UPDATE, 0,
					   hWidgeBase->rect.x, hWidgeBase->rect.y, hWidgeBase->rect.w, hWidgeBase->rect.h);
	}
	else
	{
		GUISendDrawMsg(srcWidge, MSG_WIN_INVAILD_UPDATE, 0,
					   hXRect->x, hXRect->y, hXRect->w, hXRect->h);
	}
}
/*
 * 关闭窗口
 */
void WINDOWS_MARK_HEAD(Close)(HWIN hWin)
{
	if (!hWin)
	{
		return;
	}
	/*在这里释放窗口中的内存*/

	/*处理该类需要释放的内存*/

	/*调用父类的关闭函数*/
	GROUP_MARK_HEAD(Close)
	((HGROUP_WIDGE)hWin);
}
/*移动窗体*/
void WINDOWS_MARK_HEAD(MoveTo)(HWIN hWin, int16 x, int16 y)
{
	int16 dx;
	int16 dy;
	XRECT lastRect;
	XRECT updateRect;
	if (!hWin)
	{
		return;
	}
	/*新位置相对于上一次移动的偏移量*/
	dx = x - WIDGE_X(hWin);
	dy = y - WIDGE_Y(hWin);

	/*位置没有改变*/
	if (x == WIDGE_X(hWin) && y == WIDGE_Y(hWin))
	{
		return;
	}
	XRECT_COPY(&lastRect, (HXRECT)hWin);
	/* 更新窗口，更新的位置是上一次的位置*/
	// WindowsInvaildRect((HWIDGET_BASE)(WIDGE_PARENT(hWin)), (HXRECT)(&(lastRect)));
	/*设置新的位置*/
	_PToHXRECTType(hWin)->x = x;
	_PToHXRECTType(hWin)->y = y;

	// 重新设置每一个控件位置
	HLIST hWidgeList = _PToHGroupWidgeType(hWin)->widgetList;

	/*移动内部的每一个控件*/
	if (hWidgeList != NULL)
	{
		hWidgeList = hWidgeList->next;
		while (hWidgeList)
		{
			HWIDGET_BASE hWidge = (HWIDGET_BASE)(hWidgeList->val);
			/*对内部每一个控件进行偏移*/
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);
			hWidgeList = hWidgeList->next;
		}
	}

	updateRect.x = MIN(((HXRECT)(hWin))->x, lastRect.x);
	updateRect.y = MIN(((HXRECT)(hWin))->y, lastRect.y);
	updateRect.w = MAX(((HXRECT)(hWin))->w + ((HXRECT)(hWin))->x,
					   lastRect.x + lastRect.w) -
				   updateRect.x;
	updateRect.h = MAX(((HXRECT)(hWin))->h + ((HXRECT)(hWin))->y,
					   lastRect.y + lastRect.h) -
				   updateRect.y;

	WindowsInvaildRect((HWIDGET_BASE)(WIDGE_PARENT(hWin)), (HXRECT)(&updateRect));
}
/*设置窗口颜色*/
void WINDOWS_MARK_HEAD(SetColor)(HWIN hWin, uintColor color)
{
	if (!hWin)
	{
		return;
	}
	_PToHWidgeBaseType(hWin)->pencil.DrawBkColor = color;

	/* 更新窗口 */
	WindowsInvaildRect((HWIDGET_BASE)hWin, (HXRECT)hWin);
}

/*设置是否有效*/
void WINDOWS_MARK_HEAD(SetVisable)(void *hObject, int8 isVisable)
{
	HWIN hWin;
	hWin = hObject;
	if (!hWin)
	{
		return;
	}
	_WIDGET(SetVisable)
	((HWIDGET_BASE)hWin, isVisable);
}
/*
 *重新绘制窗口
 */
void WINDOWS_MARK_HEAD(Paint)(void *hObject)
{
	HLIST cutPostionList;
	HWIN hWin;
	hWin = hObject;
	if (!hWin)
	{
		return;
	}

	if (_IsDrawAllLag(hWin))
	{
		/*首次的创建事件*/
		if (hWin->winProcessFun != NULL)
		{
			MSGE msg;
			msg.msgSrc = hWin;
			msg.msgType = MSG_WIN;
			msg.msgID = MSG_WIN_CREATE;
			hWin->winProcessFun(hWin, _PToHWidgeBaseType(hWin)->arg, &msg);
		}
	}
	/*是否需要显示*/
	if (!_GetVisable(hWin))
	{
		return;
	}
	/*是否需要剪裁*/
	if (!IsGUINeedCut((HWIDGET_BASE)hWin) && !_IsDrawAllLag(hWin))
	{
		return;
	}

	/*初次需要全部刷新*/
	_ClrDrawAllLag(hWin);

	/*设置当前绘图区域*/
	if (!DrawSetArea((HWIDGET_BASE)hWin))
	{
		return;
	}

	/*根据是否添加了窗体边框来设置剪裁域*/
	if (_IsDrawWinHead(hWin))
	{
		/*切换到当前剪裁域*/
		cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hWin)->widgetList->next);
	}
	else
	{
		cutPostionList = RectCutAddRectList(WindowsGetWidgeEx(hWin, 5));
	}

	_PToHWidgeBaseType(hWin)->pencil.DrawColor = _PToHWidgeBaseType(hWin)->pencil.DrawBkColor;
	/*画矩形*/
	DrawCutRect(hWin, _PToHXRECTType(hWin));
	/*重绘事件*/
	if (hWin->winProcessFun != NULL)
	{
		MSGE msg;
		msg.msgSrc = hWin;
		msg.msgType = MSG_WIN;
		msg.msgID = MSG_WIN_PAINT;
		hWin->winProcessFun(hWin, _PToHWidgeBaseType(hWin)->arg, &msg);
	}
	RectCutSplitRectList(cutPostionList);
	/*迭代每一个控件*/
	_StartScanU(_PToHGroupWidgeType(hWin)->widgetList)
	{ // 开始扫描
		/*当前列表控件加入剪裁域*/
		cutPostionList = RectCutAddRectList(tempItem->next);
		/*绘制控件*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));
		/*退回剪裁域*/
		RectCutSplitRectList(cutPostionList);
	}
	/*结束扫描*/
	_EndScanU();
	/*恢复绘图区域*/
	DrawResetArea((HWIDGET_BASE)hWin);
}
/*是否显示头部*/
void WINDOWS_MARK_HEAD(SetDrawHead)(HWIN hWin, int8 isEnable)
{
	uint8 i = 0;

	if (isEnable)
	{
		_SetDrawWinHead(hWin);
	}
	else
	{
		_ClrDrawWinHead(hWin);
	}

	/*前四个控件为边界和头部*/
	for (i = 0; i < 5; i++)
	{
		HLIST widge = WindowsGetWidgeEx(hWin, i);
		if (widge != NULL)
		{
			if (isEnable)
			{
				_SetVisable(widge->val);
			}
			else
			{
				_ClrVisable(widge->val);
			}
		}
	}
	/* 更新窗口 */
	WindowsInvaildRect((HWIDGET_BASE)hWin, (HXRECT)hWin);
}

/*窗口事件处理*/
int8 WINDOWS_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg)
{
	HWIN hWin = hObject;
	if (!hWin || !hMsg)
	{
		return RES_ASSERT_ERR;
	}
	if (!_GetVisable(hWin))
	{
		return RES_ASSERT_ERR;
	}

	if (hMsg->msgType == MSG_TOUCH)
	{
		if (_IsDrawWinHead(hWin))
		{
			switch (hMsg->msgID)
			{
			case MSG_TOUCH_PRESS:
				// 有头才能抓取移动
				/*被抓的是不是头*/
				if (WINDOWS_HEAD_MARK_HEAD(CallBack)(hWin->hWinHead, hMsg) == RES_OK)
				{
					hWin->t_dx = hMsg->msgVal.rect.x - WIDGE_X(hWin);
					hWin->t_dy = hMsg->msgVal.rect.y - WIDGE_Y(hWin);
					_SetWinMoveing(hWin);
					WindowsInvaildRect(WIDGE_PARENT(hWin), _PToHXRECTType(hWin));
					return 0;
				}
				break;
			case MSG_TOUCH_MOVE:
				// 有头才能抓取移动
				if (_IsWinMoving(hWin))
				{
					WindowsMoveTo(hWin,
								  hMsg->msgVal.rect.x - hWin->t_dx, hMsg->msgVal.rect.y - hWin->t_dy);
					// GUISendMoveMsg(hWin, MSG_WIN, MSG_WIN_MOVE, hMsg->msgVal.rect.x - hWin->t_dx, hMsg->msgVal.rect.y - hWin->t_dy);
					return 0;
				}
				break;
			case MSG_TOUCH_RELEASE:
				if (_IsWinMoving(hWin))
				{
					// 有头才能抓取移动
					_ClrWinMoveing(hWin);
					// return 0;
				}
				break;
			}
		}

		switch (hMsg->msgID)
		{
		case MSG_TOUCH_PRESS:
			if (
				hMsg->msgVal.rect.x >= WIDGE_X(hWin) + WIDGE_W(hWin) - 2 && hMsg->msgVal.rect.x <= WIDGE_X(hWin) + WIDGE_W(hWin))
			{
				hWin->t_xy = hMsg->msgVal.rect.x;
				_SetChangeWinRHSize(hWin);
				return 0;
			}
			else if (
				hMsg->msgVal.rect.y >= WIDGE_Y(hWin) + WIDGE_H(hWin) - 2 && hMsg->msgVal.rect.y <= WIDGE_Y(hWin) + WIDGE_H(hWin))
			{
				hWin->t_xy = hMsg->msgVal.rect.y;
				_SetChangeWinVSize(hWin);
				return 0;
			}
			else if (
				hMsg->msgVal.rect.x >= WIDGE_X(hWin) && hMsg->msgVal.rect.x <= WIDGE_X(hWin) + 2)
			{
				hWin->t_xy = hMsg->msgVal.rect.x;
				_SetChangeWinLHSize(hWin);
				return 0;
			}
			break;
		case MSG_TOUCH_MOVE:
			if (_IsChangeWinRHSize(hWin))
			{
				_PToHWidgeBaseType(hWin)->widgeResizeFun(hWin, ((HXRECT)hWin)->x, ((HXRECT)hWin)->y,
														 ((HXRECT)hWin)->w + (hMsg->msgVal.rect.x - hWin->t_xy), ((HXRECT)hWin)->h);
				hWin->t_xy = hMsg->msgVal.rect.x;
				return 0;
			}
			else if (_IsChangeWinVSize(hWin))
			{
				_PToHWidgeBaseType(hWin)->widgeResizeFun(hWin, ((HXRECT)hWin)->x, ((HXRECT)hWin)->y,
														 ((HXRECT)hWin)->w, (((HXRECT)hWin)->h + (hMsg->msgVal.rect.y - hWin->t_xy)));
				hWin->t_xy = hMsg->msgVal.rect.y;
				return 0;
			}
			else if (_IsChangeWinLHSize(hWin))
			{
				_PToHWidgeBaseType(hWin)->widgeResizeFun(hWin,
														 ((HXRECT)hWin)->x + (hMsg->msgVal.rect.x - hWin->t_xy),
														 ((HXRECT)hWin)->y,
														 ((HXRECT)hWin)->w - (hMsg->msgVal.rect.x - hWin->t_xy),
														 ((HXRECT)hWin)->h);
				hWin->t_xy = hMsg->msgVal.rect.x;
				return 0;
			}
			break;
		case MSG_TOUCH_RELEASE:
			if (_IsChangeWinRHSize(hWin))
			{
				_ClrChangeWinRHSize(hWin);
				return 0;
			}
			if (_IsChangeWinVSize(hWin))
			{
				_ClrChangeWinVSize(hWin);
				return 0;
			}
			if (_IsChangeWinLHSize(hWin))
			{
				_ClrChangeWinLHSize(hWin);
				return 0;
			}
			//
			break;
		}
		if (!_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
							   WIDGE_X(hWin), WIDGE_Y(hWin),
							   WIDGE_W(hWin), WIDGE_H(hWin)))
		{
			return 2;
		}

		HLIST hWidgeList = _PToHGroupWidgeType(hWin)->widgetList;
		if (hWidgeList == NULL)
		{
			return RES_FAIL;
		}

		if (hWidgeList)
		{
			int8 ret;
			HLIST lastWidge = ListGetLast(hWidgeList);
			if (lastWidge != NULL)
			{
				while (lastWidge != hWidgeList)
				{
					HWIDGET_BASE hWidge = (HWIDGET_BASE)(lastWidge->val);

					if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == RES_OK)
					{
						return RES_OK;
					}

					lastWidge = lastWidge->lnext;
				}
			}
		}
		return RES_FAIL;
	}
	else if (hMsg->msgType == MSG_KEY)
	{
		if (hWin->winProcessFun != NULL)
		{
			hWin->winProcessFun(hWin, _PToHWidgeBaseType(hWin)->arg, hMsg);
		}
		return RES_FAIL;
	}

	return RES_FAIL;
}
