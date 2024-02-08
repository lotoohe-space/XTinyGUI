
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"
#include "mem_dev.h"


#define USE_ALPHA			1	/*�Ƿ���͸��ͨ��,����͸��ͨ���ή�������ٶ�*/
#define USE_MEM_DEV			1	/*�Ƿ����洢�豸*/
#define USE_CURSOR			0	/*�Ƿ����α�*/

#define LCD_CACHE_LINE		1	/*����������*/

/*��ʾ���Ĵ�С*/
#define LCD_SCREEN_W		1024
#define LCD_SCREEN_H		748

/*********���ص��¼�˵��********/
#define RES_ASSERT_ERR		-1
#define RES_OK				0
#define RES_FAIL			1
#define RES_USED			10
/*******************************/

typedef struct {
	/*�̳�HWIN ����Ҳ��һ�����ڣ�����û�д�ͷ�ؼ�*/
	HWIN desktopWin;
	/*��ϵĴ���*/
	HWIN topWin;
	/*��Ҫ���µ�����*/
	XRECT updateArea;
	/*��������ĸ��ؼ�*/
	HWIDGET_BASE updateParentWidge;

#if USE_MEM_DEV
	/*�洢�豸*/
	HMEM_DEV	hMemDev;
#endif
	uint8 flag;/*0:bit �Ƿ��������*/

}*HXDESKTOP, XDESKTOP;
extern HXDESKTOP hXDesktop;

/*�״�����*/
#define _SET_FIRST_RUN(a)		_SET_BIT((((HXDESKTOP)(a))->flag),0)
#define _CLR_FIRST_RUN(a)		_CLR_BIT((((HXDESKTOP)(a))->flag),0)
#define _GET_FIRST_RUN(a)		_GET_BIT((((HXDESKTOP)(a))->flag),0)

/*��ȡһ���ؼ��Ļ�ͼ����*/
#define DESKTOP_DRAW_AREA(a)	((HXRECT)((&((a)->desktopWin->groupWidge.widgeBase.pencil))))

/*��ǰ�ؼ�����ת��Ϊ�������*/
/*����ǰ��������ڸ��ؼ���ƫ��*/
#define CDE2OPPOSITE_X(a)		(((HXRECT)(((HWIDGET_BASE)(a))->parentHWIN))->x - ((HXRECT)(a))->x)
#define CDE2OPPOSITE_Y(a)		(((HXRECT)(((HWIDGET_BASE)(a))->parentHWIN))->y - ((HXRECT)(a))->y)

/*�õ���ǰ��������ڿؼ�������������*/
#define P_CDE2OPPOSITE_X(a,b)	((b) - ((HXRECT)((HWIDGET_BASE)(a)))->x)
#define P_CDE2OPPOSITE_Y(a,b)	((b) - ((HXRECT)((HWIDGET_BASE)(a)))->y)

/*�������ת��Ϊ��������*/
#define _OPPOSITE2CDE_X(a,b)	(((HXRECT)(((HWIDGET_BASE)(a))->parentHWIN))->x - (b))
#define _OPPOSITE2CDE_Y(a,b)	(((HXRECT)(((HWIDGET_BASE)(a))->parentHWIN))->y - (b))

/*�õ��ؼ����������С*/
#define WIDGE_X(a)				(((HXRECT)(a))->x)
#define WIDGE_Y(a)				(((HXRECT)(a))->y)
#define WIDGE_W(a)				(((HXRECT)(a))->w)
#define WIDGE_H(a)				(((HXRECT)(a))->h)

/*��ÿؼ��ĸ��ؼ�*/
#define WIDGE_PARENT(a)			((HWIDGET_BASE)(((HWIDGET_BASE)(a))->parentHWIN))

HXDESKTOP		GUIInit(void);
void			GUIUpdate(HWIDGET_BASE hWidgeBase, HXRECT hXRect);
void			GUIExec(void);
void			GUIEvent(void);
uint32			GUIGetTick(void);
int8			WinListAdd(HWIDGET_BASE hWidgeBase);
HWIDGET_BASE	WinGetTop(void);
void			WinMoveTop(void* hObject);

BOOL			IsGUINeedCut(HWIDGET_BASE hWidgeBase);
void			SetUpdateArea(HWIDGET_BASE hWidgeBase, HXRECT hXRect);

uint8			DrawSetArea(HWIDGET_BASE hWidgeBase);/*���û�ͼ����*/
void			DrawResetArea(HWIDGET_BASE hWidgeBase);/*ȡ������*/

#endif

