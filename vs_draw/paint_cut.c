
#include "paint_cut.h"
#include "x_malloc.h"
#include "gui.h"
#include "list.h"
#include <string.h>

XRECT xResRect;
/*���Ǹ����β�������ײ*/
HXRECT lastRightRect = NULL;
LIST rectCutList = { 0 };//���þ���
HLIST rectItem = NULL;//��ʱ��
XRECT bgRect;//�����õľ���
XPOINT startPoint;//��ʼɨ�����

BOOL isEnd = 0;

//��Ӽ��þ���
//ÿ����ӵ�β��
HLIST RectCutAddRectList(HLIST addRectList) {
	HLIST tempRectCutList;
	tempRectCutList = &rectCutList;

	while (tempRectCutList->next) {
		tempRectCutList = tempRectCutList->next;
	}
	tempRectCutList->next = addRectList;

	return tempRectCutList;
}
//ָ����List���Ͽ�
void RectCutSplitRectList(HLIST splitRectList) {
	splitRectList->next = NULL;
}

//��ʼ������
int8 RectCutInit(void) {


	return TRUE;
}

//��ʼ����
int8 RectCutStart(HXRECT hXRECT) {
	rectItem = rectCutList.next;
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
/*���μ��ý���*/
void RectCutEnd(void) {
	rectItem = NULL;
	//if (splitItem == NULL) {return;}
	//splitItem->next = NULL;
}

extern void fill_rect(int x, int y, int w, int h, int color);
/*�õ���һ�����þ���*/
HXRECT RectCutFind(void) {
	HXRECT hRect;
	XPOINT pointEnd;
//	HLIST tempRectCutlist;

	uint16 temp_w = 0;
	uint16 val = bgRect.y + bgRect.h;
	//int val1 = bgRect.y + bgRect.h;
	
	pointEnd.x = bgRect.w + bgRect.x;
	pointEnd.y = bgRect.y + bgRect.h;

	lastRightRect = NULL;

	rectItem = rectCutList.next;
	while ((hRect = RectCutGetNext()) != NULL) {
		if (((HWIDGE_BASE)hRect)->isVisable == FALSE) { 
			continue;
		}

		/*************���Ҳο���������������һ����*************/
		/*�Ż�����Ӧ��ֻ����������һ���ߣ�Ӧ�������ұ�����ľ��ε����������һ����*/
		if (hRect->y > startPoint.y) {							/*�ϱ�*/
			/*�ڱ����þ��ε������޶���Χ��*/
			if (hRect->y > bgRect.y 
				&& hRect->y < bgRect.y + bgRect.h) {
				if (hRect->y - startPoint.y < val) {				/*�������һ��*/
					val = hRect->y - startPoint.y;
					pointEnd.y = hRect->y;
					//fill_rect(startPoint.x, startPoint.y, 5, 5, rand() % 65535);
				}
			}
		}
		else if (hRect->y + hRect->h > startPoint.y) {			/*�±�*/
			/*�ڱ����þ��ε������޶���Χ��*/
			if (hRect->y + hRect->h > bgRect.y  
				&& hRect->y + hRect->h < bgRect.y + bgRect.h) {
				if ((hRect->y + hRect->h - startPoint.y) < val) {	/*�������һ��*/
					val = (hRect->y + hRect->h - startPoint.y);

					pointEnd.y = hRect->y + hRect->h;
					//fill_rect(startPoint.x, startPoint.y, 5, 5, rand() % 65535);
				}
			}

			/************���ұ������x�������************/
			if (startPoint.x < hRect->x) {						/*���ұߵ���*/
				if (pointEnd.x > hRect->x) {					/*��������һ��*/
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

		/*�����*/
		if (_IsDrawCheckPoint(startPoint.x, startPoint.y,
			hRect->x, hRect->y, hRect->w, hRect->h)) {

			/*�����ײ�������ƶ�*/
			startPoint.x = hRect->x + hRect->w;
			if (startPoint.x >= bgRect.w + bgRect.x) {
				/*�����ڵ����յ㲢���յ�y���ڵ��ڼ��þ������˳�*/
				if (startPoint.x >= pointEnd.x
					&& pointEnd.y >= bgRect.y + bgRect.h) {

					HLIST tempItem;
					int tempPointEndY;
					tempPointEndY = pointEnd.y;
					tempItem = rectItem;
					/*pointEnd.y���ܻ�û�дﵽ��ο�λ��������ʼ�������*/
					while (hRect = RectCutGetNext()) {
						if (((HWIDGE_BASE)hRect)->isVisable == FALSE) { continue; }
						if (hRect->y > startPoint.y) {							/*�ϱ�*/
							/*�ڱ����þ��ε������޶���Χ��*/
							if (hRect->y > bgRect.y
								&& hRect->y < bgRect.y + bgRect.h) {
								if (hRect->y - startPoint.y < val) {				/*�������һ��*/
									val = hRect->y - startPoint.y;
									pointEnd.y = hRect->y;

								}
							}
						}
						else if (hRect->y + hRect->h > startPoint.y) {			/*�±�*/
							/*�ڱ����þ��ε������޶���Χ��*/
							if (hRect->y + hRect->h > bgRect.y
								&& hRect->y + hRect->h < bgRect.y + bgRect.h) {
								if ((hRect->y + hRect->h - startPoint.y) < val) {	/*�������һ��*/
									val = (hRect->y + hRect->h - startPoint.y);
									pointEnd.y = hRect->y + hRect->h;

								}
							}
						}
					}
					rectItem = tempItem;
					/*���û�б仯�����ʾ�Ҳ����ˣ��˳�*/
					if (tempPointEndY == pointEnd.y) {
						return NULL;
					}

				}
				/*���Ƴ����߽磬x�ص����*/
				startPoint.x = bgRect.x;
				/*Y�ص��ҵ��������һ����*/
				HLIST tempItem;
				int tempPointEndY;
				tempPointEndY = pointEnd.y;
				tempItem = rectItem;
				/*pointEnd.y���ܻ�û�дﵽ��ο�λ��������ʼ�������*/
				while (hRect = RectCutGetNext()) {
					if (((HWIDGE_BASE)hRect)->isVisable == FALSE) { continue; }
					if (hRect->y > startPoint.y) {							/*�ϱ�*/
							/*�ڱ����þ��ε������޶���Χ��*/
						if (hRect->y > bgRect.y 
							&& hRect->y < bgRect.y + bgRect.h) {
							if (hRect->y - startPoint.y < val) {				/*�������һ��*/
								val = hRect->y - startPoint.y;
								pointEnd.y = hRect->y;

							}
						}
					}
					else if (hRect->y + hRect->h > startPoint.y) {			/*�±�*/
						/*�ڱ����þ��ε������޶���Χ��*/
						if (hRect->y + hRect->h > bgRect.y 
							&& hRect->y + hRect->h < bgRect.y + bgRect.h) {
							if ((hRect->y + hRect->h - startPoint.y) < val) {	/*�������һ��*/
								val = (hRect->y + hRect->h - startPoint.y);
								pointEnd.y = hRect->y + hRect->h;

							}
						}
					}
				}
				//rectItem = tempItem;
				startPoint.y = pointEnd.y;

				/*������ɺ󣬼��õ���㷢���˱仯�������˻ؼ�����*/
				rectItem = rectCutList.next;

				/*��λ��ԭʼ���*/
				pointEnd.x = bgRect.w + bgRect.x;
				pointEnd.y = bgRect.y + bgRect.h;

				temp_w = 0;

				val = bgRect.y + bgRect.h;

				lastRightRect = NULL;
			}
			//fill_rect(startPoint.x, startPoint.y, 5, 5, rand() % 65535);
			/*�˻أ���Ϊ����֮ǰ�ľ��������ڵ���ײ������֮ǰ����㲢δ����*/
			rectItem = rectCutList.next;
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

		//fill_rect(startPoint.x, startPoint.y, 5, 5, rand() % 65535);

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

		//fill_rect(startPoint.x, startPoint.y, 5, 5, rand() % 65535);
	}
	//fill_rect(startPoint.x, startPoint.y, 5, 5, rand() % 65535);
	return &xResRect;
}