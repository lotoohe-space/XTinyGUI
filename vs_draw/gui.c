
#include "gui.h"
#include "list.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"
#include <string.h>

/*����ҲӦ����һ������*/
HXDESKTOP hXDesktop = NULL;



//���һ�����ڵ��б�
int8 WinListAdd(HWIN hWin) {

	WindowsWidgeAdd(hXDesktop->desktopWin, hWin);

	/*HLIST addItem = ListNew();
	addItem->val = hWin;
	if (!addItem) { return -1; }
	if (ListAddLast(hXDesktop->desktopWin->widgetList, addItem) == -1) {
		return -1;
	}
	hXDesktop->topWin = hWin;*/
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
HMSGE hTempMsg = NULL;
void GUIPostEvent(HMSGE hMsg) {
	hTempMsg = hMsg;
}
/*GUI�¼�����*/
void GUIEvent(void) {
	if (hTempMsg == NULL) { return; }
	if (hTempMsg->msgType == MSG_TOUCH) {
		//hXDesktop->desktopWin->widgeCallBackFun(hXDesktop->desktopWin, hMsg);
		HWIN tmpTopHWin = NULL;
		if (hXDesktop->desktopWin->widgetList) {
			int8 ret;
			HLIST lastWin = ListGetLast(hXDesktop->desktopWin->widgetList);
			if (lastWin != NULL) {
				while (lastWin != hXDesktop->desktopWin->widgetList) {
					HWIN hWin = (HWIN)(lastWin->val);

					if ((ret = WindowsCallBack(hWin, hTempMsg)) == 0 || ret == 1) {//�Ƿ�ͷ�������
						tmpTopHWin = hWin;
						break;
					}

					lastWin = lastWin->lnext;
				}
			}
		}
		if (tmpTopHWin != NULL && tmpTopHWin != hXDesktop->topWin) {
			WinMoveTop(tmpTopHWin);
			DrawInvaildRect(tmpTopHWin);
		}
		hTempMsg = NULL;
	}
	else if (hTempMsg->msgType == MSG_WIN) {
		WindowsMoveTo(hTempMsg->msgSrc, hTempMsg->msgVal.xy.x, hTempMsg->msgVal.xy.y);
	}
}

//��ʼ��GUI
HXDESKTOP GUIInit(void) {

	hXDesktop = (HXDESKTOP)xMalloc( sizeof(XDESKTOP));
	if (hXDesktop == NULL) { return NULL; }
	hXDesktop->desktopWin=WindowsCreate("", 0, 0, LCD_SCREEN_W, LCD_SCREEN_H);
	if (hXDesktop->desktopWin == NULL) {
		xFree( hXDesktop);
		return NULL;
	}
	hXDesktop->topWin = NULL;
	hXDesktop->winMoving = NULL;

	WindowsSetColor(hXDesktop->desktopWin, 0xffff);
	WindowsSetDrawHead(hXDesktop->desktopWin,0);

	if (!RectCutInit()) {
		WindowsClose( hXDesktop->desktopWin);
		xFree( hXDesktop);
		return NULL;
	}
	return hXDesktop;
}
void setMovingWin(HWIN hWin) {
	hXDesktop->winMoving = hWin;
}
/*��ǰ�Ĵ����Ƿ���Ҫ����*/
BOOL isGUINeedCut(HXRECT hXRECT) {
	if (hXRECT == NULL) { return TRUE; }
	
	if (hXDesktop->winMoving == hXDesktop->topWin) {
		if (hXDesktop->winMoving == NULL) {
			return TRUE;
		}
		if (hXRECT == &(hXDesktop->winMoving->winWidge.rect)) {
			return TRUE;
		}
		return _IsDrawCheckArea(hXDesktop->winMoving->winWidge.rect.x - hXDesktop->winMoving->dx
			, hXDesktop->winMoving->winWidge.rect.y - hXDesktop->winMoving->dy
			, hXDesktop->winMoving->winWidge.rect.w
			, hXDesktop->winMoving->winWidge.rect.h
			, hXRECT->x
			, hXRECT->y
			, hXRECT->w
			, hXRECT->h
		);
	}
	 if(hXDesktop->winMoving != NULL) {
		if (hXRECT == hXDesktop->winMoving) {
			return TRUE;
		}
		return _IsDrawCheckArea(hXDesktop->winMoving->winWidge.rect.x - hXDesktop->winMoving->dx
			, hXDesktop->winMoving->winWidge.rect.y - hXDesktop->winMoving->dy
			, hXDesktop->winMoving->winWidge.rect.w
			, hXDesktop->winMoving->winWidge.rect.h
			, hXRECT->x
			, hXRECT->y
			, hXRECT->w
			, hXRECT->h
		);
	}
	if(hXDesktop->topWin != NULL){
		if (hXRECT == hXDesktop->topWin) {
			return TRUE;
		}
		return _IsDrawCheckArea(hXDesktop->topWin->winWidge.rect.x - hXDesktop->topWin->dx
			, hXDesktop->topWin->winWidge.rect.y - hXDesktop->topWin->dy
			, hXDesktop->topWin->winWidge.rect.w
			, hXDesktop->topWin->winWidge.rect.h
			, hXRECT->x
			, hXRECT->y
			, hXRECT->w
			, hXRECT->h
		);
	}

	return TRUE;
	
}

extern  XRECT drawArea;
//GUIִ�к���
void GUIExec(void) {
	if (drawArea.x != -1) {
 		if (hXDesktop->topWin!=NULL
			&& memcmp(&drawArea, &(hXDesktop->topWin->winWidge.rect),sizeof(XRECT)) ==0
			) {
			hXDesktop->topWin->winWidge.paintFun(hXDesktop->topWin);
		}
		else {
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			hXDesktop->desktopWin->winWidge.paintFun(hXDesktop->desktopWin);
		}
		drawArea.x = -1;
	}
}



