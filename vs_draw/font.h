#ifndef _FONT_H__
#define _FONT_H__
#include "type.h"
#include "font_ASCII_8_12.h"
#include "font_ASCII_12_16.h"

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
	uint8 type;//0:�������ڴ��� 1:�������ⲿ�洢�豸
	FONT_INFO fontInfo;
}*HFONTF,FONTF;
////����һ��ռ���ٸ��ֽ�
//#define FONT_CHAR_PER_BTYES(a) ((uint16)(a->fontInfo.w/8.0f+1))

extern const FONTF fontASCII8_12;
extern const FONTF fontASCII12_16;

uint8* FontReadChar(HFONTF hFont, uint8 ch);

#endif

