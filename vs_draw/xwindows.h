#ifndef _XWINDOWS_H__
#define _XWINDOWS_H__
#include "type.h"
#include "paint.h"
#include "tool.h"
#include "color.h"
#include "list.h"
#include "widge.h"
#include "widge_define.h"
#include "xwindows_head.h"

/*���ڽṹ��*/
typedef struct {
	WIDGE_BASE winWidge;

	HLIST  widgetList;/*�ؼ��б�*/
	HWIN_HEAD hWinHead;/*����ͷ�ؼ�*/
	uint8 flag;/*
			   0bit:�Ƿ���Ʊ�����(1��ʾ����) 
			   1bit:�����ƶ���־λ  
			   2bit:�Ƿ���Ҫ���¼�������� 
			   3bit:�Ƿ�����С��
			   4bit:�Ƿ������
			   */

	/*��ʱƫ�������������ƶ���ʱ����Ҫ�õ�һ��ƫ�����������ƫ������ƫ���ڲ���ÿһ���ؼ�*/
	int16 t_dx;
	int16 t_dy;

	/*����ϴε�ƫ����*/
	XRECT lastRect;
	//int16 lastX;
	//int16 lastY;
}*HWIN,WIN;

//��ȡ�Ƿ����winͷ����־λ
#define _IsDrawWinHead(a)  (_GET_BIT(((HWIN)(a))->flag,0))
//������ʾ����ͷ
#define _SetDrawWinHead(a) (_SET_BIT(((HWIN)(a))->flag,0))
//����ʾͷ
#define _ClrDrawWinHead(a) (_CLR_BIT(((HWIN)(a))->flag,0))

//�Ƿ���Ҫ���¼��������
#define _SetDrawAllLag(a)	(_SET_BIT((a)->flag,2))
#define _ClrDrawAllLag(a)	(_CLR_BIT((a)->flag,2))
#define _IsDrawAllLag(a)	(_GET_BIT((a)->flag,2))

//�����Ƿ�����С��
#define _IsMinWIN(a)  (_GET_BIT(((HWIN)(a))->flag,3))
#define _SetMinWIN(a) (_SET_BIT(((HWIN)(a))->flag,3))
#define _ClrMinWIN(a) (_CLR_BIT(((HWIN)(a))->flag,3))

//�����Ƿ������
#define _IsMaxWIN(a)  (_GET_BIT(((HWIN)(a))->flag,4))
#define _SetMaxWIN(a) (_SET_BIT(((HWIN)(a))->flag,4))
#define _ClrMaxWIN(a) (_CLR_BIT(((HWIN)(a))->flag,4))

//���ô��������ƶ�
#define _SetWinMoveing(a)  (_SET_BIT((a)->flag,1))
//������������ƶ�
#define _ClrWinMoveing(a)  (_CLR_BIT((a)->flag,1))
//��ȡ�Ƿ����ƶ�
#define _IsWinMoving(a)	   (_GET_BIT((a)->flag,1))

HWIN WindowsCreate(char *title, int16 x, int16 y, int16 w, int16 h);
void WindowsClose(HWIN hWin);
void WindowsSetMax(HWIN hObject);
void WindowsSetMin(HWIN hObject);
void WindowsResize(HWIN hObject, int16 x, int16 y, uint16 w, uint16 h);
void WindowsSetVisable(void* hObject, int8 isVisable);
void WindowsPaint(void *hObject);
void WindowsMoveTo(HWIN hWin, int16 x, int16 y);
void WindowsSetColor(HWIN hWin, uintColor color);
int8 WindowsWidgeAdd(HWIN hWin, void *widge);
int8 WindowsCallBack(void* hObject, HMSGE hMsg);
void WindowsSetDrawHead(HWIN hWin, int8 isEnable);
void WindowsInvaildRect(HWIDGE_BASE hWidgeBase, HXRECT hXRect);
#endif
