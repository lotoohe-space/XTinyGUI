
#ifndef _PAINT_CUT_H__
#define _PAINT_CUT_H__
#include "type.h"
#include "gui.h"

HLIST RectCutAddRectList(HLIST addRectList);
void RectCutSplitRectList(HLIST splitRectList);

int8 RectCutInit(void);
int8 RectCutStart(HXRECT hXRECT);
HXRECT RectCutGetNext(void);
HXRECT RectCutFind(void);
void RectCutEnd(void);

extern BOOL isEnd;

//#define RECT_CUT_INIT(x,y,w,h) \
//	{HXRECT nextCutRect;\
//	if(RectCutStart((x), (y), (w), (h))){\
//		while ((nextCutRect = RectCutFind()) != NULL)
// #define RECT_CUT_ELSE() RectCutEnd();}else{
// #define RECT_CUT_END() }}
#define RECT_CUT_INIT(a)                                \
	{                                                   \
		HXRECT nextCutRect;                             \
		RectCutStart(a);                                \
		while (((nextCutRect = RectCutFind()) != NULL)) \
		{

#define RECT_CUT_END() \
	if (isEnd)         \
	{                  \
		break;         \
	}                  \
	}                  \
	RectCutEnd();      \
	}

#endif
