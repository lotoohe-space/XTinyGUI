
#include "font.h"

const FONTF fontASCII8_12 = {
	0,
	8,
	12,
	0x0,
	0x7f,
	1,
	font_ASCII_8_12
};
const FONTF fontASCII12_16 = {
	0,
	12,
	16,
	0x0,
	0x7f,
	2,
	font_ASCII_12_16
};
//读取一个字到内存
uint8* FontReadChar(HFONTF hFont,uint8 ch) {
	if (hFont->type == 0) {
		if (ch > hFont->fontInfo.startInx&&ch < hFont->fontInfo.endInx) {
			return (uint8*)(&(hFont->fontInfo.fontVal.addrFont[ch * (hFont->fontInfo.h) * hFont->fontInfo.perRowBytes]));
		}
	}
	return NULL;
}
