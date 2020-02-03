
#include "xwindows_head.h"
#include "x_malloc.h"
#include "tool.h"
#include "gui.h"
#include "paint_cut.h"
#include "text_widge.h"
#include "button_widge.h"

/*����WINDOWS Head�ؼ�*/
PUBLIC HWIN_HEAD WINDOWS_HEAD_MARK_HEAD(Create)(char *title ,int16 x,int16 y,int16 w,int16 h) {
	HWIN_HEAD hWinHead;
	HTEXT_WIDGE hTextWidge;
	
	
	hWinHead= (HWIN_HEAD)(xMalloc(sizeof(WIN_HEAD)));
	if (hWinHead == NULL) {
		return NULL;
	}
	hTextWidge = TextWidgeCreate(title ? title : _DefaultWinHeadName, 0, 0,w,h);
	if (hTextWidge == NULL) {
		xFree(hWinHead);
		return NULL;
	}
	hTextWidge->baseWidge.pencil.DrawBkColor = RGB565(255, 255, 255);

	hWinHead->hXButtonMin = BUTTON_MARK_HEAD(Create)(L"��",
		w- h, 0,h, h);
	
	if (hWinHead->hXButtonMin == NULL) {
		xFree(hWinHead);
		xFree(hTextWidge);
		return NULL;
	}
	_PToHTextWidgeType(hWinHead->hXButtonMin)->hFont = (HFONTF)& fontUNICODE16_16;

	hWinHead->hXButtonMax = BUTTON_MARK_HEAD(Create)(L"��",
		w - 2*h, 0, h, h);
	if (hWinHead->hXButtonMax == NULL) {
		xFree(hWinHead);
		xFree(hTextWidge);
		return NULL;
	}
	_PToHTextWidgeType(hWinHead->hXButtonMax)->hFont = (HFONTF)& fontUNICODE16_16;

	//�ؼ��б�
	_PToHGroupWidgeType(hWinHead)->widgetList = ListNew();
	if (_PToHGroupWidgeType(hWinHead)->widgetList == NULL) {
		xFree( hWinHead);
	}
	WIDGE_MARK_HEAD(Init)((HWIDGE_BASE)hWinHead, x, y, w, h);

	_PToHGroupWidgeType(hWinHead)->widgeBase.paintFun = WINDOWS_HEAD_MARK_HEAD(Paint);
	_PToHGroupWidgeType(hWinHead)->widgeBase.moveToFun = WINDOWS_HEAD_MARK_HEAD(MoveTo);
	_PToHGroupWidgeType(hWinHead)->widgeBase.widgeCallBackFun = WINDOWS_HEAD_MARK_HEAD(CallBack);
	_PToHGroupWidgeType(hWinHead)->widgeBase.widgeCloseFun = WINDOWS_HEAD_MARK_HEAD(Close);

	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawColor = RGB565_BLACK;
	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawFrColor = _DefaultFrColor;
	_PToHGroupWidgeType(hWinHead)->widgeBase.pencil.DrawBkColor = RGB565(255, 255, 255);

	/*���ͷ����*/
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hTextWidge);
	/*�����С��BUTTON*/
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonMin);
	/*������BUTTON*/
	WINDOWS_HEAD_MARK_HEAD(Add)(hWinHead, hWinHead->hXButtonMax);

	return hWinHead;
}
/*��ȡ�����е�ĳ��λ�õĿؼ����õ��Ĳ��Ǵ��ڶ��Ǵ��ڵ�ListItem*/
PUBLIC HWIDGE_BASE WINDOWS_HEAD_MARK_HEAD(GetWidge)(HWIN_HEAD hObject, uint16 index) {
	if (!hObject) { return NULL; }
	HLIST hlist = ListGet(_PToHGroupWidgeType(hObject)->widgetList, index);
	if (hlist != NULL) {
		return (HWIDGE_BASE)(hlist->val);
	}
	else {
		return NULL;
	}
}
/*�������ô��ڵĴ�С,����Ҫ�����ڲ��ؼ��Ĵ�С��λ��,λ����������ڿؼ��е�λ��*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Resize)(HWIN_HEAD hObject, int16 x, int16 y, uint16 w, uint16 h) {
	HWIDGE_BASE hWidgeBase;
	if (!hObject) { return; }
	WIDGE_MARK_HEAD(Resize)((HWIDGE_BASE)hObject, x, y, w, h);
	
	/*����*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 0);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase, x, y, w, h);

	/*��С����ť*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 1);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase, x+w-h, y, h, h);

	/*��󻯰�ť*/
	hWidgeBase = WINDOWS_HEAD_MARK_HEAD(GetWidge)(hObject, 2);
	if (hWidgeBase == NULL) { return; }
	WIDGE_MARK_HEAD(Resize)(hWidgeBase, x+(w - 2 * h), y, h, h);
	
	/*ˢ�¸�����*/
	WindowsInvaildRect(((HWIDGE_BASE)hObject)->parentHWIN, NULL);
}
//�ر�
PUBLIC void WINDOWS_HEAD_MARK_HEAD(Close)(HWIN_HEAD hWinHead) {
	if (!hWinHead) { return; }
	/*�������ͷŴ����е��ڴ�*/
	/*��ȡÿһ���ؼ�*/
	HLIST hWidgeList = _PToHGroupWidgeType(hWinHead)->widgetList;
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
	if (ListAddLast(_PToHGroupWidgeType(hWinHead)->widgetList, addItem) == -1) {
		return -1;
	}
	HWIDGE_BASE hWidge = widge;
	hWidge->parentHWIN = hWinHead;
	hWidge->moveToFun(hWidge, 
		hWidge->rect.x + _PToHGroupWidgeType(hWinHead)->widgeBase.rect.x,
		hWidge->rect.y + _PToHGroupWidgeType(hWinHead)->widgeBase.rect.y );

	return 0;
}
/*������С����ť�Ļص��ӿ�*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMinimumBtnClickBack)(HWIN_HEAD hObject,void *arg, ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)((HWIDGE_BASE)(hObject->hXButtonMin), arg,viewClickCallBack);
}
/*������󻯰�ť�Ļص��ӿ�*/
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetMaximumBtnClickBack)(HWIN_HEAD hObject, void* arg, ViewClickCallBack viewClickCallBack) {
	WIDGE_MARK_HEAD(SetClickBack)((HWIDGE_BASE)(hObject->hXButtonMax), arg, viewClickCallBack);
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(MoveTo)(HWIN_HEAD hObject, int16 x, int16 y) {
	int16 dx=0;
	int16 dy=0;
	if (!hObject) { return; }
	dx = x - (_PToHGroupWidgeType(hObject)->widgeBase.rect.x);
	dy = y - (_PToHGroupWidgeType(hObject)->widgeBase.rect.y);
	_PToHGroupWidgeType(hObject)->widgeBase.rect.x = x;
	_PToHGroupWidgeType(hObject)->widgeBase.rect.y = y;
	//��������ÿһ���ؼ�λ��
	HLIST hWidgeList = _PToHGroupWidgeType(hObject)->widgetList;

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
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.x = x;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.y = y;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.w = w;
	_PToHGroupWidgeType(hObject)->widgeBase.pencil.h = h;
}
PUBLIC void WINDOWS_HEAD_MARK_HEAD(SetVisable)(void* hObject,int8 isVisable) {
	WIDGE_MARK_HEAD(SetVisable)(hObject, isVisable);
	//HWIN_HEAD hWinHead;
	//hWinHead = hObject;
	//if (!hWinHead) { return; }
	//hWinHead->widgeBase.isVisable = isVisable;
}
PRIVATE void WINDOWS_HEAD_MARK_HEAD(Paint)(void * hObject){
	HWIN_HEAD hWinHead;
	HLIST cutPostionList;
	hWinHead = hObject;
	if (!hWinHead) { return; }
	if (!_GetVisable(hWinHead)) { return; }
	//if (!isGUINeedCut(hWinHead)) { return; }
	DrawSetArea(hWinHead);//����õ���ǰ��ͼ����
	//����õ���������
	cutPostionList=RectCutAddRectList(_PToHGroupWidgeType(hWinHead)->widgetList->next);
	DrawCutRect(hWinHead, 
		&(_PToHGroupWidgeType(hWinHead)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hWinHead)->widgetList) {//��ʼɨ��
		cutPostionList = RectCutAddRectList(tempItem->next);//�����ڸǵļ��о���
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));//���Ʋ������ͼ����
		RectCutSplitRectList(cutPostionList);//ȥ���ڸǵļ��о���
		DrawResetArea(hWinHead);//�ָ���ͼ����
	}
	_EndScanU();	//����ɨ��
	

}
PUBLIC int8 WINDOWS_HEAD_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg) {
	HWIN_HEAD hWinHead = hObject;
	if (!hWinHead || !hMsg) { return RES_ASSERT_ERR; }
	if (!_GetVisable(hWinHead)) { return RES_ASSERT_ERR; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(_PToHGroupWidgeType(hWinHead)->widgeBase.rect))) {
			HLIST hWidgeList = _PToHGroupWidgeType(hWinHead)->widgetList;
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
			return RES_OK;
		}
	}
	return RES_FAIL;
}
