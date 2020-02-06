
#include "xwindows.h"
#include "x_malloc.h"
#include "msg.h"
#include "paint_cut.h"
#include "paint.h"
#include "gui.h"
static void* WINDOWS_MARK_HEAD(GetWidgeEx)(HWIN hWin, uint16 index);
static void WindowsHeadBtnCallBack(void* Object, void* arg, uint8 status);

HWIN WINDOWS_MARK_HEAD(Create)(char *title,int16 x,int16 y,int16 w,int16 h) {
	/*����һ������*/
	HWIN hWin = (HWIN)(xMalloc(sizeof(WIN)));
	if (hWin == NULL) {
		return NULL;
	}

	/*����ؼ��б�*/
	hWin->widgetList = ListNew();
	if (hWin->widgetList == NULL) {
		xFree( hWin);
	}
	/*����һ������ͷ���ؼ�*/
	hWin->hWinHead = WINDOWS_HEAD_MARK_HEAD(Create)(title, 0, 0, w, 20);
	if (hWin->hWinHead == NULL) { 
		xFree( hWin->widgetList);
		xFree( hWin); 
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hWin, x, y, w, h);

	/*���ûص�����*/
	hWin->winWidge.paintFun = WindowsPaint;
	hWin->winWidge.moveToFun = WindowsMoveTo;
	hWin->winWidge.widgeCallBackFun = WindowsCallBack;
	hWin->winWidge.widgeResizeFun = WindowsResize;

	hWin->winWidge.pencil.DrawColor = RGB565_BLACK;
	hWin->winWidge.pencil.DrawFrColor = _DefaultFrColor;
	hWin->winWidge.pencil.DrawBkColor = RGB565(230,235,230);

	hWin->lastRect.x = x;
	hWin->lastRect.y = y;
	hWin->lastRect.w = w;
	hWin->lastRect.h = h;
	hWin->t_dx = 0;
	hWin->t_dy = 0;

	hWin->flag = 0;
	hWin->winProcessFun = NULL;

	/*���û����ؼ��еĴ��ڱ�ʶΪ��*/
	_SET_IS_WIN(hWin);
	/*������Ҫȫ���ػ�*/
	_SetDrawAllLag(hWin);
	/*������Ϊ����ʾͷ��*/
	_ClrDrawWinHead(hWin);

	/*����ͷ���ؼ�*/
	WindowsWidgeAdd(hWin, hWin->hWinHead);
	/*��С���Ļص�*/
	WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);
	/*��󻯻ص��ӿ�*/
	WINDOWS_HEAD_MARK_HEAD(SetMaximumBtnClickBack)(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);

	{
		HWIDGE_BASE widgeTemp;
		widgeTemp = WidgeCreate(0, 0, 1, hWin->winWidge.rect.h);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		/*��ӱ߿򣬱߿�Ҳ�ǿؼ�*/
		WindowsWidgeAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0 + hWin->winWidge.rect.w - 1, 0, 1, hWin->winWidge.rect.h);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsWidgeAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0,
			0 + hWin->winWidge.rect.h - 1,
			_PToHGroupWidgeType(hWin->hWinHead)->widgeBase.rect.w, 1);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsWidgeAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0,0,
			_PToHGroupWidgeType(hWin->hWinHead)->widgeBase.rect.w, 1);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsWidgeAdd(hWin, widgeTemp);
	}


	/*Ĭ����ʾͷ��*/
	_SetDrawWinHead(hWin);
	return hWin;
}

/*������button����¼��Ļص��ӿں���*/
static void WindowsHeadBtnCallBack(void* Object,void* arg, uint8 status) {
	HWIN CentWIN = (HWIN)arg;
	if (CentWIN == NULL) { return; }
	if (status) {
		if (CentWIN->hWinHead->hXButtonMax == Object) {
			/*������󻯰�ť*/
			WindowsSetMax(CentWIN);
		}
		else if (CentWIN->hWinHead->hXButtonMin == Object) {
			/*������С����ť*/
			WindowsSetMin(CentWIN);
		}
		
	}
}
/*���ô��ڴ���ص��¼�*/
void WINDOWS_MARK_HEAD(SetProcessCallBack)(HWIN hObject, WinProcessCallBack winProcessFun) {
	if (hObject == NULL) { return; }
	hObject->winProcessFun = winProcessFun;
}
/*���ô�����С��*/
void WINDOWS_MARK_HEAD(SetMin)(HWIN hObject) {
	if (!hObject) { return; }
	/*�������ʱ���ܹ���С��*/
	if (_IsMaxWIN(hObject)) { return; }

	if (_IsMinWIN(hObject)) {
		_ClrMinWIN(hObject);
		/*����Ϊ�ϴεĴ�С*/
		WindowsResize(hObject, 
			hObject->winWidge.rect.x, 
			hObject->winWidge.rect.y, 
			hObject->lastRect.w, 
			hObject->lastRect.h
		);
	}
	else {
		_SetMinWIN(hObject);
		/*����֮ǰ�Ŀ��*/
		hObject->lastRect.w = hObject->winWidge.rect.w;
		hObject->lastRect.h = hObject->winWidge.rect.h;
		/*����Ϊ����ͷ�Ĵ�С*/
		WindowsResize(hObject, 
			hObject->winWidge.rect.x, 
			hObject->winWidge.rect.y,
			_PToHGroupWidgeType(hObject->hWinHead)->widgeBase.rect.w, 
			_PToHGroupWidgeType(hObject->hWinHead)->widgeBase.rect.h
		);
	}
}
/*���ô�����󻯣���ʱ��δȫ��ʵ��*/
void WINDOWS_MARK_HEAD(SetMax)(HWIN hObject) {
	if (!hObject) { return; }

	/*��С��ʱ���ܹ����*/
	if (_IsMinWIN(hObject)) {
		return;
	}

	if (_IsMaxWIN(hObject)) {
		_ClrMaxWIN(hObject);
		/*��ԭλ��*/
		//XRECT_COPY((HXRECT)hObject,&(hObject->lastRect));
		WindowsResize(hObject, 
			hObject->lastRect.x, 
			hObject->lastRect.y,
			hObject->lastRect.w, 
			hObject->lastRect.h);
	}
	else {
		_SetMaxWIN(hObject);
		XRECT_COPY(&(hObject->lastRect), &(hObject->winWidge.rect));
		WindowsResize(hObject, 
			((HXRECT)hObject->winWidge.parentHWIN)->x, 
			((HXRECT)hObject->winWidge.parentHWIN)->y,
			((HXRECT)hObject->winWidge.parentHWIN)->w, 
			((HXRECT)hObject->winWidge.parentHWIN)->h
		);
	}
}

/*�������ô��ڵĴ�С,����Ҫ�����ڲ��ؼ��Ĵ�С��λ��*/
void WINDOWS_MARK_HEAD(Resize)(HWIN hObject, int16 x, int16 y, uint16 w, uint16 h) {
	HWIDGE_BASE hWidgeBase;
	HWIDGE_BASE hWidgeBaseHead;
	if (!hObject) { return ; }

	int16 dx, dy;
	/*����λ�øı��ƫ��*/
	dx = x - ((HXRECT)hObject)->x;
	dy = y - ((HXRECT)hObject)->y;

	WIDGE_MARK_HEAD(Resize)((HWIDGE_BASE)hObject, x, y ,w, h);

	hWidgeBaseHead = WindowsGetWidge(hObject, 0);
	if (hWidgeBaseHead == NULL) { return; }
	WINDOWS_HEAD_MARK_HEAD(Resize)(hWidgeBaseHead, x, y, w, 20);

	hWidgeBase = WindowsGetWidge(hObject, 1);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase, x, y, 1, h);

	hWidgeBase = WindowsGetWidge(hObject, 2);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase,x+ w - 1, y, 1, h);

	hWidgeBase = WindowsGetWidge(hObject, 3);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase, x,
		y+ h - 1,
		w, 1);
	hWidgeBase = WindowsGetWidge(hObject, 4);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase, x,
		y ,
		w, 1);
	/*����ÿһ���ؼ�*/
	_StartScanU(WindowsGetWidgeEx(hObject,4)) {//��ʼɨ��
		_PToHWidgeBaseType(val)->widgeResizeFun(
			_PToHWidgeBaseType(val),
			_PToHWidgeBaseType(val)->rect.x + dx,
			_PToHWidgeBaseType(val)->rect.y + dy,
		_PToHWidgeBaseType(val)->rect.w,
		_PToHWidgeBaseType(val)->rect.h);
	}
	/*����ɨ��*/
	_EndScanU();
	/*�ı䴰�ڴ�С����ˢ�¸��ؼ�*/
	WindowsInvaildRect(((HWIDGE_BASE)hObject)->parentHWIN, NULL);
}

/*��ȡ�ؼ�*/
HWIDGE_BASE WINDOWS_MARK_HEAD(GetWidge)(HWIN hObject, uint16 index) {
	if (!hObject) { return NULL; }
	HLIST hlist = ListGet(_PToHGroupWidgeType(hObject)->widgetList, index);
	if (hlist != NULL) {
		return (HWIDGE_BASE)(hlist->val);
	}
	else {
		return NULL;
	}
}
/*��ȡ�����е�ĳ��λ�õĿؼ����õ��Ĳ��Ǵ��ڶ��Ǵ��ڵ�ListItem*/
static void* WINDOWS_MARK_HEAD(GetWidgeEx)(HWIN hWin, uint16 index) {
	if (!hWin) { return NULL; }
	HLIST hlist=ListGet(hWin->widgetList, index);
	return hlist;
}
/*�����������һ���ؼ�*/
int8 WINDOWS_MARK_HEAD(WidgeAdd)(HWIN hWin, void *widge) {
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
		hWidge->rect.y + hWin->winWidge.rect.y + (_IsDrawWinHead(hWin) ? _PToHGroupWidgeType(hWin->hWinHead)->widgeBase.rect.h : 0)
	);
	/*���ø�����*/
	WidgeSetParentWin(hWidge, hWin);

	/* ���ø��´������� */
	//if (hWin->winWidge.parentHWIN == NULL) {
	//	DrawInvaildRect(hWin);
	//}else {
		//DrawInvaildRect(hWin->winWidge.parentHWIN);
	//}
	WindowsInvaildRect((HWIDGE_BASE)(hWin->winWidge.parentHWIN), (HXRECT)(&(hWin->winWidge)));
	return 0;
}
/*���ô��ڵ���Ч���򣬵���gui����Ч�������ú���*/
void WINDOWS_MARK_HEAD(InvaildRect)(HWIDGE_BASE hWidgeBase,HXRECT hXRect) {
	if (!hWidgeBase) { return; }
	if (hXRect == NULL) {
		GUISendDrawMsg(hWidgeBase, MSG_WIN_INVAILD_UPDATE, 0,
			hWidgeBase->rect.x, hWidgeBase->rect.y, hWidgeBase->rect.w, hWidgeBase->rect.h
		);
	}
	else {
		GUISendDrawMsg(hWidgeBase, MSG_WIN_INVAILD_UPDATE, 0,
			hXRect->x, hXRect->y, hXRect->w, hXRect->h
		);
	}
}
/*
* �رմ���
*/
void WINDOWS_MARK_HEAD(Close)(HWIN hWin) {
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
	WIDGE_MARK_HEAD(Close)((HWIDGE_BASE)hWin);
}
/*�ƶ�����*/
void WINDOWS_MARK_HEAD(MoveTo)(HWIN hWin, int16 x, int16 y) {
	int16 dx;
	int16 dy;
	XRECT lastRect;
	if (!hWin) { return; }
	/*��λ���������һ���ƶ���ƫ����*/
	dx = x - hWin->winWidge.rect.x;
	dy = y - hWin->winWidge.rect.y;

	/*λ��û�иı�*/
	if (x == hWin->winWidge.rect.x 
		&& y == hWin->winWidge.rect.y) {
		return;
	}
	XRECT_COPY(&lastRect, (HXRECT)hWin);
	/*�洢��һ�ε�xy*/
	hWin->lastRect.x = hWin->winWidge.rect.x;
	hWin->lastRect.y = hWin->winWidge.rect.y;
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

	/* ���´��ڣ����µ�λ������һ�ε�λ��*/
	WindowsInvaildRect((HWIDGE_BASE)(hWin->winWidge.parentHWIN), (HXRECT)(&(lastRect)));
}
/*���ô�����ɫ*/
void WINDOWS_MARK_HEAD(SetColor)(HWIN hWin, uintColor color) {
	if (!hWin) { return; }
	hWin->winWidge.pencil.DrawBkColor = color;

	/* ���´��� */
	//if (hWin->winWidge.parentHWIN == NULL) {
	//	WindowsInvaildRect(hWin, hWin);
	//}
	//else {
		WindowsInvaildRect((HWIDGE_BASE)hWin, (HXRECT)hWin);
	//}
}

/*�����Ƿ���Ч*/
void WINDOWS_MARK_HEAD(SetVisable)(void* hObject, int8 isVisable) {
	HWIN hWin;
	hWin = hObject;
	if (!hWin) { return; }
	WIDGE_MARK_HEAD(SetVisable)((HWIDGE_BASE)hWin, isVisable);
}
/*
*���»��ƴ���
*/
void WINDOWS_MARK_HEAD(Paint)(void *hObject) {
	HLIST cutPostionList;
	HWIN hWin;
	hWin = hObject;
	if (!hWin) { return; }

	if (_IsDrawAllLag(hWin)) {
		/*�״εĴ����¼�*/
		if (hWin->winProcessFun != NULL) {
			MSGE msg;
			msg.msgSrc = hWin;
			msg.msgType = MSG_WIN;
			msg.msgID = MSG_WIN_CREATE;
			hWin->winProcessFun(hWin, hWin->winWidge.arg, &msg);
		}
	}
	/*�Ƿ���Ҫ��ʾ*/
	if (!_GetVisable(hWin)) { return; }
	/*�Ƿ���Ҫ����*/
	//if (!IsGUINeedCut((HXRECT)hWin) && !_IsDrawAllLag(hWin)) { return; }
	if (!IsGUINeedCutEx(hWin) && !_IsDrawAllLag(hWin)) { return; }

	/*������Ҫȫ��ˢ��*/
	_ClrDrawAllLag(hWin);

	/*���õ�ǰ��ͼ����*/
	if (!DrawSetArea(hWin)) { return; }
	/*�����Ƿ�����˴���߿������ü�����*/
	if (_IsDrawWinHead(hWin)) {
		/*�л�����ǰ������*/
		cutPostionList=RectCutAddRectList(hWin->widgetList->next);
	}
	else {
		cutPostionList = RectCutAddRectList(WindowsGetWidgeEx(hWin,5));
	}

	hWin->winWidge.pencil.DrawColor = hWin->winWidge.pencil.DrawBkColor;
	/*������*/
	DrawCutRect(hWin, &(hWin->winWidge.rect));
	/*�ػ��¼�*/
	if (hWin->winProcessFun != NULL) {
		MSGE msg;
		msg.msgSrc = hWin;
		msg.msgType = MSG_WIN;
		msg.msgID = MSG_WIN_PAINT;
		hWin->winProcessFun(hWin, hWin->winWidge.arg, &msg);
	}
	RectCutSplitRectList(cutPostionList);
	/*����ÿһ���ؼ�*/
	_StartScanU(hWin->widgetList) {//��ʼɨ��
		/*��ǰ�б�ؼ����������*/
		cutPostionList = RectCutAddRectList(tempItem->next);
		/*���ƿؼ�*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));
		/*�˻ؼ�����*/
		RectCutSplitRectList(cutPostionList);
		/*�ָ���ͼ����*/
		DrawResetArea(hWin);
	}
	/*����ɨ��*/
	_EndScanU();

}
/*�Ƿ���ʾͷ��*/
void WINDOWS_MARK_HEAD(SetDrawHead)(HWIN hWin, int8 isEnable) {
	uint8 i = 0;

	if (isEnable) {
		_SetDrawWinHead(hWin);
	}
	else {
		_ClrDrawWinHead(hWin);
	}

	/*ǰ�ĸ��ؼ�Ϊ�߽��ͷ��*/
	for (i = 0; i < 5; i++) {
		HLIST widge= WindowsGetWidgeEx(hWin, i);
		if (widge != NULL) {
			if (isEnable) {
				_SetVisable(widge->val);
			}
			else {
				_ClrVisable(widge->val);
			}
		}
	}
	/* ���´��� */
	WindowsInvaildRect((HWIDGE_BASE)hWin, (HXRECT)hWin);
}

/*�����¼�����*/
int8 WINDOWS_MARK_HEAD(CallBack)(void* hObject,HMSGE hMsg) {
	HWIN hWin = hObject;
	if (!hWin || !hMsg) { return RES_ASSERT_ERR; }
	if (!_GetVisable(hWin)) { return RES_ASSERT_ERR; }

	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawWinHead(hWin)) {
			switch (hMsg->msgID) {
			case MSG_TOUCH_PRESS:
				//��ͷ����ץȡ�ƶ�
					/*��ץ���ǲ���ͷ*/
					if (WINDOWS_HEAD_MARK_HEAD(CallBack)(hWin->hWinHead, hMsg) == RES_OK) {
						hWin->t_dx = hMsg->msgVal.rect.x - hWin->winWidge.rect.x;
						hWin->t_dy = hMsg->msgVal.rect.y - hWin->winWidge.rect.y;
						_SetWinMoveing(hWin);
						WindowsInvaildRect((HWIDGE_BASE)hWin, NULL);
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
				if (_IsWinMoving(hWin)) {
					//��ͷ����ץȡ�ƶ�
					_ClrWinMoveing(hWin);
					//return 0;
				}
				break;
			}
		}

		switch (hMsg->msgID) {
		case MSG_TOUCH_PRESS:
			if (
				hMsg->msgVal.rect.x >= hWin->winWidge.rect.x + hWin->winWidge.rect.w-2
				&& hMsg->msgVal.rect.x <= hWin->winWidge.rect.x + hWin->winWidge.rect.w 
				) {
				hWin->t_xy = hMsg->msgVal.rect.x;
				_SetChangeWinRHSize(hWin);
				return 0;
			}
			else if (
				hMsg->msgVal.rect.y >= hWin->winWidge.rect.y + hWin->winWidge.rect.h - 2
				&& hMsg->msgVal.rect.y <= hWin->winWidge.rect.y + hWin->winWidge.rect.h
				) {
				hWin->t_xy = hMsg->msgVal.rect.y;
				_SetChangeWinVSize(hWin);
				return 0;
			}
			else if (
				hMsg->msgVal.rect.x >= hWin->winWidge.rect.x 
				&& hMsg->msgVal.rect.x <= hWin->winWidge.rect.x + 2
				) {
				hWin->t_xy = hMsg->msgVal.rect.x;
				_SetChangeWinLHSize(hWin);
				return 0;
			}
			break;
		case MSG_TOUCH_MOVE:
			if (_IsChangeWinRHSize(hWin)) {
				hWin->winWidge.widgeResizeFun(hWin, ((HXRECT)hWin)->x, ((HXRECT)hWin)->y,
					((HXRECT)hWin)->w + (hMsg->msgVal.rect.x - hWin->t_xy), ((HXRECT)hWin)->h);
				hWin->t_xy = hMsg->msgVal.rect.x;
				return 0;
			}else if (_IsChangeWinVSize(hWin)) {
				hWin->winWidge.widgeResizeFun(hWin, ((HXRECT)hWin)->x, ((HXRECT)hWin)->y,
					((HXRECT)hWin)->w, (((HXRECT)hWin)->h + (hMsg->msgVal.rect.y - hWin->t_xy)));
				hWin->t_xy = hMsg->msgVal.rect.y;
				return 0;
			}else if (_IsChangeWinLHSize(hWin)) {
				hWin->winWidge.widgeResizeFun(hWin,
					((HXRECT)hWin)->x + (hMsg->msgVal.rect.x - hWin->t_xy),
					((HXRECT)hWin)->y,
					((HXRECT)hWin)->w - (hMsg->msgVal.rect.x - hWin->t_xy),
					((HXRECT)hWin)->h);
				hWin->t_xy = hMsg->msgVal.rect.x;
				return 0;
			}
			break;
		case MSG_TOUCH_RELEASE:
			if (_IsChangeWinRHSize(hWin)) {
				_ClrChangeWinRHSize(hWin);
				return 0;
			}
			if (_IsChangeWinVSize(hWin)) {
				_ClrChangeWinVSize(hWin);
				return 0;
			}
			if (_IsChangeWinLHSize(hWin)) {
				_ClrChangeWinLHSize(hWin);
				return 0;
			}
			//
			break;
		}
		if (!_IsDrawCheckPoint(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			hWin->winWidge.rect.x, hWin->winWidge.rect.y, 
			hWin->winWidge.rect.w, hWin->winWidge.rect.h)) {
			return 2;
		}

		HLIST hWidgeList = hWin->widgetList;
		if (hWidgeList == NULL) { return RES_FAIL; }

		if (hWidgeList) {
			int8 ret;
			HLIST lastWidge = ListGetLast(hWidgeList);
			if (lastWidge != NULL) {
				while (lastWidge != hWidgeList) {
					HWIDGE_BASE hWidge = (HWIDGE_BASE)(lastWidge->val);

					if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == RES_OK) {
						return RES_OK;
					}

					lastWidge = lastWidge->lnext;
				}
			}
		}
		return RES_FAIL;
	}
	else if (hMsg->msgType == MSG_KEY) {
		if (hWin->winProcessFun != NULL) {
			hWin->winProcessFun(hWin, hWin->winWidge.arg, hMsg);
		}
		return RES_FAIL;
	}

	return RES_FAIL;
}



