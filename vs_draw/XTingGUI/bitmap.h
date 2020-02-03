#pragma once
#ifndef _BITMAP_H__
#define _BITMAP_H__
#include "type.h"

/*Bitmap*/
typedef struct {

	uint16	w;
	uint16	h;				
	uint8*  pixels;			/*����ֵ*/
	uint8	alpha;			/*��flag:0ʹ��ʱ��͸��ͨ��ֵ��Ч*/
	uint8	bitsPerPixel;	/*һ�����ض���λ Ŀǰ֧��RGBR565 BIN */
	uint8	flag;			/*0:bit �Ƿ�Ӧ��ȫ��͸��ͨ�� 1:bit �Ƿ�Ӧ��ͼƬ͸��ͨ��*/
}*HXBITMAP, XBITMAP;

#define BITMAP_DRAW_ARGB			0x1	/*ʹ�õ���alpha��������ͼƬ��͸����*/
#define BITMAP_DRAW_ARGB8888		0x2	/*ʹ�õ�����alphaͨ����ͨ����pixels��*/

uint8 InitBitmap(HXBITMAP hXBitmap, uint8* pixels, uint16 w, uint16 h, uint8 colorDepth);
void BitmapSetFlag(HXBITMAP hXBitmap, uint8 flag);


#define ________	0X00
#define _______X	0x01
#define ______X_	0x02
#define _____X__	0x04
#define ____X___	0x08
#define ___X____	0x10
#define __X_____	0x20
#define _X______	0x40
#define X_______	0x80


#define ______XX _______X|______X_
#define _____XX_ _____X__|______X_
#define ____XX__ _____X__|____X___
#define ___XX___ ___X____|____X___
#define __XX____ __X_____|___X____
#define _XX_____ _X______|__X_____
#define XX______ X_______|_X______


#define ________________	0X0000
#define _______________X	0x0001
#define ______________X_	0x0002
#define _____________X__	0x0004
#define ____________X___	0x0008
#define ___________X____	0x0010
#define __________X_____	0x0020
#define _________X______	0x0040
#define ________X_______	0x0080
#define _______X________	0x0100
#define ______X_________	0x0200
#define _____X__________	0x0400
#define ____X___________	0x0800
#define ___X____________	0x1000
#define __X_____________	0x2000
#define _X______________	0x4000
#define X_______________	0x8000


#define __X_______X_____	(___________X____|__X_____________)
#define ___X_____X______	(__________X_____|___X____________)
#define ____X___X_______	(_________X______|____X___________)

#endif
