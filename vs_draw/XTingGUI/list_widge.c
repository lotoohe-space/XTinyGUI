#include "list_widge.h"
#include "paint_cut.h"
#include "x_malloc.h"

static void GUITimeoutCb(void* arg);

PUBLIC HLIST_WIDGE LIST_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h) {
	HLIST_WIDGE hObject;

	hObject = (HLIST_WIDGE)(xMalloc(sizeof(LIST_WIDGE)));
	if (hObject == NULL) {
		return NULL;
	}
	GROUP_MARK_HEAD(Init)((HGROUP_WIDGE)hObject, x, y, w, h);
	
	_PToHGroupWidgeType(hObject)->widgeBase.paintFun = LIST_WIDGE_MARK_HEAD(Paint);
	//_PToHGroupWidgeType(hObject)->widgeBase.moveToFun = GROUP_MARK_HEAD(MoveTo);
	_PToHGroupWidgeType(hObject)->widgeBase.widgeCallBackFun = LIST_WIDGE_MARK_HEAD(CallBack);

	_PToHWidgeBaseType(hObject)->pencil.DrawColor = _PToHWidgeBaseType(hObject)->pencil.DrawBkColor;

	hObject->itemsSize.itemSize = LIST_WIDGE_ITEM_SIZE;
	hObject->lastXY = 0;
	hObject->flag = 0;
	hObject->itemGap = 5;
	hObject->dSP = 0;
	hObject->hGUITimeout = GUITimeoutCreate(17, hObject, GUITimeoutCb);
	GUITimeoutClose(hObject->hGUITimeout);

	/*����Ӧģʽ*/
	_SetListWidgeMode(hObject);
	/*Ĭ�ϴ�ֱ����*/
	_SetOTN_V(hObject);
	/*�򿪶���*/
	//_OpenListAnimation(hObject);
	hObject->itemsSize.itemsH = 0;

	return hObject;
}
PUBLIC void LIST_WIDGE_MARK_HEAD(SetFlag)(HLIST_WIDGE hBaseWidge, LIST_WIDGE_FLAG flagBisPoi, BOOL status) {
	if (hBaseWidge == NULL) { return ; }

	hBaseWidge->flag = ((hBaseWidge->flag) & (~(1 << flagBisPoi))) | ((status&0x01) << flagBisPoi);
}
/*���һ���ؼ�*/
PUBLIC uint8 LIST_WIDGE_MARK_HEAD(Add)(HLIST_WIDGE hBaseWidge, HWIDGE_BASE widge) {
	uint16 widgeLength;
	if (hBaseWidge == NULL || widge == NULL) { return FALSE; }

	if (GROUP_MARK_HEAD(Add)((HGROUP_WIDGE)hBaseWidge, widge) == FALSE) {
		return FALSE;
	}
	widgeLength = ((HGROUP_WIDGE)hBaseWidge)->widgeLength - 1;
	if (!_GetListWidgeMode(hBaseWidge)) {/*����ģʽ*/
		if (_GetOTN(hBaseWidge)) {
			/*�������ÿؼ�����Ϣ*/
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x + 
				hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y;
		}
		else {
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y + 
				hBaseWidge->itemsSize.itemSize * widgeLength + widgeLength;
		}
		((HXRECT)widge)->h = hBaseWidge->itemsSize.itemSize;
		((HXRECT)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
	}
	else {/*�߶�����Ӧ*/
		/*�������ÿؼ�����Ϣ*/
		if (_GetOTN(hBaseWidge)) {
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x +
				hBaseWidge->itemsSize.itemsH + hBaseWidge->itemGap * widgeLength;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y;
			hBaseWidge->itemsSize.itemsH += ((HXRECT)widge)->w;
			//((HXRECT)widge)->h = hBaseWidge->groupWidge.widgeBase.rect.h;
		}
		else {
			((HXRECT)widge)->x = hBaseWidge->groupWidge.widgeBase.rect.x;
			((HXRECT)widge)->y = hBaseWidge->groupWidge.widgeBase.rect.y + 
				hBaseWidge->itemsSize.itemsH + hBaseWidge->itemGap * widgeLength;
			hBaseWidge->itemsSize.itemsH += ((HXRECT)widge)->h;
			//((HXRECT)widge)->w = hBaseWidge->groupWidge.widgeBase.rect.w;
		}
			
	}
	
	/*ˢ��*/
	WindowsInvaildRect((HWIDGE_BASE)hBaseWidge, NULL);
	return TRUE;
}
/*ListWidge�ڲ��ؼ���λ�û���*/
PRIVATE uint8 LIST_WIDGE_MARK_HEAD(ListSlide)(HLIST_WIDGE hBaseWidge, int16 dXY) {
	HXRECT widgeRectLast;
	HXRECT widgeRectFirst;

	if (!hBaseWidge) { return FALSE; }
	/*��ȡ��һ�ؼ�*/
	widgeRectFirst = (HXRECT)GROUP_MARK_HEAD(GetWidge)((HGROUP_WIDGE)hBaseWidge, 0);
	if (widgeRectFirst == NULL) { return FALSE; }
	/*��ȡ���һ���ؼ�*/
	widgeRectLast = (HXRECT)GROUP_MARK_HEAD(GetWidge)((HGROUP_WIDGE)hBaseWidge, hBaseWidge->groupWidge.widgeLength - 1);
	if(widgeRectLast == NULL) { return FALSE; }
	if (_GetOTN(hBaseWidge)) {/*����*/
		if (widgeRectLast->x + dXY + widgeRectLast->w < ((HXRECT)(hBaseWidge))->x + ((HXRECT)(hBaseWidge))->w) {
			//return FALSE;
			dXY = ((HXRECT)(hBaseWidge))->x + ((HXRECT)(hBaseWidge))->w - (widgeRectLast->x + widgeRectLast->w);
		}
		if (widgeRectFirst->x + dXY > ((HXRECT)(hBaseWidge))->x) {
			dXY = ((HXRECT)(hBaseWidge))->x-widgeRectFirst->x;
		}
	} else {/*����*/
		if (widgeRectLast->y + dXY + widgeRectLast->h < ((HXRECT)(hBaseWidge))->y + ((HXRECT)(hBaseWidge))->h) {
			dXY = (((HXRECT)(hBaseWidge))->y + ((HXRECT)(hBaseWidge))->h) - (widgeRectLast->y + widgeRectLast->h);
		}
		if (widgeRectFirst->y + dXY > ((HXRECT)(hBaseWidge))->y) {
			dXY = ((HXRECT)(hBaseWidge))->y-widgeRectFirst->y;
		}
	}
	/*ÿ���ؼ�����ƫ��*/
	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList) {/*��ʼɨ��*/
		HXRECT hWidge = (HXRECT)_PToHWidgeBaseType(val);
		if (_GetOTN(hBaseWidge)) {/*����*/
			hWidge->x += dXY;
		}
		else {/*����*/
			hWidge->y += dXY;
		}
	}
	_EndScanU();	/*����ɨ��*/
	WindowsInvaildRect(WIDGE_PARENT(hBaseWidge), (HXRECT)hBaseWidge);
	return TRUE;
}
static void GUITimeoutCb(void* arg) {
	HLIST_WIDGE hBaseWidge = arg;
	if (arg == NULL) { return; }
	if (_GetListAnimation(hBaseWidge)) {/*�Ƿ������˶���*/
		if (!(hBaseWidge->dSP >= -1 && hBaseWidge->dSP <= 1)) {
			if (!LIST_WIDGE_MARK_HEAD(ListSlide)(hBaseWidge, hBaseWidge->dSP)) {
				hBaseWidge->dSP = 0;
			}
			else {
				if (hBaseWidge->dSP < 0) {
					hBaseWidge->dSP++;
					if (hBaseWidge->dSP > 0) {
						hBaseWidge->dSP = 0;
					}
				}
				else if (hBaseWidge->dSP > 0) {
					hBaseWidge->dSP--;
					if (hBaseWidge->dSP < 0) {
						hBaseWidge->dSP = 0;
					}
				}
				GUITimeoutOpen(hBaseWidge->hGUITimeout);
			}
		}
	}
}
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void* hObject) {
	HLIST_WIDGE hBaseWidge;
	HLIST cutPostionList;
	hBaseWidge = hObject;
	if (!hBaseWidge) { return; }
	if (!_GetVisable(hBaseWidge)) { return; }
	if (!IsGUINeedCut((HWIDGE_BASE)hBaseWidge)) { return; }
	if (!DrawSetArea((HWIDGE_BASE)hBaseWidge)) { return; }//����õ���ǰ��ͼ����

	//����õ���������
	cutPostionList = RectCutAddRectList(_PToHGroupWidgeType(hBaseWidge)->widgetList->next);
	DrawCutRect(hBaseWidge,
		&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect));
	RectCutSplitRectList(cutPostionList);

	_StartScanU(_PToHGroupWidgeType(hBaseWidge)->widgetList) {/*��ʼɨ��*/
		cutPostionList = RectCutAddRectList(tempItem->next);/*�����ڸǵļ��о���*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));/*���Ʋ������ͼ����*/
		RectCutSplitRectList(cutPostionList);/*ȥ���ڸǵļ��о���*/
		//DrawResetArea(hBaseWidge);/*�ָ���ͼ����*/
	}
	_EndScanU();	/*����ɨ��*/

	/*�ָ���ͼ����*/
	DrawResetArea((HWIDGE_BASE)hBaseWidge);
}
/*�¼��ص�����*/
PUBLIC int8 LIST_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg) {
	HLIST_WIDGE hBaseWidge = hObject;
	if (!hBaseWidge || !hMsg) { return -1; }
	if (!_GetVisable(hBaseWidge)) { return -1; }
	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawCheckPointR(hMsg->msgVal.rect.x, hMsg->msgVal.rect.y,
			&(_PToHGroupWidgeType(hBaseWidge)->widgeBase.rect))) {
			int16 dXY = 0;
			/*�����¼�*/
			if (hMsg->msgType == MSG_TOUCH) {
				switch (hMsg->msgID) {
				case MSG_TOUCH_PRESS:	
					if (_GetOTN(hBaseWidge)) {/*����*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.x;

						/*�����˶����Ż��õ��������*/
						hBaseWidge->firstXY= hMsg->msgVal.rect.x;
					}
					else {/*����*/
						hBaseWidge->lastXY = hMsg->msgVal.rect.y;

						/*�����˶����Ż��õ��������*/
						hBaseWidge->firstXY = hMsg->msgVal.rect.y;
					}
					if (_GetListAnimation(hBaseWidge)) {
						/*��������˶�������¼��ʱ���µ�ʱ��*/
						hBaseWidge->firstTime = (uint16)GUIGetTick();
					}
					break;
				case MSG_TOUCH_MOVE:
					if (_GetOTN(hBaseWidge)) {/*����*/
						/*������Ҫƫ�Ƶ�*/
						dXY = hMsg->msgVal.rect.x - hBaseWidge->lastXY;			
					}
					else {/*����*/
						dXY = hMsg->msgVal.rect.y - hBaseWidge->lastXY;
					}
					if (dXY != 0) {
						_SetSliding(hBaseWidge);
					}	
					if (_GetSlideState(hBaseWidge)) {
						LIST_WIDGE_MARK_HEAD(ListSlide)(hBaseWidge, dXY);
						if (_GetOTN(hBaseWidge)) {/*����*/
							hBaseWidge->lastXY = hMsg->msgVal.rect.x;
						}
						else {
							hBaseWidge->lastXY = hMsg->msgVal.rect.y;
						}
						return RES_OK;
					}
					break;
				case MSG_TOUCH_RELEASE:
					if (_GetSlideState(hBaseWidge)) {
						if (_GetListAnimation(hBaseWidge)) {/*��������˶���*/
							int16 dis = 0;
							if (_GetOTN(hBaseWidge)) {/*����*/
								dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
							}
							else {/*����*/
								dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY; 
							}

							/*�����ϴβ�ֵ�����ٶ�*/
							hBaseWidge->dSP = 25*(dis)/((uint16)((uint16)GUIGetTick() - hBaseWidge->firstTime));
							if (hBaseWidge->dSP != 0) {
								GUITimeoutOpen(hBaseWidge->hGUITimeout);
							}
						}
						_ClrSliding(hBaseWidge);
						return RES_OK;
					}
					break;
				}	
			}

			/*�¼����ݵ�ÿһ���ؼ�*/
			HLIST hWidgeList = _PToHGroupWidgeType(hBaseWidge)->widgetList;
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
		else {/*������*/
			if (_GetSlideState(hBaseWidge)) {
				if (_GetListAnimation(hBaseWidge)) {/*��������˶���*/
					int16 dis = 0;
					if (_GetOTN(hBaseWidge)) {/*����*/
						dis = hMsg->msgVal.rect.x - hBaseWidge->firstXY;
					}
					else {/*����*/
						dis = hMsg->msgVal.rect.y - hBaseWidge->firstXY;
					}

					/*�����ϴβ�ֵ�����ٶ�*/
					hBaseWidge->dSP =  (dis) / ((uint16)((uint16)GUIGetTick() - hBaseWidge->firstTime));
					if (hBaseWidge->dSP != 0) {
						GUITimeoutOpen(hBaseWidge->hGUITimeout);
					}
				}
				_ClrSliding(hBaseWidge);
				return RES_OK;
			}
		}
	}
	return -1;
}
