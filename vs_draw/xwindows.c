
#include "xwindows.h"
#include "x_malloc.h"
#include "msg.h"
#include "paint_cut.h"
#include "paint.h"
#include "gui.h"


HWIN WindowsCreate(char *title,int16 x,int16 y,int16 w,int16 h) {
	/*创建一个窗口*/
	HWIN hWin = (HWIN)(xMalloc(sizeof(WIN)));
	if (hWin == NULL) {
		return NULL;
	}
	/*保存控件列表*/
	hWin->widgetList = ListNew();
	if (hWin->widgetList == NULL) {
		xFree( hWin);
	}
	/*创建一个窗口头部控件*/
	hWin->hWinHead = WindowsHeadCreate(title, 0, 0, w, 0);
	if (hWin->hWinHead == NULL) { 
		xFree( hWin->widgetList);
		xFree( hWin); 
		return NULL;
	}
	
	/*设置窗口初值*/
	hWin->winWidge.rect.x = x;
	hWin->winWidge.rect.y = y;
	hWin->winWidge.rect.w = w;
	hWin->winWidge.rect.h = h;

	/*设置回调函数*/
	hWin->winWidge.paintFun = WindowsPaint;
	hWin->winWidge.moveToFun = WindowsMoveTo;
	hWin->winWidge.widgeCallBackFun = WindowsCallBack;

	/*设置画笔颜色*/
	hWin->winWidge.pencil.DrawColor = _DefaultFrColor;
	/*设置窗口有效区域*/
	hWin->winWidge.pencil.x = x;
	hWin->winWidge.pencil.y = y;
	hWin->winWidge.pencil.w = w;
	hWin->winWidge.pencil.h = h;

	hWin->winWidge.isVisable = TRUE;
	hWin->winWidge.parentHWIN = NULL;

	/*设置需要全部重绘*/
	_SetDrawAllLag(hWin);

	/*先设置为不显示头部*/
	_ClrDrawWinHead(hWin);
	/*加入一个控件*/
	WindowsWidgeAdd(hWin, hWin->hWinHead);
	/*默认显示头部*/
	_SetDrawWinHead(hWin);
	{
		HWIDGE_BASE widgeTemp;
		widgeTemp = WidgeCreate(0, 0, 1, hWin->winWidge.rect.h);
		/*添加边框，边框也是控件*/
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

/*获取窗口中的某个位置的控件，得到的不是窗口而是窗口的ListItem*/
void* WindoswGetWidge(HWIN hWin, uint16 index) {
	if (!hWin) { return NULL; }
	HLIST hlist=ListGet(hWin->widgetList, index);
	return hlist;
}

/*往窗体中添加一个控件*/
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
	/*设置父窗口*/
	WidgeSetParentWin(hWidge, hWin);

	/* 设置更新窗口区域 */
	if (hWin->winWidge.parentHWIN == NULL) {
		DrawInvaildRect(hWin);
	}else {
		DrawInvaildRect(hWin->winWidge.parentHWIN);
	}
	return 0;
}
/*设置窗口的无效区域，调用gui的无效区域设置函数*/
void WindowsInvaildRect(HWIN hWin,HXRECT hXRect) {
	if (!hWin) { return; }
	if (hXRect == NULL) {
		DrawInvaildRect((HXRECT)hWin);
	}
	else {
		DrawInvaildRect(hXRect);
	}
}
/*关闭窗口*/
void WindowsClose(HWIN hWin) {
	if (!hWin) { return; }
	/*在这里释放窗口中的内存*/


}
/*移动窗体*/
void WindowsMoveTo(HWIN hWin, int16 x, int16 y) {
	//int16 dx;
	//int16 dy;
	if (!hWin) { return; }
	/*移动的偏移量*/
	hWin->dx = x - hWin->winWidge.rect.x;
	hWin->dy = y - hWin->winWidge.rect.y;
	hWin->winWidge.rect.x = x;
	hWin->winWidge.rect.y = y;


	//重新设置每一个控件位置
	HLIST hWidgeList = hWin->widgetList;

	/*移动内部的每一个控件*/
	if (hWidgeList != NULL) {
		hWidgeList = hWidgeList->next;
		while (hWidgeList) {
			HWIDGE_BASE hWidge = (HWIDGE_BASE)(hWidgeList->val);	
			hWidge->moveToFun(hWidge, hWidge->rect.x + hWin->dx, hWidge->rect.y + hWin->dy);
			hWidgeList = hWidgeList->next;
		}
	}

	/* 更新窗口 */
	if (hWin->winWidge.parentHWIN == NULL) {
		WindowsInvaildRect(hWin, hWin);
	}
	else {
		WindowsInvaildRect(hWin,hWin->winWidge.parentHWIN);
	}
	setMovingWin(hWin);

}
/*设置窗口颜色*/
void WindowsSetColor(HWIN hWin, uintColor color) {
	if (!hWin) { return; }
	hWin->winWidge.pencil.DrawColor = color;

	/* 更新窗口 */
	if (hWin->winWidge.parentHWIN == NULL) {
		WindowsInvaildRect(hWin, hWin);
	}
	else {
		WindowsInvaildRect(hWin, hWin->winWidge.parentHWIN);
	}
}

/*设置是否有效*/
void WindowsSetVisable(void* hObject, int8 isVisable) {
	HWIN hWin;
	hWin = hObject;
	if (!hWin) { return; }
	hWin->winWidge.isVisable = isVisable;

	/* 更新窗口 */
	if (hWin->winWidge.parentHWIN == NULL) {
		WindowsInvaildRect(hWin, hWin);
	}
	else {
		WindowsInvaildRect(hWin, hWin->winWidge.parentHWIN);
	}
}
/*
*重新绘制窗口
*/
void WindowsPaint(void *hObject) {
	HLIST cutPostionList;
	HWIN hWin;
	hWin = hObject;
	if (!hWin) { return; }
	if(!(hWin->winWidge.isVisable)){return ;}
	if (!isGUINeedCut(hWin) && !_IsDrawAllLag(hWin)) { return; }
	/*初次需要全部刷新*/
	_ClrDrawAllLag(hWin);

	/*设置当前绘图区域*/
	DrawSetArea(hWin);
	/*根据是否添加了窗体边框来设置剪裁域*/
	if (_IsDrawWinHead(hWin)) {
		/*切换到当前剪裁域*/
		cutPostionList=RectCutAddRectList(hWin->widgetList->next);
	}
	else {
		cutPostionList = RectCutAddRectList(WindoswGetWidge(hWin,4));
	}
	/*画矩形*/
	DrawCutRect(hWin, &(hWin->winWidge.rect));
	RectCutSplitRectList(cutPostionList);
	/*退回到之前剪裁域*/
	//if (_IsDrawWinHead(hWin)) {
	//	//RectCutSplitRectList(hWin->widgetList->next);
	//}
	//else {
	//	//RectCutSplitRectList(WindoswGetWidge(hWin, 4));
	//}

	/*迭代每一个控件*/
	_StartScanU(hWin->widgetList) {//开始扫描
		/*当前列表控件加入剪裁域*/
		cutPostionList = RectCutAddRectList(tempItem->next);
		/*绘制控件*/
		_PToHWidgeBaseType(val)->paintFun(_PToHWidgeBaseType(val));
		/*退回剪裁域*/
		//RectCutSplitRectList(tempItem->next);
		RectCutSplitRectList(cutPostionList);
		/*恢复绘图区域*/
		DrawResetArea(hWin);
	}
	/*结束扫描*/
	_EndScanU();

}
/*是否显示头部*/
void WindowsSetDrawHead(HWIN hWin, int8 isEnable) {
	uint8 i = 0;
	if (isEnable) {
		_SetDrawWinHead(hWin);
	}
	else {
		_ClrDrawWinHead(hWin);
	}

	/*前四个控件为边界和头部*/
	for (i = 0; i < 4; i++) {
		HLIST widge= WindoswGetWidge(hWin, i);
		if (widge != NULL) {
			((HWIDGE_BASE)(widge->val))->isVisable= isEnable;
		}
	}

	/* 更新窗口 */
	if (hWin->winWidge.parentHWIN == NULL) {
		WindowsInvaildRect(hWin, hWin);
	}
	else {
		WindowsInvaildRect(hWin, hWin->winWidge.parentHWIN);
	}
}
/*窗口事件处理*/
int8 WindowsCallBack(void* hObject,HMSGE hMsg) {
	HWIN hWin = hObject;
	if (!hWin || !hMsg) { return -1; }

	if (hMsg->msgType == MSG_TOUCH) {
		if (_IsDrawWinHead(hWin)) {
			switch (hMsg->msgID) {
			case MSG_TOUCH_PRESS:
				//有头才能抓取移动
					//被抓的是不是头
					if (WindowsHeadsCallBack(hWin->hWinHead, hMsg) == 0) {
						hWin->t_dx = hMsg->msgVal.xy.x - hWin->winWidge.rect.x;
						hWin->t_dy = hMsg->msgVal.xy.y - hWin->winWidge.rect.y;
						_SetWinMoveing(hWin);
						WindowsInvaildRect(hWin, hWin);
					}
				break;
			case MSG_TOUCH_MOVE:
				//有头才能抓取移动
					if (_IsWinMoving(hWin)) {
						WindowsMoveTo(hWin, 
							hMsg->msgVal.xy.x - hWin->t_dx, hMsg->msgVal.xy.y - hWin->t_dy);
						return 0;
					}
				break;
			case MSG_TOUCH_RELEASE:
				//有头才能抓取移动
				_ClrWinMoveing(hWin);

				//return 0;
				break;
			}
		}
		if (!_IsDrawCheckPoint(hMsg->msgVal.xy.x, hMsg->msgVal.xy.y,
			hWin->winWidge.rect.x, hWin->winWidge.rect.y, 
			hWin->winWidge.rect.w, hWin->winWidge.rect.h)) {
			return 2;
		}
		if (hMsg->msgID == MSG_TOUCH_PRESS) {
			DrawInvaildRect(_PToHXRECTType(hWin));
		}
		HLIST hWidgeList = hWin->widgetList;
		if (hWidgeList == NULL) { return 1; }
		if (_IsDrawWinHead(hWin)) {
			hWidgeList = hWidgeList->next;/*跳过头部*/
		}
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
					if ((hWidge->isVisable)) {
						if ((ret = hWidge->widgeCallBackFun(hWidge, hMsg)) == 0) {
							return 0;
						}
					}
					lastWidge = lastWidge->lnext;
				
				}
			}
		}
		return 1;
	}

	return 1;
}



