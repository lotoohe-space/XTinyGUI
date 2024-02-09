#ifndef _TOOL_H__
#define _TOOL_H___

#include "msg.h"
#include "paint.h"
#include <string.h>
#define _GET_BIT(a, b) (((a) >> (b)) & 0x1)
#define _SET_BIT(a, b) (a) |= (1 << (b))
#define _CLR_BIT(a, b) (a) &= (~(1 << (b)))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern int8_t isCollsionWithRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern BOOL GetOverLapRect(p_xrect_t r1, p_xrect_t r2, p_xrect_t res);

// 某个范围内内的垂直线是否与下方的横线相交
#define _isRangeLinesCollsionLineD(x1, y1, w1, x2, y2, w2) ((y2 > y1) && (((x2 >= x1 && x2 < (x1 + w1))) || ((x2 + w2) >= x1) && ((x2 + w1) < (x1 + w1)))) // isRangeLinesCollsionLineD(x1,y1,w1,x2,y2,w2)
// 横线是否与矩形相交
#define _isLineHCollsionRect(x0, y, w, x1, y1, w1, h1) (!(!((y) >= (y1) && (y) < ((y1) + (h1))) || (((x0) + (w)-1) < (x1)) || ((x0) > ((x1) + (w1)-1)))) // isLineHCollsionRect( x0,  y,  w,  x1,  y1,  w1,  h1)
// 检查两个矩形是否碰撞
#define _IsDrawCheckArea(x1, y1, w1, h1, x2, y2, w2, h2) isCollsionWithRect(x1, y1, w1, h1, x2, y2, w2, h2)
// 点是否在矩形内
#define _IsDrawCheckPoint(x, y, x0, y0, w0, h0) (((x) >= (x0)) && ((x) < ((x0) + (w0))) && ((y) >= (y0)) && ((y) < ((y0) + (h0)))) // isInside( x,  y, x0, y0, w0, h0)
#define _IsDrawCheckPointR(xp, yp, a) _IsDrawCheckPoint((xp), (yp), ((a)->x), ((a)->y), ((a)->w), ((a)->h))
// 获得两个矩形的重叠部分
#define _GetOverLapRect(x1, y1, w1, h1, a)           \
    rRect.x = MAX((x1), ((a)->x));                   \
    rRect.y = MAX((y1), ((a)->y));                   \
    rRect.w = MIN((x1) + (w1), ((a)->x) + ((a)->w)); \
    rRect.h = MIN((y1) + (h1), ((a)->y) + ((a)->h)); \
    if ((rRect.x > rRect.w) || (rRect.y > rRect.h))  \
    {                                                \
        rRect.x = (x1);                              \
        rRect.y = (y1);                              \
        rRect.w = 0;                                 \
        rRect.h = 0;                                 \
    }                                                \
    else                                             \
    {                                                \
        rRect.w -= rRect.x;                          \
        rRect.h -= rRect.y;                          \
    }
/*获取gbk或者gb2312的长度*/
uint32_t GBK_Strlen(const char *str);
uint32_t UNI_Strlen(uint16_t *str);
const char *GBK_CharGet(const char *str, uint16_t index);

// 矩形复制函数
#define XRECT_COPY(a, b) memcpy((a), (b), sizeof(xrect_t))

#endif
