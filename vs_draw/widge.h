#ifndef _XWIDGE_H__
#define _XWIDGE_H__
#include "type.h"
#include "widge_define.h"
#include "tool.h"

#define WIDGE_MARK_HEAD(a) Widge##a

typedef struct {
	XRECT	rect;/*�ؼ���С*/
	PENCIL	pencil;/*���ʣ�������Ч��ͼ��С���Լ���ɫ*/

	/*�ڲ�����ʹ��*/
	PaintFunction paintFun;/*�ؼ�ˢ�º���*/
	WidgeMoveToFunction moveToFun;/*�ؼ��ƶ�����*/
	WidgeCallBackFunction widgeCallBackFun;/*�ؼ��¼��ص�����*/
	WidgeCloseFunction widgeCloseFun;/*�ؼ��رպ���*/

	/*�ⲿ���ã�����¼��ص����������пؼ����е�����*/
	ViewClickCallBack viewClickCallBack;
	void* arg;

	void* parentHWIN;/*���ؼ�*/
	uint8 isVisable;/*�Ƿ���ʾ*/
	uint8 flag;/*0:bit �Ƿ�Ϊ���� 1:bit �Ƿ񱻵��*/

}*HWIDGE_BASE, WIDGE_BASE;

/*��widge�ǲ��Ǵ���*/
#define _SET_IS_WIN(a)			_SET_BIT((((HWIDGE_BASE)(a))->flag),0)
#define _CLR_IS_WIN(a)			_CLR_BIT((((HWIDGE_BASE)(a))->flag),0)
#define _GET_IS_WIN(a)			_GET_BIT((((HWIDGE_BASE)(a))->flag),0)

/*�Ƿ񱻵��*/
#define _SetBtnPress(a)			_SET_BIT(((HWIDGE_BASE)(a))->flag,1)
#define _SetBtnRelease(a)		_CLR_BIT(((HWIDGE_BASE)(a))->flag,1)
#define _GetBtnStatus(a)		_GET_BIT(((HWIDGE_BASE)(a))->flag,1)

#define _PToHWidgeBaseType(a) ((HWIDGE_BASE)a)

PUBLIC HWIDGE_BASE WIDGE_MARK_HEAD(Create)(int16 x, int16 y, int16 w, int16 h);
PUBLIC HWIDGE_BASE WIDGE_MARK_HEAD(CreateEx)(HXRECT hXRect);
PUBLIC void WIDGE_MARK_HEAD(Close)(HWIDGE_BASE hObject);
PUBLIC void WIDGE_MARK_HEAD(Init)(HWIDGE_BASE hWidgeBase, int16 x, int16 y, int16 w, int16 h);
PUBLIC void WIDGE_MARK_HEAD(SetClickBack)(HWIDGE_BASE hObject, void* arg, ViewClickCallBack viewClickCallBack);
PUBLIC void WIDGE_MARK_HEAD(SetVisable)(HWIDGE_BASE hObject, uint8 isVisable);
PUBLIC void WIDGE_MARK_HEAD(Resize)(HWIDGE_BASE hObject, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC void WIDGE_MARK_HEAD(SetParentWin)(HWIDGE_BASE hObject, void* hWIN);
PUBLIC void WIDGE_MARK_HEAD(SetColor)(HWIDGE_BASE hObject, uintColor color);
PUBLIC void WIDGE_MARK_HEAD(MoveTo)(HWIDGE_BASE hObject, int16 x, int16 y);
PRIVATE void WIDGE_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 WIDGE_MARK_HEAD(CallBack)(void *hObject, HMSGE hMsg);

#endif
