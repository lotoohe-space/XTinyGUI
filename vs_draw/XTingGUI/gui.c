
#include "gui.h"
#include "list.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"
#include <string.h>

//volatile uint32 GUITick = 0;
extern unsigned long long GetCurrentTimeMsec();
uint32 GUIGetTick(void) {
	return (uint32)GetCurrentTimeMsec();
}

/*桌面也应该是一个窗体*/
HXDESKTOP hXDesktop = NULL;

//添加一个控件到桌面
int8 WinListAdd(HWIDGE_BASE hWidgeBase) {

	WindowsAdd(hXDesktop->desktopWin, hWidgeBase);

	return 0;
}
//获取顶层的控件
HWIDGE_BASE WinGetTop(void) {
	HLIST tList = _PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList;
	tList = tList->next;
	while (tList) {
		if (tList->next == NULL) {
			return (HWIDGE_BASE)(tList->val);
		}
		tList = tList->next;
	}
	return NULL;
}
//将一个控件移动到最顶上
void WinMoveTop(void* hObject) {
	if (!hObject) { return; }
	HLIST tHWinList = NULL;
	HLIST tList = _PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList;
	//tList = tList->next;
	while (tList->next) {
		if (tList->next->val == hObject) {
			tHWinList = tList->next;
			break;
		}
		tList = tList->next;
	}
	if (tList->next == NULL) {
		return;
	}
	if (tList->next->next) {
		tList->next->next->lnext = tList;
	}
	tList->next = tList->next->next;
	while (tList->next) {
		tList = tList->next;
	}
	tList->next = tHWinList;
	tHWinList->next = NULL;
	tHWinList->lnext = tList;
	hXDesktop->topWin = hObject;
}
/*设置绘图取悦*/
void _DrawInvaildRect(HXRECT hXRect) {
   XRECT_COPY(&(hXDesktop->drawArea), hXRect);
}
/*GUI事件处理*/
void GUIEvent(void) {
	HMSGE hTempMsg;
	while ((hTempMsg = GUIGetMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_TOUCH) {/*触摸事件*/
			HWIN tmpTopHWin = NULL;
			if (_PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList) {
				int8 ret;
				HLIST lastWin = ListGetLast(_PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList);
				if (lastWin != NULL) {
					while (lastWin != _PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList) {
						HWIDGE_BASE hWidgeBase = (HWIDGE_BASE)(lastWin->val);
						if (_GET_IS_WIN(hWidgeBase)) {
							/*是否为窗口,是窗口则如果被点击则，窗口置顶*/
							if ((ret = hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg)) == 0 || ret == 1) {//是被点击了
								tmpTopHWin = (HWIN)hWidgeBase;
								break;
							}
						}
						else {
							/*不是窗口则事件直接传递*/
							hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg);
						}

						lastWin = lastWin->lnext;
					}
				}
			}
			if (tmpTopHWin != NULL 
				&& tmpTopHWin != hXDesktop->topWin
				) {
				/*如果已经是顶层窗口则不需要再设置为顶层窗口*/
				if (_GET_IS_WIN(tmpTopHWin)) {
					WinMoveTop(tmpTopHWin);
					GUISendDrawMsg(tmpTopHWin, MSG_WIN_INVAILD_UPDATE, 0,
						WIDGE_X(tmpTopHWin), WIDGE_Y(tmpTopHWin),
						WIDGE_W(tmpTopHWin), WIDGE_H(tmpTopHWin)
					);
				}
			}
		}
		
		else if (hTempMsg->msgType == MSG_KEY) {
			if (hXDesktop->topWin!=NULL) {
				_PToHWidgeBaseType(hXDesktop->topWin)->widgeCallBackFun(hXDesktop->topWin, hTempMsg);
			}
		}
		GUIDelMsg(hTempMsg);
	}
	/*窗口移动事件*/
	while ((hTempMsg = GUIGetMoveMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_WIN) {
			if (hTempMsg->msgID == MSG_WIN_MOVE) {
				if (_GET_IS_WIN(hTempMsg->msgSrc)) {
					WindowsMoveTo(hTempMsg->msgSrc, hTempMsg->msgVal.rect.x, hTempMsg->msgVal.rect.y);
				}
			}
		}
		GUIDelMoveMsg(hTempMsg);
	}

	while ((hTempMsg = GUIGetDrawMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_WIN_INVAILD_UPDATE) {
			XRECT hRect;
			hRect.x = ((HXRECT)(hTempMsg->msgSrc))->x;
			hRect.y = ((HXRECT)(hTempMsg->msgSrc))->y;
			hRect.w = ((HXRECT)(hTempMsg->msgSrc))->w;
			hRect.h = ((HXRECT)(hTempMsg->msgSrc))->h;
			_DrawInvaildRect(&hRect);
			HXRECT srcWidge = ((HXRECT)(&(hTempMsg->msgVal.rect)));
			SetMovingRect(srcWidge);

			GUIExec();
		}
		GUIDelDrawMsg(hTempMsg);
	}
}

/*GUI初始化*/
HXDESKTOP GUIInit(void) {
	GUIMsgEventInit();/*事件初始化*/

	/*创建一个桌面*/
	hXDesktop = (HXDESKTOP)xMalloc( sizeof(XDESKTOP));
	if (hXDesktop == NULL) { return NULL; }
	hXDesktop->desktopWin=WindowsCreate("", 0, 0, LCD_SCREEN_W, LCD_SCREEN_H);
	if (hXDesktop->desktopWin == NULL) {
		xFree( hXDesktop);
		return NULL;
	}
	hXDesktop->topWin = NULL;

	hXDesktop->drawArea.x = 0;
	hXDesktop->drawArea.y = 0;
	hXDesktop->drawArea.w = LCD_SCREEN_W;
	hXDesktop->drawArea.h = LCD_SCREEN_H;

	/*设置桌面默认颜色*/
	WindowsSetColor(hXDesktop->desktopWin, 0xffff);
	/*设置桌面不显示头部以及边框*/
	WindowsSetDrawHead(hXDesktop->desktopWin,FALSE);

	/*剪裁初始化*/
	if (!RectCutInit()) {
		WindowsClose( hXDesktop->desktopWin);
		xFree( hXDesktop);
		return NULL;
	}
	return hXDesktop;
}
/*设置当前正在移动的窗口*/
void SetMovingRect(HXRECT hXRect) {
	if (hXRect == NULL) { return; }
	XRECT_COPY(&(hXDesktop->movingRect), hXRect);
}

/*当前的窗口是否需要剪裁
	TRUE:需要剪裁
*/
BOOL IsGUINeedCutEx(HWIN hWin) {
	HXRECT hXRECT;
	uint8 flag = 0;
	if (hWin == NULL) { return TRUE; }
	hXRECT = (HXRECT)(hWin);
#if USE_ALPHA
	/*如果透明则需要剪裁，也就是需要绘制*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}
	/*如果当前窗口时移动窗口则刷新*/
	if (hXRECT == &(hXDesktop->movingRect)) {
		return TRUE;
	}
	/*如果当前窗口和移动窗口产生了碰撞则刷新*/
	if (_IsDrawCheckArea(hXDesktop->movingRect.x
		, hXDesktop->movingRect.y
		, hXDesktop->movingRect.w
		, hXDesktop->movingRect.h
		, hXRECT->x
		, hXRECT->y
		, hXRECT->w
		, hXRECT->h
	)) {
		return TRUE;
	}
	if (WIDGE_PARENT(hWin) != NULL) {/*有父窗口才能操作*/
		
		/*如果当前窗口上面有透明窗口则刷新*/
		_StartScanU((_PToHGroupWidgeType((WIDGE_PARENT(hWin))))->widgetList)
			HXRECT hItemRect = (HXRECT)(val);
		if (flag == 1)
		{
			if (_GET_IS_DPY(hItemRect)) {
				return _IsDrawCheckArea(
					hItemRect->x
					, hItemRect->y
					, hItemRect->w
					, hItemRect->h
					, hXRECT->x
					, hXRECT->y
					, hXRECT->w
					, hXRECT->h
				);
			}
		}
		if (tempItem->val == hWin) {/**/
			flag = 1;
		}
		_EndScanU()
	}

	return TRUE;
#else
	/*如果透明则需要剪裁，也就是需要绘制*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}

	if (hXRECT == &(hXDesktop->movingRect)) {
		return TRUE;
	}
	return _IsDrawCheckArea(
		hXDesktop->movingRect.x
		, hXDesktop->movingRect.y
		, hXDesktop->movingRect.w
		, hXDesktop->movingRect.h
		, hXRECT->x
		, hXRECT->y
		, hXRECT->w
		, hXRECT->h
	);

#endif

}

/*当前的窗口是否需要剪裁
	TRUE:需要剪裁
*/
BOOL IsGUINeedCut(HWIDGE_BASE hWidgeBase) {
	HXRECT hXRECT;
	hXRECT = (HXRECT)hWidgeBase;
#if USE_ALPHA
	/*暂时这么写*/
	return TRUE;
#else
	if (hXRECT == NULL) { return TRUE; }

	/*如果透明则需要剪裁，也就是需要绘制*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}
	if (hXRECT == &(hXDesktop->movingRect)) {
		return TRUE;
	}
	return _IsDrawCheckArea(hXDesktop->movingRect.x
		, hXDesktop->movingRect.y
		, hXDesktop->movingRect.w
		, hXDesktop->movingRect.h
		, hXRECT->x
		, hXRECT->y
		, hXRECT->w
		, hXRECT->h
	);
#endif

}
/*设置允许绘图的窗口，是当前窗口与桌面的相交区域*/
uint8 DrawSetArea(HWIDGE_BASE hWidgeBase) {
	XRECT rRect;
	if (!hWidgeBase) { return FALSE; }

	/*控件矩形与桌面矩形的相交矩形即为绘图区域*/
	GetOverLapRect(&(hXDesktop->drawArea), &(hWidgeBase->rect), &rRect);
	//GetOverLapRect(&(((HWIDGE_BASE)(hWidgeBase->parentHWIN))->pencil),&(hWidgeBase->rect),&rRect);

	/*重新设置绘图区域*/
	XRECT_COPY(&(hXDesktop->drawArea), &rRect);

	/*重新设置控件的有效绘图区域*/
	XRECT_COPY(&(hWidgeBase->pencil), &rRect);

	if (rRect.w == 0 || rRect.h == 0) {
		return FALSE;
	}
	return TRUE;
}
/*复位绘图区域*/
void DrawResetArea(HWIDGE_BASE hWidgeBase) {
	if (!hWidgeBase) { return; }
	XRECT_COPY(&(hXDesktop->drawArea), _PToHXRECTType(&(hWidgeBase->pencil)));
}
//GUI执行函数
void GUIExec(void) {
	/*如果绘图区域需要更新才进行更新*/
	if (hXDesktop->drawArea.x != -1) {
#if !USE_ALPHA
 		if (hXDesktop->topWin!=NULL
			&& memcmp(&(hXDesktop->drawArea), &(_PToHXRECTType(hXDesktop->topWin)),sizeof(XRECT)) ==0 /*顶部窗口与绘制区域一样大*/
			) {
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			_PToHWidgeBaseType(hXDesktop->topWin)->paintFun(hXDesktop->topWin);		/*则只重绘顶部窗口*/
		}
		else {
#endif
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			_PToHWidgeBaseType(hXDesktop->desktopWin)->paintFun(hXDesktop->desktopWin);	/*桌面重绘*/
#if !USE_ALPHA
		}
#endif
		hXDesktop->drawArea.x = -1;
	}
}



