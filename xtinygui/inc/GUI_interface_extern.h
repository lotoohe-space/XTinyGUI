#ifndef _GUI_INTERFACE_EXTERN_H__
#define _GUI_INTERFACE_EXTERN_H__
#include "type.h"

uint32 GUIGetTick(void);
void GUIDrawPixel(int16 x, int16 y, uintColor color);
uintColor GUIGetPixel(int16 x, int16 y);
void GUIDrawRect(int16 x, int16 y, uint16 w, uint16 h, uintColor color);
void GUIDrawBitmap(int16 x, int16 y, uint16 w, uint16 h, uint8 *bitmap);

#endif
