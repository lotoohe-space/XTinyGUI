#include "table_view_widget.h"

PUBLIC p_table_widget_t TABLE_WIDGE_MARK_HEAD(Create)(int16_t x, int16_t y, uint16_t w, uint16_t h) {
	p_table_widget_t hObject;

	hObject = (p_table_widget_t)(xMalloc(sizeof(table_widget_t)));
	if (hObject == NULL) {
		return NULL;
	}
	GROUP_MARK_HEAD(Init)(hObject, x, y, w, h);

	return hObject;
}
/*添加一个控件*/
PUBLIC uint8_t TABLE_WIDGE_MARK_HEAD(Add)(p_table_widget_t hBaseWidge, p_widget_base_t widge) {
	uint16_t widgeLength;
	if (hBaseWidge == NULL || widge == NULL) { return FALSE; }

	if (GROUP_MARK_HEAD(Add)((p_group_widget_t)hBaseWidge, widge) == FALSE) {
		return FALSE;
	}
	widgeLength = ((p_group_widget_t)hBaseWidge)->widgeLength - 1;

	/*刷新*/
	WindowsInvaildRect((p_widget_base_t)hBaseWidge, NULL);
	return TRUE;
}
