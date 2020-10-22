#ifndef _LIST_WIDGE_H__
#define _LIST_WIDGE_H__
#include "group_widge.h"
#include "GUI_timeout.h"

#define LIST_WIDGE_MARK_HEAD(a) ListWidge##a

#define LIST_WIDGE_ITEM_SIZE 20
#define LIST_WIDGE_ITEM_GAP	 5

typedef struct {
	GROUP_WIDGE		groupWidge;		/*�̳�GROUP_WIDGE*/
	uint16			itemGap;		/*Ԫ��֮���Gap*/
	union {
		uint16		itemSize;		/*����С ���߶���ģʽ��Ч*/
		uint16		itemsH;			/*�ؼ��߶� �߶�����Ӧģʽ*/
	}itemsSize;
	int16			lastXY;			/*��һ�ε�λ��*/

	/*******����Ч����*******/
	uint16			firstTime;		/*��һʱ���*/
	int16			firstXY;
	int16			dSP;			/*�ٶ�ֵ*/
	HGUITIME_OUT	hGUITimeout;	/*��ʱʱ��*/
	/************************/

	uint8		flag;			/*0:bit ���߶���ģʽ/�߶�����Ӧģʽ  1:bit list���� 2:bit �������� 7:bit �Ƿ����ڻ���*/
}*HLIST_WIDGE, LIST_WIDGE;

/*list��ģʽ��Ӧλ*/
typedef enum {
	LIST_SHOW_MODE = 0,
	LIST_DIR = 1,
	LIST_OPEN_ANT = 2,
	LIST_SLIDING = 7
}LIST_WIDGE_FLAG;

/*����List��ʾģʽ*/
#define _SetListWidgeMode(a)	(_SET_BIT(((HLIST_WIDGE)(a))->flag,LIST_SHOW_MODE))
#define _ClrListWidgeMode(a)	(_CLR_BIT(((HLIST_WIDGE)(a))->flag,LIST_SHOW_MODE))
#define _GetListWidgeMode(a)	(_GET_BIT(((HLIST_WIDGE)(a))->flag,LIST_SHOW_MODE))


/*list����*/
#define _SetOTN_H(a)			(_SET_BIT(((HLIST_WIDGE)(a))->flag,LIST_DIR))
#define _SetOTN_V(a)			(_CLR_BIT(((HLIST_WIDGE)(a))->flag,LIST_DIR))
#define _GetOTN(a)				(_GET_BIT(((HLIST_WIDGE)(a))->flag,LIST_DIR))

/*��������*/
#define _OpenListAnimation(a)	(_SET_BIT(((HLIST_WIDGE)(a))->flag,LIST_OPEN_ANT))
#define _CloseListAnimation(a)	(_CLR_BIT(((HLIST_WIDGE)(a))->flag,LIST_OPEN_ANT))
#define _GetListAnimation(a)	(_GET_BIT(((HLIST_WIDGE)(a))->flag,LIST_OPEN_ANT))

/*�Ƿ����ڻ���*/
#define _SetSliding(a)			(_SET_BIT(((HLIST_WIDGE)(a))->flag,LIST_SLIDING))
#define _ClrSliding(a)			(_CLR_BIT(((HLIST_WIDGE)(a))->flag,LIST_SLIDING))
#define _GetSlideState(a)		(_GET_BIT(((HLIST_WIDGE)(a))->flag,LIST_SLIDING))

PUBLIC HLIST_WIDGE LIST_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h);
PUBLIC uint8 LIST_WIDGE_MARK_HEAD(Add)(HLIST_WIDGE hBaseWidge, HWIDGET_BASE widge);
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void* hObject);
PUBLIC void LIST_WIDGE_MARK_HEAD(Close)(HLIST_WIDGE hObject);
PUBLIC int8 LIST_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

PUBLIC void LIST_WIDGE_MARK_HEAD(SetFlag)(HLIST_WIDGE hBaseWidge, LIST_WIDGE_FLAG flagBisPoi, BOOL status);
#endif
