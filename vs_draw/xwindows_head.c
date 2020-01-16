
#include "xwindows_head.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "text_widge.h"
#include "button_widge.h"

PUBLIC HWIN_HEAD WINDOWS_HEAD_MARK_HEAD(Create)(char *title ,int16 x,int16 y,int16 w,int16 h) {
	HWIN_HEAD hWinHead;
	HTEXT_WIDGE hTextWidge;
	
	
	hWinHead= (HWIN_HEAD)(xMalloc(sizeof(WIN_HEAD)));
	if (hWinHead == NULL) {
		return NULL;
	}
	hTextWidge = TextWidegeCreate(title ? title : _DefaultWinHeadName, 0, 0);
	if (hTextWidge == NULL) {
		xFree(hWinHead);
		return NULL;
	}
	hWinHead->hXButtonMin = BUTTON_MARK_HEAD(Create)("-",
		w- hTextWidge->hFont->fontInfo.w-1, h+1,
		hTextWidge->hFont->fontInfo.w, hTextWidge->hFont->fontInfo.h-2);
	if (hWinHead->hXButtonMin == NULL) {
		xFree(hWinHead);
		xFree(hTextWidge);
		return NULL;
	}

	//�ؼ��б�
	hWinHead->widgetList = ListNew();
	if (hWinHead->widgetList == NULL) {
		xFree( hWinHead);
	}
	WIDGE_MARK_HEAD(Init)((HWIDGE_BASE)hWinHead, x, y, w, h);

	hWinHead->widgeBase.rect.x = x;
	hWinHead->widgeBase.rect.y = y;
	hWinHead->widgeBase.rect.w = w;
	hWinHead->widgeBase.rect.h = hTextWidge->hFont->fontInfo.h;
	hWinHead->widgeBase.paintFun = WINDOWS_HEAD_MARK_HEAD(Paint);
	hWinHead->widgeBase.moveToFun = WINDOWS_HEAD_MARK_HEAD(MoveTo);
	hWinHead->widgeBase.widgeCallBackFun = WINDOWS_HEAD_MARK_HEAD(CallBack);
	hWinHead->widgeBase.widgeCloseFun = WINDOWS_HEAD_MARK_HEAD(Close);
	
	hWinHead->widgeBase.pencil.DrawColor = _DefaultHeadColor;
	hWinHead->widgeBase.pencil.DrawBkColor = _DefaultFontColor;

	hWinHead->widgeBase.pencil.x = x;
	hWinHead->widgeBase.pencil.y = y;
	hWinHead->widgeBase.pencil.w = w;
	hWinHead->widgeBase.pencil.h = hWinHead->widgeBase.rect.h;

	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hTextWidge);
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonMin);

	return hWinHead;
}

//���һ���ؼ�
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Close)(HWIN_HEAD hWinHead) {
	if (!hWinHead) { return; }
	/*�������ͷŴ����е��ڴ�*/
	/*��ȡÿһ���ؼ�*/
	HLIST hWidgeList = hWinHead->widgetList;
	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);
			/*����ÿһ���ؼ��Ĺرպ���*/
			hWidge->widgeCloseFun(hWidge);

			hWidgeList = hWidgeList->next;
		}
	}
	WIDGE_MARK_HEAD(Close)((HWIDGE_BASE)hWinHead);
}
//���һ���ؼ�
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(Add)(HWIN_HEAD hWinHead, void *widge) {
	if (!hWinHead || !widge) { return -1; }
	HLIST addItem = ListNew();
	if (!addItem) { return -1; }
	addItem->val = widge;
	if (ListAddLast(hWinHead->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGE_BASE hWidge = widge;
	hWidge->parentHWIN = hWinHead;
	hWidge->moveToFun(hWidge, 
		hWidge->rect.x + hWinHead->widgeBase.rect.x,
		hWidge->rect.y + hWinHead->widgeBase.rect.y );

	return 0;
}
/*������С����ť�Ļص��ӿ�*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(HWIN_HEAD hObject,void *arg, ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)((HWIDGE_BASE)(hObject->hXButtonMin), arg,viewClickCallBack);
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(MoveTo)(HWIN_HEAD hObject, int16 x, int16 y) {
	int16 dx=0;
	int16 dy=0;
	if (!hObject) { return; }
	dx = x - (hObject->widgeBase.rect.x);
	dy = y - (hObject->widgeBase.rect.y);
	hObject->widgeBase.rect.x = x;
	hObject->widgeBase.rect.y = y;
	//��������ÿһ���ؼ�λ��
	HLIST hWidgeList = hObject->widgetList;

	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);
			hWidge->moveToFun(hWidge, hWidge->rect.x + dx, hWidge->rect.y + dy);
			hWidgeList = hWidgeList->next;
		}
	}
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetArea)(HWIN_HEAD hObject,int16 x, int16 y, int16 w, int16 h) {
	if (!hObject) { return; }
	hObject->widgeBase.pencil.x = x;
	hObject->widgeBase.pencil.y = y;
	hObject->widgeBase.pencil.w = w;
	hObject->widgeBase.pencil.h = h;
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetVisable)(void* hObject,int8 isVisable) {
	HWIN_HEAD hWinHead;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	hWinHead->widgeBase.isVisable = isVisable;
}
PRIVATE void WINDOWS_HEAD_MARK_HEAD(Paint)(void * hObject){
	HWIN_HEAD hWinHead;
	HLIST cutPostionList;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	if (!hWinHead->widgeBase.isVisable) { return; }
	//if (!isGUINeedCut(hWinHead)) { return; }
	DrawSetArea(hWinHead);//����õ���ǰ��ͼ����
	//����õ���������
	cutPostionList=RectCutAddRectList(hWinHead->widgetList->next);
	DrawCutRect(hWinHead, 
		&(hWinHead->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(hWinHead->widgetList) {//��ʼɨ��
		cutPostionList = RectCutAddRectList(tempItem->next);//�����ڸǵļ��о���
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));//���Ʋ������ͼ����
		RectCutSplitRectList(cutPostionList);//ȥ���ڸǵļ��о���
		DrawResetArea(hWinHead);//�ָ���ͼ����
	}
	_EndScanU();	//����ɨ��
	

}
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	HWIN_HEAD hWinHead = hObject;
	if (!hWinHead || !hMsg) { return -1; }
	if (!(hWinHead->widgeBase.isVisable)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(hWinHead->widgeBase.rect))) {
			HLIST hWidgeList = hWinHead->widgetList;
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
			return 0;
		}
	}
	return -1;
}
