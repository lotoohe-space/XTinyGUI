#ifndef _TYPE_H__
#define _TYPE_H__

#include "config.h"

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef struct {
	uint8  a;
	uint16 r : 5;
	uint16 g : 6;
	uint16 b : 5;
}*HARGB565, ARGB565;

typedef struct {
	uint16 r : 5;
	uint16 g : 6;
	uint16 b : 5;
}*HRGB565, RGB565;

typedef struct {
	uint8 r;
	uint8 g;
	uint8 b;
}*HRGB, RGB;

typedef struct {
	uint8 a;
	uint8 r;
	uint8 g;
	uint8 b;
}*HARGB, ARGB;

//颜色变量的定义
#if GUI_COLOR_DEEP<=8
	typedef uint8 uintColor;
#elif GUI_COLOR_DEEP>8&&GUI_COLOR_DEEP<=16
	//typedef union {
	//	ARGB565 aColor;/*带透明色的RGB565*/
	//	uint16 color;/*颜色*/
	//}uintColor;
	typedef uint32 uintColor;/*Argb rgb为16bit*/
#elif GUI_COLOR_DEEP>=24
	typedef uint32 uintColor;
#endif

#ifndef NULL
#define NULL 0L
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif

#define BOOL uint8

#define PUBLIC 
#define PRIVATE static

#ifdef WIN32
#define _INLINE_	_inline
#else
#define _INLINE_	inline
#endif

#endif
