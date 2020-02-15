
#include "GUI_interface_extern.h"
#include "tool.h"
#include "color.h"
#ifdef WIN32
extern unsigned int g_pix(int x, int y);
extern void d_pix(int x, int y, int color);
#endif


//volatile uint32 GUITick = 0;
extern unsigned long long GetCurrentTimeMsec();
uint32 GUIGetTick(void) {
	return (uint32)GetCurrentTimeMsec();
}

void GUIDrawPixel(int16  x,int16 y,uintColor color) {
	d_pix(x, y, RGB565TORGB888(color));
}
uintColor GUIGetPixel(int16 x, int16 y) {
	return RGB888T0RGB565(g_pix(x, y));
}
void GUIDrawBitmap(int16 x, int16 y, uint16 w, uint16 h, uint8* bitmap) {
	int16 i, j;
	for (j = y; j < y + h; j++) {
		for (i = x; i < x + w; i++) {
			GUIDrawPixel(i,j, ((uint16*)(bitmap))[(i- x) + (j- y) * w]);
		}
	}
}
