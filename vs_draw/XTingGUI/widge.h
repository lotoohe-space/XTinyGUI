#ifndef _XWIDGE_H__
#define _XWIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "tool.h"

#define _WIDGET(a) Widget##a

typedef struct {
	XRECT	rect;/*�ؼ���С*/
	PENCIL	pencil;/*���ʣ�������Ч��ͼ��С���Լ���ɫ*/

	/*�ڲ�����ʹ��*/
	PaintFunction paintFun;/*�ؼ�ˢ�º���*/
	WidgeMoveToFunction moveToFun;/*�ؼ��ƶ�����*/
	WidgeCallBackFunction widgeCallBackFun;/*�ؼ��¼��ص�����*/
	WidgeCloseFunction widgeCloseFun;/*�ؼ��رպ���*/
	WidgeResizeFunction widgeResizeFun;/*�ؼ������С����*/

	/*�ⲿ���ã�����¼��ص����������пؼ����е�����*/
	ViewClickCallBack viewClickCallBack;
	/*���Դ���һ������*/
	void* arg;

	void* parentHWIN;/*���ؼ�*/
	uint8 flag;/*
			   0:bit �Ƿ�Ϊ���� 
			   1:bit �Ƿ񱻵�� 
			   2:bit �Ƿ���ʾ 
			   3:bit �Ƿ��н���(��δʵ��)
			   4:bit �Ƿ���Ҫ����͸������
			   5:bit �Ƿ�(��group_widge����Ҫ���⴦��)
			   */

}*HWIDGET_BASE, WIDGET_BASE;

/*�ؼ��Ƿ���Ҫ��������*/
#define _SET_GROUP_CTRL(a)		_SET_BIT((((HWIDGET_BASE)(a))->flag),5)
#define _CLR_GROUP_CTRL(a)		_CLR_BIT((((HWIDGET_BASE)(a))->flag),5)
#define _GET_GROUP_CTRL(a)		_GET_BIT((((HWIDGET_BASE)(a))->flag),5)

/*�ؼ��Ƿ���Ҫ����͸������*/
#define _SET_IS_DPY(a)			_SET_BIT((((HWIDGET_BASE)(a))->flag),4)
#define _CLR_IS_DPY(a)			_CLR_BIT((((HWIDGET_BASE)(a))->flag),4)
#define _GET_IS_DPY(a)			_GET_BIT((((HWIDGET_BASE)(a))->flag),4)

/*��widge�ǲ��Ǵ���*/
#define _SET_IS_WIN(a)			_SET_BIT((((HWIDGET_BASE)(a))->flag),0)
#define _CLR_IS_WIN(a)			_CLR_BIT((((HWIDGET_BASE)(a))->flag),0)
#define _GET_IS_WIN(a)			_GET_BIT((((HWIDGET_BASE)(a))->flag),0)

/*�Ƿ񱻵��*/
#define _SetBtnPress(a)			_SET_BIT(((HWIDGET_BASE)(a))->flag,1)
#define _SetBtnRelease(a)		_CLR_BIT(((HWIDGET_BASE)(a))->flag,1)
#define _GetBtnStatus(a)		_GET_BIT(((HWIDGET_BASE)(a))->flag,1)

/*�Ƿ���ʾ*/
#define _SetVisable(a)			_SET_BIT(((HWIDGET_BASE)(a))->flag,2)
#define _ClrVisable(a)			_CLR_BIT(((HWIDGET_BASE)(a))->flag,2)
#define _GetVisable(a)			_GET_BIT(((HWIDGET_BASE)(a))->flag,2)

#define _PToHWidgeBaseType(a) ((HWIDGET_BASE)a)	/*ת��ΪHWIDGE_BASE����*/

PUBLIC	HWIDGET_BASE	_WIDGET(Create)(int16 x, int16 y, int16 w, int16 h);
PUBLIC	HWIDGET_BASE	_WIDGET(CreateEx)(HXRECT hXRect);
PUBLIC	void			_WIDGET(Close)(HWIDGET_BASE hObject);
PUBLIC	void			_WIDGET(Init)(HWIDGET_BASE hWidgeBase, int16 x, int16 y, int16 w, int16 h);
PUBLIC	void			_WIDGET(SetClickBack)(HWIDGET_BASE hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC	void			_WIDGET(SetVisable)(HWIDGET_BASE hObject, uint8 isVisable);
PUBLIC	void			_WIDGET(Resize)(HWIDGET_BASE hObject, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC	void			_WIDGET(SetParentWin)(HWIDGET_BASE hObject, void* hWIN);
PUBLIC	void			_WIDGET(SetColor)(HWIDGET_BASE hObject, uintColor color);
PUBLIC	void			_WIDGET(MoveTo)(HWIDGET_BASE hObject, int16 x, int16 y);
PRIVATE void			_WIDGET(Paint)(void* hObject);
PUBLIC	int8			_WIDGET(CallBack)(void *hObject, HMSGE hMsg);

#endif
