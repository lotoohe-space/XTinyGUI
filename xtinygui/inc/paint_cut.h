
#ifndef _PAINT_CUT_H__
#define _PAINT_CUT_H__
#include "x_types.h"
#include "gui.h"

p_rlist_t RectCutAddRectList(p_rlist_t addRectList);
void RectCutSplitRectList(p_rlist_t splitRectList);

int8_t RectCutInit(void);
void *RectCutStart(p_xrect_t hXRECT);
p_xrect_t RectCutFind(void);
void RectCutEnd(void);
BOOL RectCutIsEnd(void);

#define RECT_CUT_FOREACH(pos, a) \
	for (RectCutStart(a); !RectCutIsEnd() && ((pos = RectCutFind()) != NULL);)

#endif
