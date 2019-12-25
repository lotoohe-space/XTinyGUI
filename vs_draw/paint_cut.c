
#include "paint_cut.h"
#include "x_malloc.h"
#include "gui.h"
#include "list.h"
#include <string.h>

XRECT xResRect;
/*和那个矩形产生了碰撞*/
HXRECT lastRightRect = NULL;
HLIST rectCutList;//剪裁矩形
HLIST rectItem;//临时用
XRECT bgRect;//被剪裁的矩形
XPOINT startPoint;//初始扫描起点

HLIST lastAddPostion = NULL;
BOOL isEnd = 0;

//添加剪裁矩形
//每次添加到尾部
HLIST RectCutAddRectList(HLIST addRectList) {
	HLIST tempRectCutList;
	tempRectCutList = rectCutList;

	while (tempRectCutList->next) {
		tempRectCutList = tempRectCutList->next;
	}
	tempRectCutList->next = addRectList;
	//lastAddPostion = tempRectCutList;
	return tempRectCutList;
}
//指定的List处断开
void RectCutSplitRectList(HLIST splitRectList) {
	//HLIST tempRectCutList;
	//tempRectCutList = rectCutList;
	splitRectList->next = NULL;
	/*if (lastAddPostion->next == splitRectList) {
		lastAddPostion->next = NULL;
	}
	else {*/
	/*while (tempRectCutList->next != splitRectList) {
		tempRectCutList = tempRectCutList->next;
	}
	tempRectCutList->next = NULL;*/
	/*}*/
}

//初始化剪裁
int8 RectCutInit(void) {

	rectCutList = (HLIST)xMalloc(sizeof(LIST));
	if (!rectCutList) {
		return FALSE;
	}

	return TRUE;
}

//开始剪裁
int8 RectCutStart(HXRECT hXRECT) {
	rectItem = rectCutList->next;
	bgRect.x = hXRECT->x;
	bgRect.y = hXRECT->y;
	bgRect.w = hXRECT->w;
	bgRect.h = hXRECT->h;

	startPoint.x = bgRect.x;
	startPoint.y = bgRect.y;

	isEnd = FALSE;

	return 1;
}

HXRECT RectCutGetNext(void) {
	HLIST tempList = rectItem;

	if (rectItem != NULL) {
		rectItem = rectItem->next;

		return tempList->val;
	}
	return NULL;
}
/*矩形剪裁结束*/
void RectCutEnd(void) {
	rectItem = NULL;
	//if (splitItem == NULL) {return;}
	//splitItem->next = NULL;
}

extern void fill_rect(int x, int y, int w, int h, int color);
/*得到下一个剪裁矩形*/
HXRECT RectCutFind(void) {
	HXRECT hRect;
	XPOINT pointEnd;
	HLIST tempRectCutlist;

	uint16 temp_w = 0;
	uint16 val = bgRect.y + bgRect.h;
	//int val1 = bgRect.y + bgRect.h;
	//fill_rect(startPoint.x, startPoint. y, 5, 5, rand()%65535);
	pointEnd.x = bgRect.w + bgRect.x;
	pointEnd.y = bgRect.y + bgRect.h;

	lastRightRect = NULL;
gotonext:
	rectItem = rectCutList->next;
	while ((hRect = RectCutGetNext()) != NULL) {

		/*************查找参考线下面离得最近的一条边*************/
		/*优化，不应该只找离得最近的一条边，应该是找右边最近的矩形的上下最近的一条边*/
		if (hRect->y > startPoint.y) {							/*上边*/
			if (hRect->y - startPoint.y < val) {				/*找最近的一条*/
				val = hRect->y - startPoint.y;
				pointEnd.y = hRect->y;

			}
		}
		else if (hRect->y + hRect->h > startPoint.y) {			/*下边*/
			if ((hRect->y + hRect->h - startPoint.y) < val) {	/*找最近的一条*/
				val = (hRect->y + hRect->h - startPoint.y);
				pointEnd.y = hRect->y + hRect->h;

			}

			/************找右边离起点x最近的线************/
			if (startPoint.x < hRect->x) {						/*在右边的线*/
				if (pointEnd.x > hRect->x) {					/*离得最近的一条*/
					pointEnd.x = hRect->x;
					lastRightRect = hRect;
					temp_w = hRect->x + hRect->w;
				}
			}
			if (lastRightRect != NULL) {
				if (_IsDrawCheckArea(lastRightRect->x, lastRightRect->y, lastRightRect->w, lastRightRect->h,
					hRect->x, hRect->y, hRect->w, hRect->h)) {
					if (hRect->x + hRect->w > lastRightRect->x + lastRightRect->w) {
						temp_w = hRect->x + hRect->w;
					}
					else {
						temp_w = lastRightRect->x + lastRightRect->w;
					}
					lastRightRect = hRect;
				}

			}

			/********************************************/

		}
		/*****************************************************/

		/*找起点*/
		if (_IsDrawCheckPoint(startPoint.x, startPoint.y,
			hRect->x, hRect->y, hRect->w, hRect->h)) {

			/*起点碰撞，向右移动*/
			startPoint.x = hRect->x + hRect->w;
			if (startPoint.x >= bgRect.w + bgRect.x) {
				/*起点大于等于终点并且终点y大于等于剪裁矩形则退出*/
				if (startPoint.x >= pointEnd.x
					&& pointEnd.y >= bgRect.y + bgRect.h) {

					HLIST tempItem;
					int tempPointEndY;
					tempPointEndY = pointEnd.y;
					tempItem = rectItem;
					/*pointEnd.y可能还没有达到与参考位置最近，故继续遍历*/
					while (hRect = RectCutGetNext()) {
						if (hRect->y > startPoint.y) {							/*上边*/
							if (hRect->y - startPoint.y < val) {				/*找最近的一条*/
								val = hRect->y - startPoint.y;
								pointEnd.y = hRect->y;

							}
						}
						else if (hRect->y + hRect->h > startPoint.y) {			/*下边*/
							if ((hRect->y + hRect->h - startPoint.y) < val) {	/*找最近的一条*/
								val = (hRect->y + hRect->h - startPoint.y);
								pointEnd.y = hRect->y + hRect->h;

							}
						}
					}
					rectItem = tempItem;
					/*如果没有变化，则表示找不到了，退出*/
					if (tempPointEndY == pointEnd.y) {
						return NULL;
					}

				}
				/*右移超出边界，x回到起点*/
				startPoint.x = bgRect.x;
				/*Y回到向下上次找到的最近的一条边*/
				startPoint.y = pointEnd.y;

				/*更新完成后，剪裁的起点发生了变化，重新退回剪裁域*/
				rectItem = rectCutList->next;

				/*复位到原始起点*/
				pointEnd.x = bgRect.w + bgRect.x;
				pointEnd.y = bgRect.y + bgRect.h;

				val = bgRect.y + bgRect.h;

				lastRightRect = NULL;
			}
			/*退回，因为可能之前的矩形与现在的碰撞，但是之前的起点并未发现*/
			rectItem = rectCutList->next;
		}
	}

	xResRect.x = startPoint.x;
	xResRect.y = startPoint.y;
	xResRect.w = pointEnd.x - startPoint.x;
	xResRect.h = pointEnd.y - startPoint.y;

	//lastY = pointEnd.y;
	if (pointEnd.x >= bgRect.x + bgRect.w || temp_w >= bgRect.x + bgRect.w) {
		startPoint.y = pointEnd.y;
		startPoint.x = bgRect.x;
		if (pointEnd.y >= bgRect.h + bgRect.y) {
			isEnd = TRUE;
		}
	}
	else {
		startPoint.x = pointEnd.x;
		if (startPoint.x >= bgRect.x + bgRect.w) {
			startPoint.y = pointEnd.y;
			startPoint.x = bgRect.x;
		}
	}
	return &xResRect;
}