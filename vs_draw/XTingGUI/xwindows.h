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

#define WINDOWS_MARK_HEAD(a) Windows##a

/*���ڽṹ��*/
typedef struct {
	GROUP_WIDGE groupWidge;/*�̳�GROUP_WIDGE*/

	HWIN_HEAD hWinHead;/*����ͷ�ؼ�*/
	/*��ʱƫ�������������ƶ���ʱ����Ҫ�õ�һ��ƫ�����������ƫ������ƫ���ڲ���ÿһ���ؼ�*/
	int16 t_dx;
	int16 t_dy;

	/*֮ǰ���ڵĴ�С*/
	XRECT lastRect;
	/*��ʱ�洢*/
	int16 t_xy;
	/*windows����ص�����*/
	WinProcessCallBack winProcessFun;

	uint8 flag;/*
			   0bit:�Ƿ���Ʊ����� (1��ʾ����) 
			   1bit:�����ƶ���־λ����־�����Ƿ������ƶ�
			   2bit:�Ƿ���Ҫ���¼�������� 
			   3bit:�Ƿ�����С��
			   4bit:�Ƿ������
			   5bit:�Ƿ������϶����ں����С
			   6bit:�Ƿ������϶����������С
			   7bit:�Ƿ������϶�����������С
			   */
}*HWIN,WIN;

//��ȡ�Ƿ����winͷ����־λ
#define _IsDrawWinHead(a)  (_GET_BIT(((HWIN)(a))->flag,0))
//������ʾ����ͷ
#define _SetDrawWinHead(a) (_SET_BIT(((HWIN)(a))->flag,0))
//����ʾͷ
#define _ClrDrawWinHead(a) (_CLR_BIT(((HWIN)(a))->flag,0))

//�Ƿ���Ҫ���¼��������
#define _SetDrawAllLag(a)	(_SET_BIT(((HWIN)(a))->flag,2))
#define _ClrDrawAllLag(a)	(_CLR_BIT(((HWIN)(a))->flag,2))
#define _IsDrawAllLag(a)	(_GET_BIT(((HWIN)(a))->flag,2))

//�����Ƿ�����С��
#define _IsMinWIN(a)  (_GET_BIT(((HWIN)(a))->flag,3))
#define _SetMinWIN(a) (_SET_BIT(((HWIN)(a))->flag,3))
#define _ClrMinWIN(a) (_CLR_BIT(((HWIN)(a))->flag,3))

//�����Ƿ������
#define _IsMaxWIN(a)  (_GET_BIT(((HWIN)(a))->flag,4))
#define _SetMaxWIN(a) (_SET_BIT(((HWIN)(a))->flag,4))
#define _ClrMaxWIN(a) (_CLR_BIT(((HWIN)(a))->flag,4))

/*�Ƿ������϶����ں����С*/
#define _IsChangeWinRHSize(a)  (_GET_BIT(((HWIN)(a))->flag,5))
#define _SetChangeWinRHSize(a) (_SET_BIT(((HWIN)(a))->flag,5))
#define _ClrChangeWinRHSize(a) (_CLR_BIT(((HWIN)(a))->flag,5))

#define _IsChangeWinVSize(a)  (_GET_BIT(((HWIN)(a))->flag,6))
#define _SetChangeWinVSize(a) (_SET_BIT(((HWIN)(a))->flag,6))
#define _ClrChangeWinVSize(a) (_CLR_BIT(((HWIN)(a))->flag,6))

#define _IsChangeWinLHSize(a)  (_GET_BIT(((HWIN)(a))->flag,7))
#define _SetChangeWinLHSize(a) (_SET_BIT(((HWIN)(a))->flag,7))
#define _ClrChangeWinLHSize(a) (_CLR_BIT(((HWIN)(a))->flag,7))

//���ô��������ƶ�
#define _SetWinMoveing(a)  (_SET_BIT((a)->flag,1))
//������������ƶ�
#define _ClrWinMoveing(a)  (_CLR_BIT((a)->flag,1))
//��ȡ�Ƿ����ƶ�
#define _IsWinMoving(a)	   (_GET_BIT((a)->flag,1))

HWIN			WINDOWS_MARK_HEAD(Create)(char *title, int16 x, int16 y, int16 w, int16 h);
void			WINDOWS_MARK_HEAD(Close)(HWIN hWin);
void			WINDOWS_MARK_HEAD(SetProcessCallBack)(HWIN hObject, WinProcessCallBack winProcessFun);
void			WINDOWS_MARK_HEAD(SetMax)(HWIN hObject);
void			WINDOWS_MARK_HEAD(SetMin)(HWIN hObject);
HWIDGET_BASE	WINDOWS_MARK_HEAD(GetWidge)(HWIN hObject, uint16 index);
void			WINDOWS_MARK_HEAD(Resize)(HWIN hObject, int16 x, int16 y, uint16 w, uint16 h);
void			WINDOWS_MARK_HEAD(SetVisable)(void* hObject, int8 isVisable);
void			WINDOWS_MARK_HEAD(Paint)(void *hObject);
void			WINDOWS_MARK_HEAD(MoveTo)(HWIN hWin, int16 x, int16 y);
void			WINDOWS_MARK_HEAD(SetColor)(HWIN hWin, uintColor color);
int8			WINDOWS_MARK_HEAD(Add)(HWIN hWin, void *widge);
int8			WINDOWS_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
void			WINDOWS_MARK_HEAD(SetDrawHead)(HWIN hWin, int8 isEnable);
void			WINDOWS_MARK_HEAD(InvaildRect)(HWIDGET_BASE hWidgeBase, HXRECT hXRect);
#endif
