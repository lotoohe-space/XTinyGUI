#ifndef _FONT_H__
#define _FONT_H__
#include "type.h"


typedef struct {
	uint16 w;//字符的宽
	uint16 h;//字符的高
	uint16 startInx;//开始位置
	uint16 endInx;//结束位置
	uint16 perRowBytes;//一个字符的一行占多少字节
	union {
		const uint8* addrFont;//字体的地址
		uint32 fd;//文件句柄
	}fontVal;
}*HFONT_INFO,FONT_INFO;

#define ASCII_TYPE		0
#define GB2312_TYPE		1
#define GBK_TYPE		2
#define UNICODE_TYPE	3

typedef struct {
	FONT_INFO fontInfo;
	HFONT_INFO additionFont;
	uint8 typePos;//0:字体在内存中 1:字体在外部存储设备
	uint8 fontType;//字库类型 0:ascii 1:gb2312 2:gbk(未实现) 3:unicode编码
}*HFONTF,FONTF;
////计算一行占多少个字节
//#define FONT_CHAR_PER_BTYES(a) ((uint16)(a->fontInfo.w/8.0f+1))

extern const FONTF fontASCII8_12;
extern const FONTF fontASCII12_16;
extern const FONTF fontASCII16_16;
extern const FONTF fontASCII100_100;
extern const FONTF fontGB231216_16;
extern const FONTF fontUNICODE16_16;

uint8* FontReadChar(HFONTF hFont, uint16 ch);

#endif

