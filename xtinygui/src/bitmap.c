#include "bitmap.h"

uint8_t InitBitmap(p_xbitmap_t hXBitmap, uint8_t *pixels, uint16_t w, uint16_t h, uint8_t colorDepth)
{
	if (hXBitmap == NULL)
	{
		return FALSE;
	}

	hXBitmap->w = w;
	hXBitmap->h = h;
	hXBitmap->pixels = pixels;
	hXBitmap->bitsPerPixel = colorDepth;
	hXBitmap->flag = 0;
	hXBitmap->alpha = 255;
	return TRUE;
}
void BitmapSetFlag(p_xbitmap_t hXBitmap, uint8_t flag)
{
	if (hXBitmap == NULL)
	{
		return;
	}
	hXBitmap->flag = flag;
}
uint8_t InitBitmapFromFile(const char *file)
{

	return TRUE;
}
