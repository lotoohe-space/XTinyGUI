
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

//���һ���ؼ�������
int8 WinListAdd(HWIDGE_BASE hWidgeBase) {

	WindowsAdd(hXDesktop->desktopWin, hWidgeBase);

	return 0;
}
//��ȡ����Ŀؼ�
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
//��һ���ؼ��ƶ������
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
/*���û�ͼȡ��*/
void _DrawInvaildRect(HXRECT hXRect) {
   XRECT_COPY(&(hXDesktop->drawArea), hXRect);
}
/*GUI�¼�����*/
void GUIEvent(void) {
	HMSGE hTempMsg;
	while ((hTempMsg = GUIGetMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_TOUCH) {/*�����¼�*/
			HWIN tmpTopHWin = NULL;
			if (_PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList) {
				int8 ret;
				HLIST lastWin = ListGetLast(_PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList);
				if (lastWin != NULL) {
					while (lastWin != _PToHGroupWidgeType(hXDesktop->desktopWin)->widgetList) {
						HWIDGE_BASE hWidgeBase = (HWIDGE_BASE)(lastWin->val);
						if (_GET_IS_WIN(hWidgeBase)) {
							/*�Ƿ�Ϊ����,�Ǵ��������������򣬴����ö�*/
							if ((ret = hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg)) == 0 || ret == 1) {//�Ǳ������
								tmpTopHWin = (HWIN)hWidgeBase;
								break;
							}
						}
						else {
							/*���Ǵ������¼�ֱ�Ӵ���*/
							hWidgeBase->widgeCallBackFun(hWidgeBase, hTempMsg);
						}

						lastWin = lastWin->lnext;
					}
				}
			}
			if (tmpTopHWin != NULL 
				&& tmpTopHWin != hXDesktop->topWin
				) {
				/*����Ѿ��Ƕ��㴰������Ҫ������Ϊ���㴰��*/
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
	/*�����ƶ��¼�*/
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

/*GUI��ʼ��*/
HXDESKTOP GUIInit(void) {
	GUIMsgEventInit();/*�¼���ʼ��*/

	/*����һ������*/
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

	/*��������Ĭ����ɫ*/
	WindowsSetColor(hXDesktop->desktopWin, 0xffff);
	/*�������治��ʾͷ���Լ��߿�*/
	WindowsSetDrawHead(hXDesktop->desktopWin,FALSE);

	/*���ó�ʼ��*/
	if (!RectCutInit()) {
		WindowsClose( hXDesktop->desktopWin);
		xFree( hXDesktop);
		return NULL;
	}
	return hXDesktop;
}
/*���õ�ǰ�����ƶ��Ĵ���*/
void SetMovingRect(HXRECT hXRect) {
	if (hXRect == NULL) { return; }
	XRECT_COPY(&(hXDesktop->movingRect), hXRect);
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
	if (hXRECT == &(hXDesktop->movingRect)) {
		return TRUE;
	}
	/*�����ǰ���ں��ƶ����ڲ�������ײ��ˢ��*/
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
	if (WIDGE_PARENT(hWin) != NULL) {/*�и����ڲ��ܲ���*/
		
		/*�����ǰ����������͸��������ˢ��*/
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
	/*���͸������Ҫ���ã�Ҳ������Ҫ����*/
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
/*���������ͼ�Ĵ��ڣ��ǵ�ǰ������������ཻ����*/
uint8 DrawSetArea(HWIDGE_BASE hWidgeBase) {
	XRECT rRect;
	if (!hWidgeBase) { return FALSE; }

	/*�ؼ�������������ε��ཻ���μ�Ϊ��ͼ����*/
	GetOverLapRect(&(hXDesktop->drawArea), &(hWidgeBase->rect), &rRect);
	//GetOverLapRect(&(((HWIDGE_BASE)(hWidgeBase->parentHWIN))->pencil),&(hWidgeBase->rect),&rRect);

	/*�������û�ͼ����*/
	XRECT_COPY(&(hXDesktop->drawArea), &rRect);

	/*�������ÿؼ�����Ч��ͼ����*/
	XRECT_COPY(&(hWidgeBase->pencil), &rRect);

	if (rRect.w == 0 || rRect.h == 0) {
		return FALSE;
	}
	return TRUE;
}
/*��λ��ͼ����*/
void DrawResetArea(HWIDGE_BASE hWidgeBase) {
	if (!hWidgeBase) { return; }
	XRECT_COPY(&(hXDesktop->drawArea), _PToHXRECTType(&(hWidgeBase->pencil)));
}
//GUIִ�к���
void GUIExec(void) {
	/*�����ͼ������Ҫ���²Ž��и���*/
	if (hXDesktop->drawArea.x != -1) {
#if !USE_ALPHA
 		if (hXDesktop->topWin!=NULL
			&& memcmp(&(hXDesktop->drawArea), &(_PToHXRECTType(hXDesktop->topWin)),sizeof(XRECT)) ==0 /*�����������������һ����*/
			) {
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			_PToHWidgeBaseType(hXDesktop->topWin)->paintFun(hXDesktop->topWin);		/*��ֻ�ػ涥������*/
		}
		else {
#endif
			//fill_rect(0, 0, 1024, 700, 0xffffff);
			_PToHWidgeBaseType(hXDesktop->desktopWin)->paintFun(hXDesktop->desktopWin);	/*�����ػ�*/
#if !USE_ALPHA
		}
#endif
		hXDesktop->drawArea.x = -1;
	}
}



