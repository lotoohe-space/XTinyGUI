
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"

/*�Ƿ���͸��ͨ��,����͸��ͨ���ή�������ٶ�*/
#define USE_ALPHA		0
/*��ʾ���Ĵ�С*/
#define LCD_SCREEN_W	800
#define LCD_SCREEN_H	480

/*********���ص��¼�˵��********/
#define RES_ASSERT_ERR		-1
#define RES_OK				0
#define RES_FAIL			1
#define RES_USED			10
/*******************************/

typedef struct {
	/*����Ҳ��һ�����ڣ�����û�д�ͷ�ؼ�*/
	HWIN desktopWin;
	/*��ͼ����*/
	XRECT drawArea;
	/*��ϵĴ���*/
	HWIN topWin;
	/*�����ƶ��Ŀؼ�*/
	XRECT movingRect;
}*HXDESKTOP, XDESKTOP;
extern HXDESKTOP hXDesktop;

/*��ǰ�ؼ�����ת��Ϊ�������*/
/*����ǰ��������ڸ��ؼ���ƫ��*/
#define CDE2OPPOSITE_X(a)		(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->x - ((HXRECT)(a))->x)
#define CDE2OPPOSITE_Y(a)		(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->y - ((HXRECT)(a))->y)

/*�õ���ǰ��������ڿؼ�������������*/
#define P_CDE2OPPOSITE_X(a,b)	((b) - ((HXRECT)((HWIDGE_BASE)(a)))->x)
#define P_CDE2OPPOSITE_Y(a,b)	((b) - ((HXRECT)((HWIDGE_BASE)(a)))->y)

/*�������ת��Ϊ��������*/
#define _OPPOSITE2CDE_X(a,b)	(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->x - (b))
#define _OPPOSITE2CDE_Y(a,b)	(((HXRECT)(((HWIDGE_BASE)(a))->parentHWIN))->y - (b))

/*�õ��ؼ����������С*/
#define WIDGE_X(a)				(((HXRECT)(a))->x)
#define WIDGE_Y(a)				(((HXRECT)(a))->y)
#define WIDGE_W(a)				(((HXRECT)(a))->w)
#define WIDGE_H(a)				(((HXRECT)(a))->h)

/*��ÿؼ��ĸ��ؼ�*/
#define WIDGE_PARENT(a)			((HWIDGE_BASE)(((HWIDGE_BASE)(a))->parentHWIN))

void GUIExec(void);
int8 WinListAdd(HWIDGE_BASE hWidgeBase);
HWIDGE_BASE WinGetTop(void);
HXDESKTOP GUIInit(void);
void GUIEvent(void);
void WinMoveTop(void* hObject);
BOOL IsGUINeedCut(HWIDGE_BASE hWidgeBase);
BOOL IsGUINeedCutEx(HWIN hWin);
void SetMovingRect(HXRECT hXRect);

/*���û�ͼ����*/
uint8 DrawSetArea(HWIDGE_BASE hWidgeBase);
/*ȡ������*/
void DrawResetArea(HWIDGE_BASE hWidgeBase);

#endif

