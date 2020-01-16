#ifndef _TYPE_H__
#define _TYPE_H__

#include "config.h"

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;


//颜色变量的定义
#if GUI_COLOR_DEEP<=8
	typedef uint8 uintColor;
#elif GUI_COLOR_DEEP>8&&GUI_COLOR_DEEP<=16
	typedef uint16 uintColor;
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


#endif
