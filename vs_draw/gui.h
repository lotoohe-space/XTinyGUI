
#ifndef _GUI_H__
#define _GUI_H__
#include "xwindows.h"
#include "type.h"
#include "paint.h"

#define LCD_SCREEN_W 800
#define LCD_SCREEN_H 480

typedef struct {
	/*����Ҳ��һ�����ڣ�����û�д�ͷ�ؼ�*/
	HWIN desktopWin;

	/*��ϵĴ���*/
	HWIN topWin;

	/*�����ƶ��Ĵ���*/
	HWIN winMoving;
	//XRECT desktopRect;
	//PENCIL pencil;//����
	//XRECT invaildRect;
	/*HLIST winList;*/
	
}*HXDESKTOP, XDESKTOP;

extern  XRECT drawArea;//��������


//void GUIInvaildRect(HXRECT hXRect);
void GUIExec(void);
int8 WinListAdd(HWIN hWin);
HWIN WinGetTop(void);
HWIN WinGetFocus(void);
HXDESKTOP GUIInit(void);
void GUIEvent(void);
void WinMoveTop(void* hObject);
void GUIPostEvent(HMSGE hMsg);
BOOL isGUINeedCut(HWIN hWin);
void setMovingWin(HWIN hWin);



#endif

