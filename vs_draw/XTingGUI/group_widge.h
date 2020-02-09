#ifndef _GROUP_WIDGE_H__
#define _GROUP_WIDGE_H__

#include "widge.h"
#include "list.h"

#define GROUP_MARK_HEAD(a) GroupWidge##a

/*组控件*/
typedef struct {
	WIDGE_BASE	widgeBase;		/*继承Widge*/
	HLIST		widgetList;		/*控件列表*/
	uint16		widgeLength;	/*控件个数*/
}*HGROUP_WIDGE, GROUP_WIDGE;

#define _PToHGroupWidgeType(a) ((HGROUP_WIDGE)a)

PUBLIC HGROUP_WIDGE GROUP_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h);
PUBLIC uint8		GROUP_MARK_HEAD(Init)(HGROUP_WIDGE hBaseWidge, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC void			GROUP_MARK_HEAD(Close)(HWIDGE_BASE hObject);
PUBLIC HWIDGE_BASE	GROUP_MARK_HEAD(GetWidge)(HGROUP_WIDGE hObject, uint16 index);
PUBLIC void			GROUP_MARK_HEAD(Resize)(HGROUP_WIDGE hObject, int16 x, int16 y, uint16 w, uint16 h);
PUBLIC uint8		GROUP_MARK_HEAD(Add)(HGROUP_WIDGE hBaseWidge, HWIDGE_BASE widge);
PUBLIC void			GROUP_MARK_HEAD(Paint)(void* hObject);
PUBLIC void			GROUP_MARK_HEAD(MoveTo)(HGROUP_WIDGE hObject, int16 x, int16 y);
PUBLIC int8			GROUP_MARK_HEAD(CallBack)(void* hObject, HMSGE hMsg);

#endif
