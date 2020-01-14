#pragma once
#ifndef _BITMAP_H__
#define _BITMAP_H__
#include "type.h"

/*Bitmap*/
typedef struct {

	uint16	w;
	uint16	h;
	uint8* pixels;
	uint8	bitsPerPixel;	/*һ�����ض���λ*/

}*HXBITMAP, XBITMAP;


uint8 InitBitmap(HXBITMAP hXBitmap, uint8* pixels, uint16 w, uint16 h, uint8 colorDepth);

#endif
