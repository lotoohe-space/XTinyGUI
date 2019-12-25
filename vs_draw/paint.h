#ifndef _XPAINT_H__
#define _XPAINT_H__

#include "type.h"
#include "widge_define.h"
#include "font.h"


void  DrawInvaildRect(HXRECT hXRect);
void  DrawSetArea(void* hObject);
void  DrawResetArea(void* hObject);
uint8 DrawPixel(uintColor color, int16 x, int16 y);
uint8 DrawLineH(HPENCIL hPencil, int16 x0, int16 y, int16 x1);
uint8 DrawLineV(HPENCIL hPencil, int16 x, int16 y0, int16 y1);
uint8 DrawRect(HPENCIL hPencil, HXRECT hXRECT);
uint8 DrawCutRect(void* hObject, HXRECT hXRECT);
uint8 DrawChar(HXRECT hXRectArea, HFONTF hFont, int16 x, int16 y, uint8 ch, uintColor color);
uint8 DrawString(HPENCIL hPencil, HFONTF hFont, HXRECT dHRect, int16 x, int16 y, uint8* text);
uint8 DrawCutString(void* hObject, HFONTF hFont, HXRECT border, uint8* text);



#endif 