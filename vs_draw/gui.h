
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"

/*�Ƿ���͸��ͨ��*/
#define USE_ALPHA	1

#define LCD_SCREEN_W 800
#define LCD_SCREEN_H 480

typedef struct {
	/*����Ҳ��һ�����ڣ�����û�д�ͷ�ؼ�*/
	HWIN desktopWin;

	/*��ͼ����*/
	XRECT drawArea;

	/*��ϵĴ���*/
	HWIN topWin;

	/*�����ƶ��Ĵ���*/
	XRECT movingWidge;

	
	//XRECT desktopRect;
	//PENCIL pencil;//����
	//XRECT invaildRect;
	/*HLIST winList;*/
	
}*HXDESKTOP, XDESKTOP;
extern HXDESKTOP hXDesktop;
//extern  XRECT drawArea;//��������


//void GUIInvaildRect(HXRECT hXRect);
void GUIExec(void);
int8 WinListAdd(HWIN hWin);
HWIN WinGetTop(void);
HWIN WinGetFocus(void);
HXDESKTOP GUIInit(void);
void GUIEvent(void);
void WinMoveTop(void* hObject);
//void GUIPostEvent(HMSGE hMsg);
BOOL IsGUINeedCut(HXRECT hXRECT);
void SetMovingWin(HXRECT hXRect);



#endif

