
#include "xwindows.h"
#include "x_malloc.h"
#include "msg.h"
#include "paint_cut.h"
#include "paint.h"
#include "gui.h"


HWIN WindowsCreate(char *title,int16 x,int16 y,int16 w,int16 h) {
	/*����һ������*/
	HWIN hWin = (HWIN)(xMalloc(sizeof(WIN)));
	if (hWin == NULL) {
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hWin, x, y, w, h);
	/*����ؼ��б�*/
	hWin->widgetList = ListNew();
	if (hWin->widgetList == NULL) {
		xFree( hWin);
	}
	/*����һ������ͷ���ؼ�*/
	hWin->hWinHead = WindowsHeadCreate(title, 0, 0, w, 0);
	if (hWin->hWinHead == NULL) { 
		xFree( hWin->widgetList);
		xFree( hWin); 
		return NULL;
	}
	
	/*���ô��ڳ�ֵ*/
	hWin->winWidge.rect.x = x;
	hWin->winWidge.rect.y = y;
	hWin->winWidge.rect.w = w;
	hWin->winWidge.rect.h = h;

	/*���ûص�����*/
	hWin->winWidge.paintFun = WindowsPaint;
	hWin->winWidge.moveToFun = WindowsMoveTo;
	hWin->winWidge.widgeCallBackFun = WindowsCallBack;

	/*���û�����ɫ*/
	hWin->winWidge.pencil.DrawColor = _DefaultFrColor;
	/*���ô�����Ч����*/
	hWin->winWidge.pencil.x = x;
	hWin->winWidge.pencil.y = y;
	hWin->winWidge.pencil.w = w;
	hWin->winWidge.pencil.h = h;

	hWin->winWidge.isVisable = TRUE;
	hWin->winWidge.parentHWIN = NULL;

	hWin->lastRect.x = x;
	hWin->lastRect.y = y;
	hWin->lastRect.w = w;
	hWin->lastRect.h = h;
	hWin->t_dx = 0;
	hWin->t_dy = 0;

	/*���û����ؼ��еĴ��ڱ�ʶΪ��*/
	_SET_IS_WIN(hWin);

	/*������Ҫȫ���ػ�*/
	_SetDrawAllLag(hWin);

	/*������Ϊ����ʾͷ��*/
	_ClrDrawWinHead(hWin);
	/*����һ���ؼ�*/
	WindowsWidgeAdd(hWin, hWin->hWinHead);
	/*Ĭ����ʾͷ��*/
	_SetDrawWinHead(hWin);
	{
		HWIDGE_BASE widgeTemp;
		widgeTemp = WidgeCreate(0, 0, 1, hWin->winWidge.rect.h);
		/*��ӱ߿򣬱߿�Ҳ�ǿؼ�*/
		WindowsWidgeAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0 + hWin->winWidge.rect.w - 1, 0, 1, hWin->winWidge.rect.h);
		WindowsWidgeAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0, 
			0 + hWin->winWidge.rect.h - hWin->hWinHead->headWidge.rect.h - 1,
			hWin->hWinHead->headWidge.rect.w, 1);
		WindowsWidgeAdd(hWin, widgeTemp);
	}
	

	return hWin;
}

/*��ȡ�����е�ĳ��λ�õĿؼ����õ��Ĳ��Ǵ��ڶ��Ǵ��ڵ�ListItem*/
void* WindoswGetWidge(HWIN hWin, uint16 index) {
	if (!hWin) { return NULL; }
	HLIST hlist=ListGet(hWin->widgetList, index);
	return hlist;
}

/*�����������һ���ؼ�*/
int8 WindowsWidgeAdd(HWIN hWin, void *widge) {
	if (!hWin|| !widge) { return -1; }
	HLIST addItem = ListNew();
	if (!addItem) { return -1; }
	addItem->val = widge;
	
	if (ListAddLast(hWin->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGE_BASE hWidge = widge;

	hWidge->moveToFun(
		hWidge, 
		hWidge->rect.x + hWin->winWidge.rect.x,
		hWidge->rect.y + hWin->winWidge.rect.y + (_IsDrawWinHead(hWin) ? hWin->hWinHead->headWidge.rect.h : 0)
	);
	/*���ø�����*/
	WidgeSetParentWin(hWidge, hWin);

	/* ���ø��´������� */
	//if (hWin->winWidge.parentHWIN == NULL) {
	//	DrawInvaildRect(hWin);
	//}else {
		//DrawInvaildRect(hWin->winWidge.parentHWIN);
	//}
	WindowsInvaildRect(hWin, hWidge);
	return 0;
}
/*���ô��ڵ���Ч���򣬵���gui����Ч�������ú���*/
void WindowsInvaildRect(HWIN hWin,HXRECT hXRect) {
	if (!hWin) { return; }
	if (hXRect == NULL) {
		//DrawInvaildRect((HXRECT)hWin);
		GUISendDrawMsg(hWin, MSG_WIN_INVAILD_UPDATE, 0, 
		  hWin->winWidge.rect.x, hWin->winWidge.rect.y, hWin->winWidge.rect.w, hWin->winWidge.rect.h
		//, hWin->lastRect.x, hWin->lastRect.y, hWin->lastRect.w, hWin->lastRect.h
		);
	}
	else {
		//DrawInvaildRect(hXRect);
		GUISendDrawMsg(hWin, MSG_WIN_INVAILD_UPDATE, 0,
			hXRect->x, hXRect->y, hXRect->w, hXRect->h
		//	, hWin->lastRect.x, hWin->lastRect.y, hWin->lastRect.w, hWin->lastRect.h
		);
	}
}
/*
* �رմ���
*/
void WindowsClose(HWIN hWin) {
	if (!hWin) { return; }
	/*�������ͷŴ����е��ڴ�*/
	/*��ȡÿһ���ؼ�*/
	HLIST hWidgeList = hWin->widgetList;
	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);
			/*����ÿһ���ؼ��Ĺرպ���*/
			hWidge->widgeCloseFun(hWidge);

			hWidgeList = hWidgeList->next;
		}
	}
	WIDGE_MARK_HEAD(Close)(hWin);
}
/*�ƶ�����*/
void WindowsMoveTo(HWIN hWin, int16 x, int16 y) {
	int16 dx;
	int16 dy;
	if (!hWin) { return; }
	/*��λ���������һ���ƶ���ƫ����*/
	dx = x - hWin->winWidge.rect.x;
	dy = y - hWin->winWidge.rect.y;

	/*λ��û�иı�*/
	if (x == hWin->winWidge.rect.x 
		&& y == hWin->winWidge.rect.y) {
		return;
	}

	/*��������Ϊ��һ�ε�*/
	hWin->lastRect.x = hWin->winWidge.rect.x;
	hWin->lastRect.y = hWin->winWidge.rect.y;
	hWin->lastRect.w = hWin->winWidge.rect.w;
	hWin->lastRect.h = hWin->winWidge.rect.h;
	/*�����µ�λ��*/
	hWin->winWidge.rect.x = x;
	hWin->winWidge.rect.y = y;

	//��������ÿһ���ؼ�λ��
	HLIST hWidgeList = hWin->widgetList;

	/*�ƶ��ڲ���ÿһ���ؼ�*/
	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);	
			/*���ڲ�ÿһ���ؼ�����ƫ��*/
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);
			hWidgeList = hWidgeList->next;
		}
	}

	/* ���´��� */
	if (hWin->winWidge.parentHWIN == NULL) {
		WindowsInvaildRect(hWin, NULL);
	}
	else {
		WindowsInvaildRect(hWin->winWidge.parentHWIN, &(hWin->lastRect) );
	}
	/*���õ�ǰ����Ϊ�ƶ�����*/
	//setMovingWin(hWin);

}
/*���ô�����ɫ*/
void WindowsSetColor(HWIN hWin, uintColor color) {
	if (!hWin) { return; }
	hWin->winWidge.pencil.DrawColor = color;

	/* ���´��� */
	//if (hWin->winWidge.parentHWIN == NULL) {
	//	WindowsInvaildRect(hWin, hWin);
	//}
	//else {
		WindowsInvaildRect(hWin, hWin);
	//}
}

/*�����Ƿ���Ч*/
void WindowsSetVisable(void* hObject, int8 isVisable) {
	HWIN hWin;
	hWin = hObject;
	if (!hWin) { return; }
	hWin->winWidge.isVisable = isVisable;

	/* ���´��� */
	//if (hWin->winWidge.parentHWIN == NULL) {
	//	WindowsInvaildRect(hWin, hWin);
	//}
	//else {
		WindowsInvaildRect(hWin, hWin);
	//}
}
/*
*���»��ƴ���
*/
void WindowsPaint(void *hObject) {
	HLIST cutPostionList;
	HWIN hWin;
	hWin = hObject;
	if (!hWin) { return; }
	if(!(hWin->winWidge.isVisable)){return ;}
	if (!isGUINeedCut(hWin) 
		&& !_IsDrawAllLag(hWin)
		) { return; }
	/*������Ҫȫ��ˢ��*/
	_ClrDrawAllLag(hWin);

	/*���õ�ǰ��ͼ����*/
	DrawSetArea(hWin);
	/*�����Ƿ�����˴���߿������ü�����*/
	if (_IsDrawWinHead(hWin)) {
		/*�л�����ǰ������*/
		cutPostionList=RectCutAddRectList(hWin->widgetList->next);
	}
	else {
		cutPostionList = RectCutAddRectList(WindoswGetWidge(hWin,4));
	}
	/*������*/
	DrawCutRect(hWin, &(hWin->winWidge.rect));
	RectCutSplitRectList(cutPostionList);
	/*�˻ص�֮ǰ������*/
	//if (_IsDrawWinHead(hWin)) {
	//	//RectCutSplitRectList(hWin->widgetList->next);
	//}
	//else {
	//	//RectCutSplitRectList(WindoswGetWidge(hWin, 4));
	//}

	/*����ÿһ���ؼ�*/
	_StartScanU(hWin->widgetList) {//��ʼɨ��
		/*��ǰ�б�ؼ����������*/
		cutPostionList = RectCutAddRectList(tempItem->next);
		/*���ƿؼ�*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));
		/*�˻ؼ�����*/
		//RectCutSplitRectList(tempItem->next);
		RectCutSplitRectList(cutPostionList);
		/*�ָ���ͼ����*/
		DrawResetArea(hWin);
	}
	/*����ɨ��*/
	_EndScanU();

}
/*�Ƿ���ʾͷ��*/
void WindowsSetDrawHead(HWIN hWin, int8 isEnable) {
	uint8 i = 0;
	if (isEnable) {
		_SetDrawWinHead(hWin);
	}
	else {
		_ClrDrawWinHead(hWin);
	}

	/*ǰ�ĸ��ؼ�Ϊ�߽��ͷ��*/
	for (i = 0; i < 4; i++) {
		HLIST widge= WindoswGetWidge(hWin, i);
		if (widge != NULL) {
			((HWIDGE_BASE)(widge->val))->isVisable= isEnable;
		}
	}

	/* ���´��� */
	//if (hWin->winWidge.parentHWIN == NULL) {
	//	WindowsInvaildRect(hWin, hWin);
	//}
	//else {
		WindowsInvaildRect(hWin, hWin);
	//}
}
/*�����¼�����*/
int8 WindowsCallBack(void* hObject,HMSGE hMsg) {
	HWIN hWin = hObject;
	if (!hWin || !hMsg) { return -1; }
	if (!(hWin->winWidge.isVisable)) { return -1; }

	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawWinHead(hWin)) {
			switch (hMsg->msgID) {
			case MSG_TOUCH_PRESS:
				//��ͷ����ץȡ�ƶ�
					//��ץ���ǲ���ͷ
					if (WindowsHeadsCallBack(hWin->hWinHead, hMsg) == 0) {
						hWin->t_dx = hMsg->msgVal.rect.x - hWin->winWidge.rect.x;
						hWin->t_dy = hMsg->msgVal.rect.y - hWin->winWidge.rect.y;
						_SetWinMoveing(hWin);
						WindowsInvaildRect(hWin, hWin);
						return 0;
					}
				break;
			case MSG_TOUCH_MOVE:
				//��ͷ����ץȡ�ƶ�
					if (_IsWinMoving(hWin)) {
						/*WindowsMoveTo(hWin, 
							hMsg->msgVal.xy.x - hWin->t_dx, hMsg->msgVal.xy.y - hWin->t_dy);*/
						GUISendMoveMsg(hWin, MSG_WIN, MSG_WIN_MOVE, hMsg->msgVal.rect.x - hWin->t_dx, hMsg->msgVal.rect.y - hWin->t_dy);
						return 0;
					}
				break;
			case MSG_TOUCH_RELEASE:
				//��ͷ����ץȡ�ƶ�
				_ClrWinMoveing(hWin);
				//return 0;
				break;
			}
		}
		if (!_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			hWin->winWidge.rect.x, hWin->winWidge.rect.y, 
			hWin->winWidge.rect.w, hWin->winWidge.rect.h)) {
			return 2;
		}
		//if (hMsg->msgID == MSG_TOUCH_PRESS) {
		//	DrawInvaildRect(_PToHXRECTType(hWin));
		//}
		HLIST hWidgeList = hWin->widgetList;
		if (hWidgeList == NULL) { return 1; }
		//if (_IsDrawWinHead(hWin)) {
		//	hWidgeList = hWidgeList->next;/*����ͷ��*/
		//}

		//_StartScanU(hWidgeList)
		//	HWIDGE_BASE hWidge = (HWIDGE_BASE)val;
		//	if (hWidge->widgeCallBackFun(hWidge, hMsg) == 1) {
		//		
		//		break;
		//	}
		//_EndScanU();

		if (hWidgeList) {
			int8 ret;
			HLIST lastWidge = ListGetLast(hWidgeList);
			if (lastWidge != NULL) {
				while (lastWidge != hWidgeList) {
					HWIDGE_BASE hWidge = (HWIDGE_BASE)(lastWidge->val);

					if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == 0) {
						return 0;
					}

					lastWidge = lastWidge->lnext;
				}
			}
		}
		return 1;
	}

	return 1;
}



