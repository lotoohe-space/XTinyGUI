
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
	if (GROUP_MARK_HEAD(Init)(_PToHGroupWidgeType(hWin), x, y, w, h) == FALSE) {
		return NULL;
	}
	/*����һ������ͷ���ؼ�*/
	hWin->hWinHead = WINDOWS_HEAD_MARK_HEAD(Create)(title, 0, 0, w, 20);
	if (hWin->hWinHead == NULL) { 
		//xFree( hWin->widgetList);
		_PToHGroupWidgeType(hWin)->widgeBase.widgeCloseFun(_PToHGroupWidgeType(hWin));
		xFree( hWin); 
		return NULL;
	}
	WidgeInit((HWIDGE_BASE)hWin, x, y, w, h);

	/*���ûص�����*/
	_PToHWidgeBaseType(hWin)->paintFun = WINDOWS_MARK_HEAD(Paint);
	_PToHWidgeBaseType(hWin)->moveToFun = WINDOWS_MARK_HEAD(MoveTo);
	_PToHWidgeBaseType(hWin)->widgeCallBackFun = WINDOWS_MARK_HEAD(CallBack);
	_PToHWidgeBaseType(hWin)->widgeResizeFun = WINDOWS_MARK_HEAD(Resize);
	_PToHWidgeBaseType(hWin)->widgeCloseFun = WINDOWS_MARK_HEAD(Close);
		
	_PToHWidgeBaseType(hWin)->pencil.DrawColor = RGB565_BLACK;
	_PToHWidgeBaseType(hWin)->pencil.DrawFrColor = _DefaultFrColor;
	_PToHWidgeBaseType(hWin)->pencil.DrawBkColor = RGB565(230,235,230);

	hWin->lastRect.x = x;
	hWin->lastRect.y = y;
	hWin->lastRect.w = w;
	hWin->lastRect.h = h;
	hWin->t_dx = 0;
	hWin->t_dy = 0;

	hWin->flag = 0;
	/*Windows�¼�����ص�����*/
	hWin->winProcessFun = NULL;

	/*���û����ؼ��еĴ��ڱ�ʶΪ��*/
	_SET_IS_WIN(hWin);
	/*������Ҫȫ���ػ�*/
	_SetDrawAllLag(hWin);
	/*������Ϊ����ʾͷ��*/
	_ClrDrawWinHead(hWin);

	/*����ͷ���ؼ�*/
	WindowsAdd(hWin, hWin->hWinHead);
	/*��С���Ļص�*/
	WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);
	/*��󻯻ص��ӿ�*/
	WINDOWS_HEAD_MARK_HEAD(SetMaximumBtnClickBack)(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);
	WINDOWS_HEAD_MARK_HEAD(SetCloseBtnClickBack)(hWin->hWinHead, hWin, WindowsHeadBtnCallBack);

	{
		HWIDGE_BASE widgeTemp;
		widgeTemp = WidgeCreate(0, 0, 1, WIDGE_H(hWin));
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		/*��ӱ߿򣬱߿�Ҳ�ǿؼ�*/
		WindowsAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0 + WIDGE_W(hWin) - 1, 0, 1, WIDGE_H(hWin));
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0,
			0 + WIDGE_H(hWin) - 1,
			WIDGE_W(hWin->hWinHead), 1);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsAdd(hWin, widgeTemp);

		widgeTemp = WidgeCreate(0,0,
			WIDGE_W(hWin->hWinHead), 1);
		widgeTemp->pencil.DrawBkColor = RGB565(150, 150, 150);
		WindowsAdd(hWin, widgeTemp);
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
		else if (CentWIN->hWinHead->hXButtonClose == Object) {
			GUISendWINDelMsg(WIDGE_PARENT(CentWIN), CentWIN);
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
			WIDGE_X(hObject),
			WIDGE_Y(hObject),
			hObject->lastRect.w, 
			hObject->lastRect.h
		);
	}
	else {
		_SetMinWIN(hObject);
		/*����֮ǰ�Ŀ��*/
		hObject->lastRect.w = WIDGE_W(hObject);
		hObject->lastRect.h = WIDGE_H(hObject);
		/*����Ϊ����ͷ�Ĵ�С*/
		WindowsResize(hObject, 
			WIDGE_X(hObject),
			WIDGE_Y(hObject),
			WIDGE_W(hObject->hWinHead),
			WIDGE_H(hObject->hWinHead)
		);
	}
}
/*���ô������*/
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
		XRECT_COPY(&(hObject->lastRect), hObject);
		WindowsResize(hObject, 
			WIDGE_X(WIDGE_PARENT(hObject)),
			WIDGE_Y(WIDGE_PARENT(hObject)),
			WIDGE_W(WIDGE_PARENT(hObject)),
			WIDGE_H(WIDGE_PARENT(hObject))
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
	WINDOWS_HEAD_MARK_HEAD(Resize)((HWIN_HEAD)hWidgeBaseHead, x, y, w, 20);

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
	HLIST hlist=ListGet(_PToHGroupWidgeType(hWin)->widgetList, index);
	return hlist;
}
/*�����������һ���ؼ�*/
int8 WINDOWS_MARK_HEAD(Add)(HWIN hWin, void *widge) {
	if (!hWin|| !widge) { return -1; }
	HLIST addItem = ListNew();
	if (!addItem) { return -1; }
	addItem->val = widge;
	
	if (ListAddLast(_PToHGroupWidgeType(hWin)->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGE_BASE hWidge = widge;

	hWidge->moveToFun(
		hWidge, 
		hWidge->rect.x + WIDGE_X(hWin),
		hWidge->rect.y + WIDGE_Y(hWin) + (_IsDrawWinHead(hWin) ? _PToHGroupWidgeType(hWin->hWinHead)->widgeBase.rect.h : 0)
	);
	/*���ø�����*/
	WidgeSetParentWin(hWidge, hWin);

	/* ���ø��´������� */
	WindowsInvaildRect((HWIDGE_BASE)(WIDGE_PARENT(hWin)), (HXRECT)(hWin));
	return 0;
}
/*���ô��ڵ���Ч���򣬵���gui����Ч�������ú���*/
void WINDOWS_MARK_HEAD(InvaildRect)(HWIDGE_BASE hWidgeBase,HXRECT hXRect) {
	HWIDGE_BASE srcWidge;
	if (!hWidgeBase) { return; }
	/*�����������͸��������ԴΪ͸������*/
	srcWidge = hWidgeBase;
	while (WIDGE_PARENT(srcWidge)!=NULL && _GET_IS_DPY(WIDGE_PARENT(srcWidge))) {
		srcWidge = WIDGE_PARENT(srcWidge);
	}

	if (hXRect == NULL) {
		GUISendDrawMsg(srcWidge, MSG_WIN_INVAILD_UPDATE, 0,
			hWidgeBase->rect.x, hWidgeBase->rect.y, hWidgeBase->rect.w, hWidgeBase->rect.h
		);
	}
	else {
		GUISendDrawMsg(srcWidge, MSG_WIN_INVAILD_UPDATE, 0,
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

	/*���������Ҫ�ͷŵ��ڴ�*/

	/*���ø���Ĺرպ���*/
	GROUP_MARK_HEAD(Close)((HGROUP_WIDGE)hWin);
}
/*�ƶ�����*/
void WINDOWS_MARK_HEAD(MoveTo)(HWIN hWin, int16 x, int16 y) {
	int16 dx;
	int16 dy;
	XRECT lastRect;
	XRECT updateRect;
	if (!hWin) { return; }
	/*��λ���������һ���ƶ���ƫ����*/
	dx = x - WIDGE_X(hWin);
	dy = y - WIDGE_Y(hWin);

	/*λ��û�иı�*/
	if (x == WIDGE_X(hWin)
		&& y == WIDGE_Y(hWin)) {
		return;
	}
	XRECT_COPY(&lastRect, (HXRECT)hWin);
	/* ���´��ڣ����µ�λ������һ�ε�λ��*/
	//WindowsInvaildRect((HWIDGE_BASE)(WIDGE_PARENT(hWin)), (HXRECT)(&(lastRect)));
	/*�����µ�λ��*/
	_PToHXRECTType(hWin)->x = x;
	_PToHXRECTType(hWin)->y = y;

	//��������ÿһ���ؼ�λ��
	HLIST hWidgeList = _PToHGroupWidgeType(hWin)->widgetList;

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


	updateRect.x = MIN(((HXRECT)(hWin))->x, lastRect.x);
	updateRect.y = MIN(((HXRECT)(hWin))->y, lastRect.y);
	updateRect.w = MAX(((HXRECT)(hWin))->w + ((HXRECT)(hWin))->x,
		lastRect.x + lastRect.w) - updateRect.x;
	updateRect.h = MAX(((HXRECT)(hWin))->h + ((HXRECT)(hWin))->y,
		lastRect.y + lastRect.h) - updateRect.y;


	WindowsInvaildRect((HWIDGE_BASE)(WIDGE_PARENT(hWin)), (HXRECT)(&updateRect));
}
/*���ô�����ɫ*/
void WINDOWS_MARK_HEAD(SetColor)(HWIN hWin, uintColor color) {
	if (!hWin) { return; }
	_PToHWidgeBaseType(hWin)->pencil.DrawBkColor = color;

	/* ���´��� */
	WindowsInvaildRect((HWIDGE_BASE)hWin, (HXRECT)hWin);
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
			hWin->winProcessFun(hWin, _PToHWidgeBaseType(hWin)->arg, &msg);
		}
	}
	/*�Ƿ���Ҫ��ʾ*/
	if (!_GetVisable(hWin)) { return; }
	/*�Ƿ���Ҫ����*/
	if (!IsGUINeedCut((HWIDGE_BASE)hWin) && !_IsDrawAllLag(hWin)) { return; }

	/*������Ҫȫ��ˢ��*/
	_ClrDrawAllLag(hWin);

	/*���õ�ǰ��ͼ����*/
	if (!DrawSetArea((HWIDGE_BASE)hWin)) { return; }

	/*�����Ƿ�����˴���߿������ü�����*/
	if (_IsDrawWinHead(hWin)) {
		/*�л�����ǰ������*/
		cutPostionList=RectCutAddRectList(_PToHGroupWidgeType(hWin)->widgetList->next);
	}
	else {
		cutPostionList = RectCutAddRectList(WindowsGetWidgeEx(hWin,5));
	}

	_PToHWidgeBaseType(hWin)->pencil.DrawColor = _PToHWidgeBaseType(hWin)->pencil.DrawBkColor;
	/*������*/
	DrawCutRect(hWin, _PToHXRECTType(hWin));
	/*�ػ��¼�*/
	if (hWin->winProcessFun != NULL) {
		MSGE msg;
		msg.msgSrc = hWin;
		msg.msgType = MSG_WIN;
		msg.msgID = MSG_WIN_PAINT;
		hWin->winProcessFun(hWin, _PToHWidgeBaseType(hWin)->arg, &msg);
	}
	RectCutSplitRectList(cutPostionList);
	/*����ÿһ���ؼ�*/
	_StartScanU(_PToHGroupWidgeType(hWin)->widgetList) {//��ʼɨ��
		/*��ǰ�б�ؼ����������*/
		cutPostionList = RectCutAddRectList(tempItem->next);
		/*���ƿؼ�*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));
		/*�˻ؼ�����*/
		RectCutSplitRectList(cutPostionList);
	
	}
	/*����ɨ��*/
	_EndScanU();
	/*�ָ���ͼ����*/
	DrawResetArea((HWIDGE_BASE)hWin);
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
						hWin->t_dx = hMsg->msgVal.rect.x - WIDGE_X(hWin);
						hWin->t_dy = hMsg->msgVal.rect.y - WIDGE_Y(hWin);
						_SetWinMoveing(hWin);
						WindowsInvaildRect(WIDGE_PARENT(hWin), _PToHXRECTType(hWin));
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
				hMsg->msgVal.rect.x >= WIDGE_X(hWin) + WIDGE_W(hWin) -2
				&& hMsg->msgVal.rect.x <= WIDGE_X(hWin) + WIDGE_W(hWin)
				) {
				hWin->t_xy = hMsg->msgVal.rect.x;
				_SetChangeWinRHSize(hWin);
				return 0;
			}
			else if (
				hMsg->msgVal.rect.y >= WIDGE_Y(hWin) + WIDGE_H(hWin) - 2
				&& hMsg->msgVal.rect.y <= WIDGE_Y(hWin) + WIDGE_H(hWin)
				) {
				hWin->t_xy = hMsg->msgVal.rect.y;
				_SetChangeWinVSize(hWin);
				return 0;
			}
			else if (
				hMsg->msgVal.rect.x >= WIDGE_X(hWin)
				&& hMsg->msgVal.rect.x <= WIDGE_X(hWin) + 2
				) {
				hWin->t_xy = hMsg->msgVal.rect.x;
				_SetChangeWinLHSize(hWin);
				return 0;
			}
			break;
		case MSG_TOUCH_MOVE:
			if (_IsChangeWinRHSize(hWin)) {
				_PToHWidgeBaseType(hWin)->widgeResizeFun(hWin, ((HXRECT)hWin)->x, ((HXRECT)hWin)->y,
					((HXRECT)hWin)->w + (hMsg->msgVal.rect.x - hWin->t_xy), ((HXRECT)hWin)->h);
				hWin->t_xy = hMsg->msgVal.rect.x;
				return 0;
			}else if (_IsChangeWinVSize(hWin)) {
				_PToHWidgeBaseType(hWin)->widgeResizeFun(hWin, ((HXRECT)hWin)->x, ((HXRECT)hWin)->y,
					((HXRECT)hWin)->w, (((HXRECT)hWin)->h + (hMsg->msgVal.rect.y - hWin->t_xy)));
				hWin->t_xy = hMsg->msgVal.rect.y;
				return 0;
			}else if (_IsChangeWinLHSize(hWin)) {
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
			WIDGE_X(hWin), WIDGE_Y(hWin),
			WIDGE_W(hWin), WIDGE_H(hWin))) {
			return 2;
		}

		HLIST hWidgeList = _PToHGroupWidgeType(hWin)->widgetList;
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
			hWin->winProcessFun(hWin, _PToHWidgeBaseType(hWin)->arg, hMsg);
		}
		return RES_FAIL;
	}

	return RES_FAIL;
}



