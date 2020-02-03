#ifndef _WIDGE_CALL_DEFINE_H__
#define _WIDGE_CALL_DEFINE_H__

#include "msg.h"
//���ʵĶ���
typedef struct {
	/*�������Ƶ�����,ȫ0XFFFF��ʾ��Ч����*/
	int16 x;
	int16 y;
	uint16 w;
	uint16 h;

	uintColor DrawColor;/*������ɫ*/
	uintColor DrawFrColor;/*ǰ��ɫ*/
	uintColor DrawBkColor;/*����ɫ*/
}*HPENCIL, PENCIL;

/*����*/
typedef struct {
	int16 x;
	int16 y;
	int16 w;
	int16 h;
}*HXRECT, XRECT;

/*��*/
typedef struct {
	int16 x;
	int16 y;
}*HXPOINT, XPOINT;

/*����*/
typedef struct {
	int16 x0;
	int16 y;
	int16 x1;
}*HXHLINE, XHLINE;



/*����ǿתΪ����*/
#define _PToHXRECTType(a) ((HXRECT)(a))

/*ˢ�º���*/
typedef void (*PaintFunction)(void* hObject);
/*�ƶ�����*/
typedef void (*WidgeMoveToFunction)(void* hObject, int16 x, int16 y);
/*�¼��ص�����*/
typedef int8(*WidgeCallBackFunction)(void* hObject, HMSGE hMsg);
/*�رպ���*/
typedef void (*WidgeCloseFunction)(void* hObject);
/*�����С����*/
typedef void (*WidgeResizeFunction)(void* hOjbect,int16 x,int16 y,uint16 w,uint16 h);

/*�����¼��ص�*/
typedef uint8(*WinProcessCallBack)(void* Object, void* arg, HMSGE hMsg);
/*��ť�¼��ص�*/
typedef void (*ViewClickCallBack)(void* Object, void* arg, uint8 status);

#endif