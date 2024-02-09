
#ifndef _PAINT_CUT_H__
#define _PAINT_CUT_H__
#include "type.h"
#include "gui.h"

p_rlist_t RectCutAddRectList(p_rlist_t addRectList);
void RectCutSplitRectList(p_rlist_t splitRectList);

int8_t RectCutInit(void);
int8_t RectCutStart(p_xrect_t hXRECT);
p_xrect_t RectCutFind(void);
void RectCutEnd(void);
BOOL RectCutIsEnd(void);

#define RECT_CUT_INIT(a)                                \
	{                                                   \
		p_xrect_t nextCutRect;                          \
		RectCutStart(a);                                \
		while (((nextCutRect = RectCutFind()) != NULL)) \
		{

#define RECT_CUT_END()  \
	if (RectCutIsEnd()) \
	{                   \
		break;          \
	}                   \
	}                   \
	RectCutEnd();       \
	}

#endif
