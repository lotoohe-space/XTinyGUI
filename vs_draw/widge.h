#ifndef _XWIDGE_H__
#define _XWIDGE_H__
#include "type.h"
#include "widge_define.h"

typedef struct {
	XRECT	rect;/*�ؼ���С*/
	PENCIL	pencil;/*���ʣ�������Ч��ͼ��С���Լ���ɫ*/
	PaintFunction paintFun;/*�ؼ�ˢ�º���*/
	WidgeMoveToFunction moveToFun;/*�ؼ��ƶ�����*/
	WidgeCallBackFunction widgeCallBackFun;/*�ؼ��¼��ص�����*/

	int8 isVisable;/*�Ƿ���ʾ*/
	void* parentHWIN;/*���ؼ������Խ��ؼ�����*/
	
}*HWIDGE_BASE, WIDGE_BASE;

#define _PToHWidgeBaseType(a) ((HWIDGE_BASE)a)

HWIDGE_BASE WidgeCreate(int16 x, int16 y, int16 w, int16 h);

void WidgeSetParentWin(HWIDGE_BASE hObject, void* hWIN);
void WidgeSetColor(HWIDGE_BASE hObject, uintColor color);
void WidgePaint(void* hObject);
void WidgeMoveTo(HWIDGE_BASE hObject, int16 x, int16 y);
//void WidgeSetArea(void* hObject, int16 x, int16 y, int16 w, int16 h);
int8 WidgeCallBack(void *hObject, HMSGE hMsg);

#endif
