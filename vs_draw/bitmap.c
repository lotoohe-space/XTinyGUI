#include "bitmap.h"



uint8 InitBitmap(HXBITMAP hXBitmap,uint8* pixels, uint16 w, uint16 h, uint8 colorDepth) {
	if (hXBitmap == NULL) { return FALSE; }

	hXBitmap->w = w;
	hXBitmap->h = h;
	hXBitmap->pixels = pixels;
	hXBitmap->bitsPerPixel = colorDepth;

	return TRUE;
}
uint8 InitBitmapFromFile(const char* file) {

	return TRUE;
}
