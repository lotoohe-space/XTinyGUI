
#include "font.h"
#include "font_ASCII_8_12.h"
#include "font_ASCII_12_16.h"
#include "font_ASCII_16_16.h"
#include "font_ASCII_100_100.h"
#include "font_GB2312_16_16.h"
#include "font_UNICODE_16_16.h"
const FONTF fontASCII8_12 = {
	8,
	12,
	0x0,
	0x7f,
	1,
	font_ASCII_8_12,
	NULL,
	0,
	0,
};
const FONTF fontASCII12_16 = {
	12,
	16,
	0x0,
	0x7f,
	2,
	font_ASCII_12_16,
	NULL,
	0,
	0,
};
const FONTF fontASCII16_16 = {
	16,
	16,
	0x0,
	0x7f,
	2,
	font_ASCII_16_16,
	NULL,
	0,
	0,
};
const FONTF fontASCII100_100 = {
	96,
	96,
	0x20,
	0x7f,
	12,
	font_ASCII_100_100,
	NULL,
	0,
	0,
};
const FONTF fontGB231216_16 = {
	16,
	16,
	0xa1a1,
	0xfefe,
	2,
	font_GB2312_16_16,
	&fontASCII16_16,
	0,
	1,
};
const FONTF fontUNICODE16_16 = {
	16,
	16,
	0x0000,
	0xffff,
	2,
	font_UNICODE_16_16,
	NULL,
	0,
	3,
};
// 读取一个字到内存
uint8 *FontReadChar(HFONTF hFont, uint16 ch)
{
	if (hFont->typePos == 0)
	{
		if (hFont->fontType == 0)
		{
			return (uint8 *)(&(hFont->fontInfo.fontVal.addrFont[(ch - (hFont->fontInfo.startInx)) * (hFont->fontInfo.h) * hFont->fontInfo.perRowBytes]));
		}
		else if (hFont->fontType == 1)
		{
			HFONT_INFO additionFont;
			additionFont = hFont->additionFont;
			if (ch > hFont->fontInfo.startInx && ch < hFont->fontInfo.endInx)
			{
				/*中文字*/
				if (ch & 0x0080)
				{
					return (uint8 *)(&(hFont->fontInfo.fontVal.addrFont[(((ch >> 8) - ((hFont->fontInfo.startInx) >> 8)) * 94 + ((ch & 0xff) - (hFont->fontInfo.startInx) & 0xff)) * (hFont->fontInfo.h) * hFont->fontInfo.perRowBytes]));
				}
			}
			else if (ch > additionFont->startInx && ch < additionFont->endInx)
			{
				return (uint8 *)(&(additionFont->fontVal.addrFont[(ch - (additionFont->startInx)) * (additionFont->h) * additionFont->perRowBytes]));
			}
		}
		else if (hFont->fontType == 3)
		{
			return (uint8 *)(&(hFont->fontInfo.fontVal.addrFont[(ch - (hFont->fontInfo.startInx)) * (hFont->fontInfo.h) * hFont->fontInfo.perRowBytes]));
		}
	}
	return NULL;
}
