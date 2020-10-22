#include "table_view_widget.h"

PUBLIC HTABLE_WIDGE TABLE_WIDGE_MARK_HEAD(Create)(int16 x, int16 y, uint16 w, uint16 h) {
	HTABLE_WIDGE hObject;

	hObject = (HTABLE_WIDGE)(xMalloc(sizeof(TABLE_WIDGE)));
	if (hObject == NULL) {
		return NULL;
	}
	GROUP_MARK_HEAD(Init)(hObject, x, y, w, h);



	return hObject;
}
/*添加一个控件*/
PUBLIC uint8 TABLE_WIDGE_MARK_HEAD(Add)(HTABLE_WIDGE hBaseWidge, HWIDGET_BASE widge) {
	uint16 widgeLength;
	if (hBaseWidge == NULL || widge == NULL) { return FALSE; }

	if (GROUP_MARK_HEAD(Add)((HGROUP_WIDGE)hBaseWidge, widge) == FALSE) {
		return FALSE;
	}
	widgeLength = ((HGROUP_WIDGE)hBaseWidge)->widgeLength - 1;
	


	/*刷新*/
	WindowsInvaildRect((HWIDGET_BASE)hBaseWidge, NULL);
	return TRUE;
}
