#ifndef _COLOR_H__
#define _COLOR_H__



#define RGB565_WHITE	0xffff
#define RGB565_BLACK	0x0000

#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f

//RGB565
#define RGB565TORGB888(n565Color) (((((n565Color) & RGB565_RED)>>8)<<16)|((((n565Color) & RGB565_GREEN)>>3)<<8)|((((n565Color) & RGB565_BLUE)<< 3)))
#define RGB888T0RGB565(a) (((((a)&0x00ff0000)>>19 )<<11) | ((((a)&0x0000ff00)>>10)<<5) |((((a)&0x000000ff)>>3)<<0))
#define BGR888T0RGB565(a) (((((a)&0x00ff0000)>>19)>>0) | ((((a)&0x0000ff00)>>10)<<5) |((((a)&0x000000ff)>>3)<<11))

/*获取rgb565的颜色值 A R G B*/
#define C565A(a) (((a)&0x00ff0000)>>16)
#define C565R(a) ((((a)&RGB565_RED)>>11)<<3)
#define C565G(a) ((((a)&RGB565_GREEN)>>5)<<2)
#define C565B(a) ((((a)&RGB565_BLUE)>>0)<<3)


#define RGB565(r,g,b) (((((r)>>3)&0x1f)<<11)|((((g)>>2)&0x3f)<<5)|(((b)>>3)&0x1f))
#define ARGB565(a,r,g,b) (((a&0xff)<<16)|RGB565(r,g,b))

//默认的背景颜色
#define _DefaultFrColor RGB565(80, 80, 80)
#define _DefaultBKColor RGB565(230,235,230)

#endif

