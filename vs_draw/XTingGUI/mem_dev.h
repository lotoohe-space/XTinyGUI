#ifndef _MEM_DEV_H__
#define _MEM_DEV_H__

#include "type.h"
#include "widge_define.h"


typedef struct {

	XRECT	rect;		/*�洢�豸�Ĵ�С����*/
	XPOINT	rawPoint;	/*ԭʼ��ʼλ��*/
	uint8*	mem;		/*�ڴ�ռ�*/
	uint32	memSize;	/*�ڴ��С*/

	uint8	colorDepth;	/*��ɫ���*/

}*HMEM_DEV,MEM_DEV;

HMEM_DEV	MemDevCreate(int16 x, int16 y, uint16 w, uint16 h, uint8 colorDepth);
void		MemDevClear(HMEM_DEV hMemDev, uintColor color);
uint8		MemDevReset(HMEM_DEV hMemDev, int16 x, int16 y, uint16 w, uint16 h);
void		MemDevDrawPT(HMEM_DEV hMemDev, int16 x, int16 y, uintColor color);
uintColor	MemDevReadPT(HMEM_DEV hMemDev, int16 x, int16 y);
void		MemDevCopyToLCD(HMEM_DEV hMemDev);

#endif
