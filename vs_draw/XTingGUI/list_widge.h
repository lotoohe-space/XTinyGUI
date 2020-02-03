#ifndef _LIST_WIDGE_H__
#define _LIST_WIDGE_H__
#include "group_widge.h"
#define LIST_WIDGE_MARK_HEAD(a) ListWidge##a


#define LIST_WIDGE_ITEM_SIZE 20
typedef struct {
	GROUP_WIDGE groupWidge;
	union {
		uint16		itemSize;		/*间距大小 定高定宽模式有效*/
		uint16		itemsH;			/*控件高度 高度自适应模式*/
	}itemsSize;
	int16		lastY;

	uint8		flag;			/*0:bit 定高定宽模式/高度自适应模式 1:bit 是否正在滑动*/
}*HLIST_WIDGE, LIST_WIDGE;

/*设置List显示模式*/
#define _SetListWidgeMode(a)	(_SET_BIT(((HLIST_WIDGE)(a))->flag,0))
#define _ClrListWidgeMode(a)	(_CLR_BIT(((HLIST_WIDGE)(a))->flag,0))
#define _GetListWidgeMode(a)	(_GET_BIT(((HLIST_WIDGE)(a))->flag,0))

/*是否正在滑动*/
#define _SetSliding(a)			(_SET_BIT(((HLIST_WIDGE)(a))->flag,1))
#define _ClrSliding(a)			(_CLR_BIT(((HLIST_WIDGE)(a))->flag,1))
#define _GetSlideState(a)		(_GET_BIT(((HLIST_WIDGE)(a))->flag,1))

PUBLIC HLIST_WIDGE LIST_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h);
PUBLIC uint8 LIST_WIDGE_MARK_HEAD(Add)(HLIST_WIDGE hBaseWidge, HWIDGE_BASE widge);
PUBLIC void LIST_WIDGE_MARK_HEAD(Paint)(void* hObject);
PUBLIC int8 LIST_WIDGE_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);
#endif
