
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

/*����ҲӦ����һ������*/
HXDESKTOP hXDesktop = NULL;

//���һ�����ڵ��б�
int8 WinListAdd(HWIN hWin) {

	WindowsWidgeAdd(hXDesktop->desktopWin, hWin);

	return 0;
}
//��ȡ����Ĵ���
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
//��һ�������ƶ������
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
//��ȡ�н���Ĵ���
HWIN WinGetFocus(void) {
	//���û�д��ڽ���Ĵ����򷵻ض���Ĵ���
	return hXDesktop->topWin;
}
void _DrawInvaildRect(HXRECT hXRect) {
   XRECT_COPY(&(hXDesktop->drawArea), hXRect);
}
/*GUI�¼�����*/
void GUIEvent(void) {
	HMSGE hTempMsg;
	uint8 flag = 0;
	while ((hTempMsg = GUIGetMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_TOUCH) {/*�����¼�*/
			HWIN tmpTopHWin = NULL;
			if (hXDesktop->desktopWin->widgetList) {
				int8 ret;
				HLIST lastWin = ListGetLast(hXDesktop->desktopWin->widgetList);
				if (lastWin != NULL) {
					while (lastWin != hXDesktop->desktopWin->widgetList) {
						HWIDGE_BASE hWidgeBase = (HWIDGE_BASE)(lastWin->val);
						if (_GET_IS_WIN(hWidgeBase)) {
							/*�Ƿ�Ϊ����,�Ǵ��������������򣬴����ö�*/
							if ((ret = hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg)) == 0 || ret == 1) {//�Ǳ������
								tmpTopHWin = (HWIN)hWidgeBase;
								break;
							}
						}
						else {
							/*���Ǵ�����ֱ��ˢ�¼���*/
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
		else if (hTempMsg->msgType == MSG_WIN) {/*�����ƶ��¼�*/
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

//��ʼ��GUI
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
/*���õ�ǰ�����ƶ��Ĵ���*/
void SetMovingWin(HXRECT hXRect) {
	if (hXRect == NULL) { return; }
	hXDesktop->movingWidge.x = hXRect->x;
	hXDesktop->movingWidge.y = hXRect->y;
	hXDesktop->movingWidge.w = hXRect->w;
	hXDesktop->movingWidge.h = hXRect->h;
}

/*��ǰ�Ĵ����Ƿ���Ҫ����
	TRUE:��Ҫ����
*/
BOOL IsGUINeedCutEx(HWIN hWin) {
	HXRECT hXRECT;
	uint8 flag = 0;
	if (hWin == NULL) { return TRUE; }
	hXRECT = (HXRECT)(hWin);
#if USE_ALPHA
	/*���͸������Ҫ���ã�Ҳ������Ҫ����*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}
	/*�����ǰ����ʱ�ƶ�������ˢ��*/
	if (hXRECT == &(hXDesktop->movingWidge)) {
		return TRUE;
	}
	/*�����ǰ���ں��ƶ����ڲ�������ײ��ˢ��*/
	if (_IsDrawCheckArea(hXDesktop->movingWidge.x
		, hXDesktop->movingWidge.y
		, hXDesktop->movingWidge.w
		, hXDesktop->movingWidge.h
		, hXRECT->x
		, hXRECT->y
		, hXRECT->w
		, hXRECT->h
	)) {
		return TRUE;
	}
	if (hWin->winWidge.parentHWIN != NULL) {
		/*�����ǰ����������͸��������ˢ��*/
		_StartScanU(((HWIN)(hWin->winWidge.parentHWIN))->widgetList)
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
	/*���͸������Ҫ���ã�Ҳ������Ҫ����*/
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

/*��ǰ�Ĵ����Ƿ���Ҫ����
	TRUE:��Ҫ����
*/
BOOL IsGUINeedCut(HWIDGE_BASE hWidgeBase) {
	HXRECT hXRECT;
	hXRECT = (HXRECT)hWidgeBase;
#if USE_ALPHA
	/*��ʱ��ôд*/
	return TRUE;
#else
	if (hXRECT == NULL) { return TRUE; }

	/*���͸������Ҫ���ã�Ҳ������Ҫ����*/
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
//GUIִ�к���
void GUIExec(void) {
	if (hXDesktop->drawArea.x != -1) {
#if !USE_ALPHA
 		if (hXDesktop->topWin!=NULL
			&& memcmp(&(hXDesktop->drawArea), &(hXDesktop->topWin->winWidge.rect),sizeof(XRECT)) ==0 /*�����������������һ����*/
			) {
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			hXDesktop->topWin->winWidge.paintFun(hXDesktop->topWin);		/*��ֻ�ػ涥������*/
		}
		else {
#endif
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			hXDesktop->desktopWin->winWidge.paintFun(hXDesktop->desktopWin);	/*�����ػ�*/
#if !USE_ALPHA
		}
#endif
		hXDesktop->drawArea.x = -1;
	}
}



