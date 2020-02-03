
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

//添加一个窗口到列表
int8 WinListAdd(HWIN hWin) {

	WindowsWidgeAdd(hXDesktop->desktopWin, hWin);

	return 0;
}
//获取顶层的窗口
HWIN WinGetTop(void) {
	//if (!hXDesktop->winList) { return NULL; }
	HLIST tList = hXDesktop->desktopWin->widgetList;
	tList = tList->next;
	while (tList) {
		if (tList->next == NULL) {
			return (HWIN)(tList->val);
		}
		tList = tList->next;
	}
	return NULL;
}
//将一个窗口移动到最顶上
void WinMoveTop(void* hObject) {
	//if (!hXDesktop->desktopWin->widgetList) { return ; }
	if (!hObject) { return; }
	HLIST tHWinList = NULL;
	HLIST tList = hXDesktop->desktopWin->widgetList;
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
//获取有焦点的窗口
HWIN WinGetFocus(void) {
	//如果没有存在焦点的窗口则返回顶层的窗口
	return hXDesktop->topWin;
}
void _DrawInvaildRect(HXRECT hXRect) {
   XRECT_COPY(&(hXDesktop->drawArea), hXRect);
}
/*GUI事件处理*/
void GUIEvent(void) {
	HMSGE hTempMsg;
	uint8 flag = 0;
	while ((hTempMsg = GUIGetMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_TOUCH) {/*触摸事件*/
			HWIN tmpTopHWin = NULL;
			if (hXDesktop->desktopWin->widgetList) {
				int8 ret;
				HLIST lastWin = ListGetLast(hXDesktop->desktopWin->widgetList);
				if (lastWin != NULL) {
					while (lastWin != hXDesktop->desktopWin->widgetList) {
						HWIDGE_BASE hWidgeBase = (HWIDGE_BASE)(lastWin->val);
						if (_GET_IS_WIN(hWidgeBase)) {
							/*是否为窗口,是窗口则如果被点击则，窗口置顶*/
							if ((ret = hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg)) == 0 || ret == 1) {//是被点击了
								tmpTopHWin = (HWIN)hWidgeBase;
								break;
							}
						}
						else {
							/*不是窗口则直接刷新即可*/
							hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg);
						}

						lastWin = lastWin->lnext;
					}
				}
			}
			if (tmpTopHWin != NULL && tmpTopHWin != hXDesktop->topWin) {
				if (_GET_IS_WIN(tmpTopHWin)) {
					WinMoveTop(tmpTopHWin);
					GUISendDrawMsg(tmpTopHWin, MSG_WIN_INVAILD_UPDATE, 0,
						tmpTopHWin->winWidge.rect.x, tmpTopHWin->winWidge.rect.y,
						tmpTopHWin->winWidge.rect.w, tmpTopHWin->winWidge.rect.h
					);
				}
			}
		}
		else if (hTempMsg->msgType == MSG_WIN) {/*窗口移动事件*/
			if (hTempMsg->msgID == MSG_WIN_MOVE) {
				if (_GET_IS_WIN(hTempMsg->msgSrc)) {
					WindowsMoveTo(hTempMsg->msgSrc, hTempMsg->msgVal.rect.x, hTempMsg->msgVal.rect.y);
				}
			}
		}
		else if (hTempMsg->msgType == MSG_KEY) {
			if (hXDesktop->topWin!=NULL) {
				hXDesktop->topWin->winWidge.widgeCallBackFun(hXDesktop->topWin, hTempMsg);
			}
		}
		GUIDelMsg(hTempMsg);
		flag = 1;
	}
	if (flag) {
		return;
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
			SetMovingWin(srcWidge);

			GUIExec();
		}
		GUIDelDrawMsg(hTempMsg);
	}
}

//初始化GUI
HXDESKTOP GUIInit(void) {

	GUIMsgEventInit();

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

	WindowsSetColor(hXDesktop->desktopWin, 0xffff);
	WindowsSetDrawHead(hXDesktop->desktopWin,FALSE);

	if (!RectCutInit()) {
		WindowsClose( hXDesktop->desktopWin);
		xFree( hXDesktop);
		return NULL;
	}
	return hXDesktop;
}
/*设置当前正在移动的窗口*/
void SetMovingWin(HXRECT hXRect) {
	if (hXRect == NULL) { return; }
	hXDesktop->movingWidge.x = hXRect->x;
	hXDesktop->movingWidge.y = hXRect->y;
	hXDesktop->movingWidge.w = hXRect->w;
	hXDesktop->movingWidge.h = hXRect->h;
}
/*当前的窗口是否需要剪裁
	TRUE:需要剪裁
*/
//extern void d_rect(int x, int y, int w, int h, int color);
BOOL IsGUINeedCut(HXRECT hXRECT) {

#if USE_ALPHA
	/*暂时这么写*/
	return TRUE;
#else
	if (hXRECT == NULL) { return TRUE; }

	/*如果透明则需要剪裁，也就是需要绘制*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}

		if (hXRECT == &(hXDesktop->movingWidge)) {
			return TRUE;
		}
		return _IsDrawCheckArea(hXDesktop->movingWidge.x
			, hXDesktop->movingWidge.y
			, hXDesktop->movingWidge.w
			, hXDesktop->movingWidge.h
			, hXRECT->x
			, hXRECT->y
			, hXRECT->w
			, hXRECT->h
		);
#endif

}

//extern  XRECT drawArea;
//GUI执行函数
void GUIExec(void) {
	if (hXDesktop->drawArea.x != -1) {
#if !USE_ALPHA
 		if (hXDesktop->topWin!=NULL
			&& memcmp(&(hXDesktop->drawArea), &(hXDesktop->topWin->winWidge.rect),sizeof(XRECT)) ==0 /*顶部窗口与绘制区域一样大*/
			) {
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			hXDesktop->topWin->winWidge.paintFun(hXDesktop->topWin);		/*则只重绘顶部窗口*/
		}
		else {
#endif
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			hXDesktop->desktopWin->winWidge.paintFun(hXDesktop->desktopWin);	/*桌面重绘*/
#if !USE_ALPHA
		}
#endif
		hXDesktop->drawArea.x = -1;
	}
}



