#ifndef _XPAINT_H__
#define _XPAINT_H__

#include "type.h"
#include "widge_define.h"
#include "font.h"
#include "bitmap.h"

uintColor _GetPixel(int16_t x, int16_t y);
void DrawPixel(uintColor color, int16_t x, int16_t y);
void DrawAPixel(uintColor aColor, int16_t x, int16_t y);
// uint8_t DrawLineH(p_pencil_t hPencil, int16_t x0, int16_t y, int16_t x1);
// uint8_t DrawLineV(p_pencil_t hPencil, int16_t x, int16_t y0, int16_t y1);
uint8_t DrawRect(p_pencil_t hPencil, p_xrect_t hXRECT);
uint8_t DrawCutRect(void *hObject, p_xrect_t hXRECT);
uint8_t DrawChar(p_pencil_t hPencil, p_font_t hFont, p_xrect_t border, p_xrect_t bgBorder, char ch);
uint8_t DrawString(p_pencil_t hPencil, p_font_t hFont, p_xrect_t dHRect, int16_t x, int16_t y, uint8_t *text);

uint8_t DrawCutChar(void *hObject, p_font_t hFont, p_xrect_t bgRect, p_xpoint_t hStartXPoint, char ch);
uint8_t DrawCutString(void *hObject, p_font_t hFont, p_xrect_t border, p_xpoint_t hXPoint, const char *text);
uint8_t DrawCutBitmap(void *hObject, p_xrect_t border, p_xbitmap_t hXBitmap);

#endif
