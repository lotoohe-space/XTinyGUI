#ifndef _FONT_H__
#define _FONT_H__
#include "type.h"
#include "font_ASCII_8_12.h"
#include "font_ASCII_12_16.h"

typedef struct {
	uint16 w;
	uint16 h;
	uint16 startInx;//开始位置
	uint16 endInx;//结束位置
	uint16 perRowBytes;//一个字符的一行占多少字节
	union {
		const uint8* addrFont;//字体的地址
		uint32 fd;//文件句柄
	}fontVal;
}*HFONT_INFO,FONT_INFO;

typedef struct {
	uint8 type;//0:字体在内存中 1:字体在外部存储设备
	FONT_INFO fontInfo;
}*HFONTF,FONTF;
////计算一行占多少个字节
//#define FONT_CHAR_PER_BTYES(a) ((uint16)(a->fontInfo.w/8.0f+1))

extern const FONTF fontASCII8_12;
extern const FONTF fontASCII12_16;

uint8* FontReadChar(HFONTF hFont, uint8 ch);

#endif

