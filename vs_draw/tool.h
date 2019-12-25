#ifndef _TOOL_H__
#define _TOOL_H___

#include "msg.h"
#include "paint.h"
#define _GET_BIT(a,b) (((a)>>(b))&0x1)
#define _SET_BIT(a,b) (a)|=(1<<(b))
#define _CLR_BIT(a,b) (a)&=(~(1<<(b)))

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

extern int8 _inline isCollsionWithRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

	
//ĳ����Χ���ڵĴ�ֱ���Ƿ����·��ĺ����ཻ
#define _isRangeLinesCollsionLineD(x1,y1,w1,x2,y2,w2) ((y2 > y1)&&(((x2 >= x1 && x2 < (x1 + w1))) || ((x2 + w2) >= x1) && ((x2 + w1)<(x1 + w1))))//isRangeLinesCollsionLineD(x1,y1,w1,x2,y2,w2)
//�����Ƿ�������ཻ
#define _isLineHCollsionRect(x0,y,w,x1,y1,w1,h1) (!(!((y)>=(y1) && (y) < ((y1) + (h1)))  || (((x0) + (w) - 1) < (x1)) || ((x0) > ((x1) + (w1) - 1))))//isLineHCollsionRect( x0,  y,  w,  x1,  y1,  w1,  h1)
//������������Ƿ���ײ
#define _IsDrawCheckArea(x1,y1,w1,h1,x2,y2,w2,h2) isCollsionWithRect(x1,y1,w1,h1,x2,y2,w2,h2)
//���Ƿ��ھ�����
#define _IsDrawCheckPoint(x,y,x0,y0,w0,h0) (((x) >= (x0)) && ((x) < ((x0) + (w0))) && ((y)>=(y0)) && ((y)<((y0) + (h0))))//isInside( x,  y, x0, y0, w0, h0)
#define _IsDrawCheckPointR(xp,yp,a) _IsDrawCheckPoint((xp),(yp),(a->x),(a->y),(a->w),(a->h))
//����������ε��ص�����
#define _GetOverLapRect(x1,y1,w1,h1,a) \
rRect.x = MAX((x1), ((a)->x));\
rRect.y = MAX((y1), ((a)->y));\
rRect.w = MIN((x1) + (w1), ((a)->x) +((a)->w));\
rRect.h = MIN((y1) + (h1), ((a)->y) + ((a)->h));\
if ((rRect.x > rRect.w) || (rRect.y > rRect.h)) {\
rRect.x=(x1);rRect.y=(y1);rRect.w=0;rRect.h=0;\
}else{\
rRect.w-=rRect.x;rRect.h-=rRect.y;\
}
void   TMemcpy(void *dst, void *src, uint32 len);
uint32 TStrlen(char *str);

//���θ��ƺ���
#define XRECT_COPY(a,b) TMemcpy((a), (b), sizeof(XRECT))

extern BOOL  _inline GetOverLapRect(HXRECT r1, HXRECT r2, HXRECT res);




#endif
