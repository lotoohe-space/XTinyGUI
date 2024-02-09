#ifndef _GUI_INTERFACE_EXTERN_H__
#define _GUI_INTERFACE_EXTERN_H__
#include "type.h"

uint32_t GUIGetTick(void);
void GUIDrawPixel(int16_t x, int16_t y, uintColor color);
uintColor GUIGetPixel(int16_t x, int16_t y);
void GUIDrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uintColor color);
void GUIDrawBitmap(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t *bitmap);

#endif
