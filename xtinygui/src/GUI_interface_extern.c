
#include "GUI_interface_extern.h"
#include "x_tool.h"
#include "color.h"
#ifdef WIN32
extern unsigned int g_pix(int x, int y);
extern void d_pix(int x, int y, int color);
#else
#include "lcd.h"
extern unsigned int sys_tick;
#endif

extern uint32_t GetCurrentTimeMsec(void);

uint32_t GUIGetTick(void)
{
	return (uint32_t)GetCurrentTimeMsec();
}
void GUIDrawPixel(int16_t x, int16_t y, uintColor color)
{
	d_pix(x, y, color);
}
uintColor GUIGetPixel(int16_t x, int16_t y)
{
	return g_pix(x, y);
}
void GUIDrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uintColor color)
{
	int16_t i, j;
	for (j = y; j < y + h; j++)
	{
		for (i = x; i < x + w; i++)
		{
			GUIDrawPixel(i, j, color);
		}
	}
}
extern void SDL2_GUIDrawBitmap(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t *bitmap);
void GUIDrawBitmap(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t *bitmap)
{
	SDL2_GUIDrawBitmap(x, y, w, h, bitmap);
	// int16_t i, j;
	// for (j = y; j < y + h; j++)
	// {
	// 	for (i = x; i < x + w; i++)
	// 	{
	// 		GUIDrawPixel(i, j, ((uint16_t *)(bitmap))[(i - x) + (j - y) * w]);
	// 	}
	// }
}
