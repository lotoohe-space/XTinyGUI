#ifndef _FONT_H__
#define _FONT_H__
#include "type.h"


typedef struct {
	uint16 w;//�ַ��Ŀ�
	uint16 h;//�ַ��ĸ�
	uint16 startInx;//��ʼλ��
	uint16 endInx;//����λ��
	uint16 perRowBytes;//һ���ַ���һ��ռ�����ֽ�
	union {
		const uint8* addrFont;//����ĵ�ַ
		uint32 fd;//�ļ����
	}fontVal;
}*HFONT_INFO,FONT_INFO;

typedef struct {
	FONT_INFO fontInfo;
	HFONT_INFO additionFont;
	uint8 typePos;//0:�������ڴ��� 1:�������ⲿ�洢�豸
	uint8 fontType;//�ֿ����� 0:ascii 1:gb2312 2:gbk(δʵ��) 3:unicode����
}*HFONTF,FONTF;
////����һ��ռ���ٸ��ֽ�
//#define FONT_CHAR_PER_BTYES(a) ((uint16)(a->fontInfo.w/8.0f+1))

extern const FONTF fontASCII8_12;
extern const FONTF fontASCII12_16;
extern const FONTF fontASCII16_16;
extern const FONTF fontASCII100_100;
extern const FONTF fontGB231216_16;
extern const FONTF fontUNICODE16_16;

uint8* FontReadChar(HFONTF hFont, uint16 ch);

#endif

