#ifndef _COLOR_H__
#define _COLOR_H__

//默认的背景颜色
#define _DefaultFrColor 0x0000
#define _DefaultBKColor 0xff00

#define RGB565_WHITE	0xffff
#define RGB565_BLACK	0x0000

#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f

//RGB565
#define RGB565TORGB888(n565Color) (((((n565Color) & RGB565_RED)>>8)<<16)|((((n565Color) & RGB565_GREEN)>>3)<<8)|((((n565Color) & RGB565_BLUE)<< 3)))
#define RGB888T0RGB565(a) (((((a)&0x00ff0000)>>19 )<<11) | ((((a)&0x0000ff00)>>10)<<5) |((((a)&0x000000ff)>>3)<<0))

/*获取rgb565的颜色值 A R G B*/
#define C565A(a) (((a)&0x00ff0000)>>16)
#define C565R(a) (((a)&RGB565_RED)>>11)
#define C565G(a) (((a)&RGB565_GREEN)>>5)
#define C565B(a) (((a)&RGB565_BLUE)>>0)


#define RGB565(r,g,b) ((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|((b)&0x1f))
#define ARGB565(a,r,g,b) (((a&0xff)<<16)|RGB565(r,g,b))
//#define RGB565toRGB(rgb565)    (((((((rgb565)&0xF800)>>11)<<3)|((((((rgb565)&0x07E0)>>5)<<2))<<8))|(((((rgb565)&0x001F)<<3))<<16)))

#endif

