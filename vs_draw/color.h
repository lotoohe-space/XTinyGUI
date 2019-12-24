#ifndef _COLOR_H__
#define _COLOR_H__

//Ä¬ÈÏµÄ±³¾°ÑÕÉ«
#define _DefaultFrColor 0x0000
#define _DefaultBKColor 0xaaaa

#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f
#define RGB565_WHITE	0xffff
#define RGB565_BLACK	0x0000

//RGB565
#define RGB565TORGB888(n565Color) (((((n565Color) & RGB565_RED)>>8))|((((n565Color) & RGB565_GREEN)>>3)<<8)|((((n565Color) & RGB565_BLUE)<< 3)<<16))

#define RGB565(r,g,b) ((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|((b)&0x1f))
//#define RGB565toRGB(rgb565)    (((((((rgb565)&0xF800)>>11)<<3)|((((((rgb565)&0x07E0)>>5)<<2))<<8))|(((((rgb565)&0x001F)<<3))<<16)))

#endif

