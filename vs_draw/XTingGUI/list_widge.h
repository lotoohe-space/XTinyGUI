#ifndef _LIST_WIDGE_H__
#define _LIST_WIDGE_H__
#include "group_widge.h"
#define LIST_WIDGE_MARK_HEAD(a) ListWidge##a


#define LIST_WIDGE_ITEM_SIZE 20
typedef struct {
	GROUP_WIDGE groupWidge;
	union {
		uint16		itemSize;		/*����С ���߶���ģʽ��Ч*/
		uint16		itemsH;			/*�ؼ��߶� �߶�����Ӧģʽ*/
	}itemsSize;
	int16		lastY;

	uint8		flag;			/*0:bit ���߶���ģʽ/�߶�����Ӧģʽ 1:bit �Ƿ����ڻ���*/
}*HLIST_WIDGE, LIST_WIDGE;

/*����List��ʾģʽ*/
#define _SetListWidgeMode(a)	(_SET_BIT(((HLIST_WIDGE)(a))->flag,0))
#define _ClrListWidgeMode(a)	(_CLR_BIT(((HLIST_WIDGE)(a))->flag,0))
#define _GetListWidgeMode(a)	(_GET_BIT(((HLIST_WIDGE)(a))->flag,0))

/*�Ƿ����ڻ���*/
#define _SetSliding(a)			(_SET_BIT(((HLIST_WIDGE)(a))->flag,1))
#define _ClrSliding(a)			(_CLR_BIT(((HLIST_WIDGE)(a))->flag,1))
#define _GetSlideState(a)		(_GET_BIT(((HLIST_WIDGE)(a))->flag,1))

PUBLIC HLIST_WIDGE LIST_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h);
PUBLIC uint8 LIST_WIDGE_MARK_HEAD(Add)(HLIST_WIDGE hBaseWidge, HWIDGE_BASE widge);
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 LIST_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
#endif
