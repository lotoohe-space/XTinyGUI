
#include "GUI_interface_extern.h"
#include "tool.h"
#include "color.h"
#ifdef WIN32
extern unsigned int g_pix(int x, int y);
extern void d_pix(int x, int y, int color);
#else
#include "lcd.h"
extern unsigned int sys_tick;
#endif

extern uint32 GetCurrentTimeMsec(void);

uint32 GUIGetTick(void) {
	return (uint32)GetCurrentTimeMsec();
}
void GUIDrawPixel(int16  x, int16 y, uintColor color) {
	d_pix(x, y, color);
}
uintColor GUIGetPixel(int16 x, int16 y) {
	return g_pix(x, y);
}
void GUIDrawRect(int16 x, int16 y, uint16 w, uint16 h, uintColor color) {
	int16 i, j;
	for (j = y; j < y + h; j++) {
		for (i = x; i < x + w; i++) {
			GUIDrawPixel(i, j, color);
		}
	}
}
void GUIDrawBitmap(int16 x, int16 y, uint16 w, uint16 h, uint8* bitmap) {
	int16 i, j;
	for (j = y; j < y + h; j++) {
		for (i = x; i < x + w; i++) {
			GUIDrawPixel(i, j, ((uint16*)(bitmap))[(i - x) + (j - y) * w]);
		}
	}
}
