
#include "gui.h"
#include "list.h"
#include "paint_cut.h"
#include "x_malloc.h"
#include "xwindows.h"
#include "GUI_timeout.h"
#include <string.h>


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

	WindowsInvaildRect(WIDGE_PARENT(hXDesktop->topWin), _PToHXRECTType(hXDesktop->topWin));
	/*GUISendDrawMsg(hXDesktop->topWin, MSG_WIN_INVAILD_UPDATE, 0,
		WIDGE_X(hXDesktop->topWin), WIDGE_Y(hXDesktop->topWin),
		WIDGE_W(hXDesktop->topWin), WIDGE_H(hXDesktop->topWin)
	);*/
}

/*GUI�¼�����*/
void GUIEvent(void) {
	HMSGE hTempMsg;

	GUITimeoutProcess();

	/*�����¼��������¼�*/
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
				}
			}
		}

		else if (hTempMsg->msgType == MSG_KEY) {
			if (hXDesktop->topWin != NULL) {
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
	/*ˢ���¼�*/
	while ((hTempMsg = GUIGetDrawMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_WIN_INVAILD_UPDATE) {
			//XRECT hRect;
			//hRect.x = ((HXRECT)(hTempMsg->msgSrc))->x;
			//hRect.y = ((HXRECT)(hTempMsg->msgSrc))->y;
			//hRect.w = ((HXRECT)(hTempMsg->msgSrc))->w;
			//hRect.h = ((HXRECT)(hTempMsg->msgSrc))->h;
			//_DrawInvaildRect(&hRect);
			//HXRECT srcWidge = ((HXRECT)(&(hTempMsg->msgVal.rect)));
			//SetMovingRect(srcWidge);
			GUIUpdate(hTempMsg->msgSrc, (HXRECT)(&(hTempMsg->msgVal.rect)));
		}
		GUIDelDrawMsg(hTempMsg);
	}
	while ((hTempMsg = GUIGetWINDelMsg()) != NULL) {
		if (hTempMsg->msgType == MSG_WIN) {
			if (hTempMsg->msgID == MSG_WIN_DEL_WIDGE) {/*ɾ������*/
				GROUP_MARK_HEAD(Del)(hTempMsg->msgSrc, hTempMsg->msgVal.v);
			}
		}
		GUIDelWinDelMsg(hTempMsg);
	}
	
	

}
void GUIUpdate(HWIDGE_BASE hWidgeBase, HXRECT hXRect) {
#if USE_MEM_DEV
	int16 i,j;
	XRECT rRect;
	XRECT oldDrawRect;
#endif
	SetUpdateArea(hWidgeBase, hXRect);
#if USE_MEM_DEV
	if (MemDevReset(hXDesktop->hMemDev, DESKTOP_DRAW_AREA(hXDesktop)->x, DESKTOP_DRAW_AREA(hXDesktop)->y,
		DESKTOP_DRAW_AREA(hXDesktop)->w, DESKTOP_DRAW_AREA(hXDesktop)->h) == FALSE) {
		return;
	}
	/*����֮ǰ��*/
	XRECT_COPY(&oldDrawRect, DESKTOP_DRAW_AREA(hXDesktop));
	for (i = hXRect->y; i < hXRect->y+ hXRect->h; i += hXDesktop->hMemDev->rect.h) {
		/*��洢�豸������㣬�õ��ཻ����*/
		GetOverLapRect(DESKTOP_DRAW_AREA(hXDesktop), &(hXDesktop->hMemDev->rect), &rRect);
		if (rRect.w == 0 || rRect.h==0) { 
			hXDesktop->hMemDev->rect.y += hXDesktop->hMemDev->rect.h;
			XRECT_COPY(DESKTOP_DRAW_AREA(hXDesktop), &oldDrawRect);
			continue; 
		}
		XRECT_COPY(DESKTOP_DRAW_AREA(hXDesktop), &rRect);
		MemDevReset(hXDesktop->hMemDev, rRect.x, rRect.y,
			rRect.w, rRect.h);

		MemDevClear(hXDesktop->hMemDev, 0xffff);
#endif
		GUIExec();
#if USE_MEM_DEV
		MemDevCopyToLCD(hXDesktop->hMemDev);
		hXDesktop->hMemDev->rect.y += hXDesktop->hMemDev->rect.h;
		XRECT_COPY(DESKTOP_DRAW_AREA(hXDesktop), &oldDrawRect);
		
	}
	XRECT_COPY(DESKTOP_DRAW_AREA(hXDesktop) , &oldDrawRect);
#endif
}
/*���ø�������*/
void SetUpdateArea(HWIDGE_BASE hWidgeBase,HXRECT hXRect) {
	XRECT rRect;

	hXDesktop->updateParentWidge = hWidgeBase;
	/*��ͼ����������ȡ�ཻ��Ȼ��ֵ������Pencil*/
	GetOverLapRect((HXRECT)(&(hXDesktop->desktopWin->groupWidge)), hXRect, &rRect);

	/*�������ÿؼ�����Ч��ͼ����*/
	XRECT_COPY(&(hXDesktop->desktopWin->groupWidge.widgeBase.pencil), &rRect);

	/*���ø�������*/
	XRECT_COPY(&(hXDesktop->updateArea), hXRect);
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
#if USE_MEM_DEV
	hXDesktop->hMemDev = MemDevCreate(0, 0, LCD_SCREEN_W, LCD_SCREEN_H*0.1, GUI_COLOR_DEEP);/*����һ���洢�豸*/
#endif

	/*��������Ĭ����ɫ*/
	WindowsSetColor(hXDesktop->desktopWin, RGB565(0,128,230));
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

/*��ǰ�Ĵ����Ƿ���Ҫ����
	TRUE:��Ҫ����
*/
BOOL IsGUINeedCut(HWIDGE_BASE hWidgeBase) {
	HXRECT hXRECT;
	uint8 flag = 0;
	if (hWidgeBase == NULL) { return TRUE; }
	hXRECT = (HXRECT)(hWidgeBase);
#if USE_ALPHA
	/*���͸������Ҫ���ã�Ҳ������Ҫ����*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}
	///*�����ǰ����ʱ�ƶ�������ˢ��*/
	//if (memcmp(hXRECT ,&(hXDesktop->movingRect),sizeof(XRECT)) == 0) {
	//	return TRUE;
	//}
	/*�����ǰ���ں��ƶ����ڲ�������ײ��ˢ��*/
	if (_IsDrawCheckArea(hXDesktop->updateArea.x
		, hXDesktop->updateArea.y
		, hXDesktop->updateArea.w
		, hXDesktop->updateArea.h
		, hXRECT->x
		, hXRECT->y
		, hXRECT->w
		, hXRECT->h
	)) {
		return TRUE;
	}
	if (WIDGE_PARENT(hWidgeBase) != NULL) {/*�и����ڲ��ܲ���*/
		/*�����ǰ����������͸��������ˢ��*/
		_StartScanU((_PToHGroupWidgeType((WIDGE_PARENT(hWidgeBase))))->widgetList)
			HXRECT hItemRect = (HXRECT)(val);
		if (flag == 1)
		{
			if (_GET_IS_DPY(hItemRect)) {
				if (_IsDrawCheckArea(
					hItemRect->x
					, hItemRect->y
					, hItemRect->w
					, hItemRect->h
					, hXRECT->x
					, hXRECT->y
					, hXRECT->w
					, hXRECT->h
				)) {
					return TRUE;
				}
			}
		}
		if (tempItem->val == hWidgeBase) {/**/
			flag = 1;
		}
		_EndScanU()
	}

	return FALSE;
#else
	/*���͸������Ҫ���ã�Ҳ������Ҫ����*/
	if (_GET_IS_DPY(hXRECT)) {
		return TRUE;
	}

	//if (memcmp(hXRECT, &(hXDesktop->movingRect), sizeof(XRECT)) == 0) {
	//	return TRUE;
	//}
	return _IsDrawCheckArea(
		hXDesktop->updateArea.x
		, hXDesktop->updateArea.y
		, hXDesktop->updateArea.w
		, hXDesktop->updateArea.h
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
	if (!hWidgeBase ) { return FALSE; }
	if (!WIDGE_PARENT(hWidgeBase)) {/*���û�и��ؼ�����ǰ�ؼ��Ļ�ͼ������ڿؼ���С*/
/*		XRECT_COPY(&(hWidgeBase->pencil), hWidgeBase)*/;
		return TRUE;
	}

	/*�ؼ������븸�ؼ��Ŀɻ��������ཻ���μ�Ϊ��ͼ����*/
	GetOverLapRect((HXRECT)(&(((HWIDGE_BASE)(hWidgeBase->parentHWIN))->pencil)),&(hWidgeBase->rect),&rRect);

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
	//XRECT_COPY(_PToHXRECTType(&(hWidgeBase->pencil)) ,hWidgeBase);
}
//GUIִ�к���
void GUIExec(void) {
#if !USE_ALPHA
	if (!_GET_IS_DPY(hXDesktop->updateParentWidge) && hXDesktop->updateParentWidge == (HWIDGE_BASE)(hXDesktop->topWin)) {/*����Ƕ��㴰����ֱ�Ӹ��¶��㴰��*/
		//XRECT hRect;
		//hRect.x = 0;
		//hRect.y = 0;
		//hRect.w = 800;
		//hRect.h = 480;
		//RawDrawRect(&hRect, 0xffff);
		hXDesktop->updateParentWidge->paintFun(hXDesktop->updateParentWidge);
	}
	else 
#endif
	{
//XRECT hRect;
//hRect.x = 0;
//hRect.y = 0;
//hRect.w = 800;
//hRect.h = 480;
//RawDrawRect(&hRect, 0xffff);
		_PToHWidgeBaseType(hXDesktop->desktopWin)->paintFun(hXDesktop->desktopWin);	/*�����ػ�*/
	}
}



