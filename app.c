#include <stdio.h>

#include "msg.h"
#include "gui.h"
#include "x_widget.h"
#include "xwindows.h"
#include "button_widge.h"
#include "text_widge.h"
#include "bitmap.h"
#include "bitmap_widge.h"
#include "progress_widge.h"
#include "check_box_widge.h"
#include "switch_button_widge.h"
#include "radio_button_widge.h"
#include "list_widge.h"
#include "slide_widge.h"
extern void nes_main(void);

p_win_t hWin4;
xbitmap_t pngBitmap;
xbitmap_t xBitmap[33];
p_bitmap_widget_t dyBitMap;
xbitmap_t xGirl1Bitmap;
xbitmap_t xGirlBitmap;
p_bitmap_widget_t hBITMAP_WIDGE1;
p_text_widget_t fpsTextWidge;
p_progress_widget_t testProgressWidge;
p_win_t hWinDPY;
uint64_t lastTick = 0;

int count = 0;
char data[10] = {0};
p_xbutton_t hXButton;
p_text_widget_t hTEXT_WIDGE;
void btnCallBack(void *Object, void *arg, uint8_t status)
{
	if (status)
	{
		sprintf(data, "%d", count);
		BUTTON_MARK_HEAD(SetText)
		(hXButton, data);
		TextWidgetSetText(hTEXT_WIDGE, data);
		count += 5;

		if (count > 100)
		{
			count = 0;
			;
		}
	}
}
p_checkbox_widget_t testCheckBox;
p_bitmap_widget_t hBITMAP_WIDGE;

void checkCallBack(void *Object, void *arg, uint8_t status)
{
	if (status)
	{
		WidgetSetVisable((p_widget_base_t)hBITMAP_WIDGE1, TRUE);
	}
	else
	{
		WidgetSetVisable((p_widget_base_t)hBITMAP_WIDGE1, FALSE);
	}
}

int winCount = 0;
#include <stdlib.h>

#include "girl_bitmap.h"
#include "tu001000.c"
#include "tu001001.c"
#include "tu001002.c"
#include "tu001003.c"
#include "tu001004.c"
#include "tu001005.c"
#include "tu001006.c"
#include "tu001007.c"
#include "tu001008.c"
#include "tu001009.c"
#include "tu001010.c"
#include "tu001011.c"
#include "tu001012.c"
#include "tu001013.c"
#include "tu001014.c"
#include "tu001015.c"
#include "tu001016.c"
#include "tu001017.c"
#include "tu001018.c"
#include "tu001019.c"
#include "tu001020.c"
#include "tu001021.c"
#include "tu001022.c"
#include "tu001023.c"
#include "tu001024.c"
#include "tu001025.c"
#include "tu001026.c"
#include "tu001027.c"
#include "tu001028.c"
#include "tu001029.c"
#include "tu001030.c"
#include "tu001031.c"
#include "tu001032.c"
#include "png_img.h"
p_win_t hWin2;
unsigned short nesImg[(256) * 260] = {0};
uint8_t WinProcessFunction(void *Object, void *arg, p_msg_t hMsg)
{
	switch (hMsg->msgType)
	{
	case MSG_WIN:
		if (hMsg->msgID == MSG_WIN_CREATE)
		{

			/*窗口中添加一个文本框*/
			hTEXT_WIDGE = TextWidgetCreate(L"AB", 5, 5, 150, 100);
			hTEXT_WIDGE->hFont = &fontASCII100_100;
			WindowsAdd(Object, hTEXT_WIDGE);

			/*添加一个进度条*/
			testProgressWidge = PROGRESS_MARK_HEAD(Create)(5, 115, 100, 20, 100);
			WindowsAdd(Object, testProgressWidge);

			/*添加一个checkbox*/
			testCheckBox = CHECKBOX_MARK_HEAD(Create)(5, 140, 140, 18, "CheckBox");
			CHECKBOX_MARK_HEAD(SetClickBack)
			(testCheckBox, testCheckBox, checkCallBack);
			WindowsAdd(Object, testCheckBox);

			/*窗口中添加一个文本框*/
			fpsTextWidge = TextWidgetCreate("0fps", 5, 160, 50, 12);
			fpsTextWidge->hFont = &fontASCII8_12;
			WindowsAdd(Object, fpsTextWidge);

			WindowsAdd(Object, SLIDE_MARK_HEAD(Create)(5, 180, 100, 20));

			p_xlist_widget_t hListWidge = LIST_WIDGE_MARK_HEAD(Create)(160, 5, 240, 250);
			_OpenListAnimation(hListWidge);
			WidgetSetColor(hListWidge, ARGB565_GEN(128, 0xff, 0x00, 0xff));

			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, TEXT_MARK_HEAD(Create)(L"test00", 0, 0, 0, 20));
			p_bitmap_widget_t listBitmap = BitmapWidgeCreate(100, 100, 160, 120, &(xBitmap[0]));
			//_SET_IS_DPY(listBitmap);
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, listBitmap);
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, TEXT_MARK_HEAD(Create)(L"test10", 0, 0, 0, 20));
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, CHECKBOX_MARK_HEAD(Create)(250, 50, 140, 18, "CheckBox0"));
			listBitmap = BitmapWidgeCreate(100, 100, 160, 120, &(xBitmap[15]));
			//_SET_IS_DPY(listBitmap);
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, listBitmap);
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, PROGRESS_MARK_HEAD(Create)(10, 200, 100, 20, 100));
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, TEXT_MARK_HEAD(Create)(L"test50", 0, 0, 0, 20));
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, CHECKBOX_MARK_HEAD(Create)(250, 50, 140, 18, "CheckBox1"));
			listBitmap = BitmapWidgeCreate(100, 100, 160, 120, &(xBitmap[31]));
			//_SET_IS_DPY(listBitmap);
			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, listBitmap);

			LIST_WIDGE_MARK_HEAD(Add)
			(hListWidge, BitmapWidgeCreate(0, 0, 256, 240, &xGirlBitmap));

			WindowsAdd(Object, hListWidge);

			InitBitmap(&pngBitmap, pngImg, 160, 120, 32);
			BitmapSetFlag(&pngBitmap, BITMAP_DRAW_ARGB8888);
			dyBitMap = BitmapWidgeCreate(5, 240, 160, 120, &(pngBitmap));
			/*多张图片动态切换显示*/
			// InitBitmap(&(xBitmap[0]), gImage_tu001032, 240, 160, 16);
			// dyBitMap = BitmapWidgeCreate(10, 10, 330, 230, &(xBitmap[0]));
			_SET_IS_DPY(dyBitMap);
			WidgetSetColor(dyBitMap, ARGB565_GEN(255, 0xff, 0x00, 0xff));
			WindowsAdd(Object, dyBitMap);

			p_bitmap_widget_t alphaBitmap = BitmapWidgeCreate(400, 5, 400, 400, &(xBitmap[30]));
			xBitmap[30].flag = 0x1;
			xBitmap[30].alpha = 128;
			_SET_IS_DPY(alphaBitmap);
			WidgetSetColor(alphaBitmap, ARGB565_GEN(255, 0xff, 0xff, 0xff));
			WindowsAdd(Object, alphaBitmap);

			/*内部添加一个窗口*/
			hWin2 = WindowsCreate(L"Windows", 100, 100, 100, 100);
			_SET_IS_DPY(hWin2);
			WindowsSetColor(hWin2, ARGB565_GEN(128, 0, 0, 0xff));
			WindowsAdd(Object, hWin2);
		}
		else if (hMsg->msgID == MSG_WIN_PAINT)
		{
			/*画一个正方形*/
			/*xrect_t drawRect;
			drawRect.x=((p_widget_base_t)(Object))->rect.x+250;
			drawRect.y=((p_widget_base_t)(Object))->rect.y+150;
			drawRect.w = 35;
			drawRect.h = 45;
			uintColor temp = ((p_widget_base_t)(Object))->pencil.DrawColor;
			((p_widget_base_t)(Object))->pencil.DrawColor = 0x0000;
			DrawCutRect(Object,&drawRect);
			((p_widget_base_t)(Object))->pencil.DrawColor = temp;*/
		}
		break;
	}
	return TRUE;
}
uint8_t WinProcessNESFunction(void *Object, void *arg, p_msg_t hMsg)
{
	switch (hMsg->msgType)
	{
	case MSG_WIN:
		if (hMsg->msgID == MSG_WIN_CREATE)
		{
			/*用来显示nes的图片*/
			InitBitmap(&xGirlBitmap, nesImg, 256, 240, 16);
			// xGirlBitmap.flag = 0x1;
			// xGirlBitmap.alpha = 100;
			hBITMAP_WIDGE1 = BitmapWidgeCreate(0, 0, 256, 240, &xGirlBitmap);
			_CLR_IS_DPY(hBITMAP_WIDGE1);
			WindowsAdd(Object, hBITMAP_WIDGE1);
		}
		else if (hMsg->msgID == MSG_WIN_PAINT)
		{
			/*重绘*/
		}
		break;
	case MSG_KEY:
		switch (hMsg->msgID)
		{
		case MSG_KEY_UP:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 4, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 4, 0);
			}
			break;
		case MSG_KEY_DOWN:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 5, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 5, 0);
			}
			break;
		case MSG_KEY_LEFT:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 6, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 6, 0);
			}
			break;
		case MSG_KEY_RIGHT:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 7, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 7, 0);
			}
			break;
		case MSG_KEY_0:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 3, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 3, 0);
			}
			break;
		case MSG_KEY_1:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 2, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 2, 0);
			}
			break;
		case MSG_KEY_N:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 0, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 0, 0);
			}
			break;
		case MSG_KEY_M:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(0, 1, 1);
			}
			else
			{
				NES_SetJoyPadValue(0, 1, 0);
			}
			break;

		case MSG_KEY_W:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(1, 4, 1);
			}
			else
			{
				NES_SetJoyPadValue(1, 4, 0);
			}
			break;
		case MSG_KEY_S:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(1, 5, 1);
			}
			else
			{
				NES_SetJoyPadValue(1, 5, 0);
			}
			break;
		case MSG_KEY_A:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(1, 6, 1);
			}
			else
			{
				NES_SetJoyPadValue(1, 6, 0);
			}
			break;
		case MSG_KEY_D:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(1, 7, 1);
			}
			else
			{
				NES_SetJoyPadValue(1, 7, 0);
			}
			break;
		case MSG_KEY_V:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(1, 0, 1);
			}
			else
			{
				NES_SetJoyPadValue(1, 0, 0);
			}
			break;
		case MSG_KEY_B:
			if (hMsg->msgVal.status)
			{
				NES_SetJoyPadValue(1, 1, 1);
			}
			else
			{
				NES_SetJoyPadValue(1, 1, 0);
			}
			break;
		}
		break;
	}
	return TRUE;
}
xbitmap_t xBitmapItem0;
xbitmap_t xBitmapItem1;
xbitmap_t xBitmapItem2;
unsigned char __data[32];
uint8_t WinProcess1Function(void *Object, void *arg, p_msg_t hMsg)
{
	switch (hMsg->msgType)
	{
	case MSG_WIN:
		if (hMsg->msgID == MSG_WIN_CREATE)
		{
			p_text_widget_t textWidge = TextWidgetCreate("Nes emulator !!!! My Name is zhang zheng", 5, 100, 200, 48);
			textWidge->hFont = &fontASCII8_12;
			TextWidgetSetTextMode(textWidge, TEXT_TOP_LEFT_ALIGN);
			WindowsAdd(Object, textWidge);

			textWidge = TextWidgetCreate("left right A-->N B-->M", 5, 50, 200, 12);
			textWidge->hFont = &fontASCII8_12;
			WindowsAdd(Object, textWidge);

			sprintf(__data, "%f", 123);
			textWidge = TextWidgetCreate(__data, 5, 74, 200, 12);
			textWidge->hFont = &fontASCII8_12;
			WindowsAdd(Object, textWidge);

			p_xlist_widget_t hListWidge = LIST_WIDGE_MARK_HEAD(Create)(59, 50, 400, 259);
			_SetOTN_H(hListWidge);
			//_SET_IS_DPY(hListWidge);
			// WIDGE_MARK_HEAD(SetColor)(hListWidge, ARGB565_GEN(255, 0xff, 0xFF, 0xff));

			/*	extern const unsigned long img_item_0[];
				extern const unsigned long img_item_1[];
				extern const unsigned long img_item_2[];
				InitBitmap(&xBitmapItem0, (uint8_t*)img_item_0, 150, 259, 32);
				BitmapSetFlag(&xBitmapItem0, BITMAP_DRAW_ARGB8888);
				InitBitmap(&xBitmapItem1, (uint8_t*)img_item_1, 150, 259, 32);
				BitmapSetFlag(&xBitmapItem1, BITMAP_DRAW_ARGB8888);
				InitBitmap(&xBitmapItem2, (uint8_t*)img_item_2, 150, 259, 32);
				BitmapSetFlag(&xBitmapItem2, BITMAP_DRAW_ARGB8888);*/

			// p_bitmap_widget_t listBitmap0 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem0);
			// p_bitmap_widget_t listBitmap1 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem1);
			// p_bitmap_widget_t listBitmap2 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem2);
			// p_bitmap_widget_t listBitmap3 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem0);
			// p_bitmap_widget_t listBitmap4 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem1);
			// p_bitmap_widget_t listBitmap5 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem2);
			// p_bitmap_widget_t listBitmap6 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem0);
			// p_bitmap_widget_t listBitmap7 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem1);
			// p_bitmap_widget_t listBitmap8 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem2);
			// p_bitmap_widget_t listBitmap9 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem0);
			// p_bitmap_widget_t listBitmap10 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem1);
			// p_bitmap_widget_t listBitmap11 = BitmapWidgeCreate(0, 0, 150, 259, &xBitmapItem2);
			// _SET_IS_DPY(listBitmap0);
			// _SET_IS_DPY(listBitmap1);
			// _SET_IS_DPY(listBitmap2);
			// _SET_IS_DPY(listBitmap3);
			// _SET_IS_DPY(listBitmap4);
			// _SET_IS_DPY(listBitmap5);
			// _SET_IS_DPY(listBitmap6);
			// _SET_IS_DPY(listBitmap7);
			// _SET_IS_DPY(listBitmap8);
			// _SET_IS_DPY(listBitmap9);
			// _SET_IS_DPY(listBitmap10);
			// _SET_IS_DPY(listBitmap11);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap0);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap1);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap2);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap3);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap4);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap5);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap6);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap7);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap8);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap9);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap10);
			// LIST_WIDGE_MARK_HEAD(Add)
			// (hListWidge, listBitmap11);

			// LIST_WIDGE_MARK_HEAD(SetFlag)
			// (hListWidge, LIST_OPEN_ANT, TRUE);
			// //_SET_IS_DPY(hListWidge);
			// WindowsAdd(Object, hListWidge);
		}
		else if (hMsg->msgID == MSG_WIN_PAINT)
		{
			/*画一个正方形*/
			xrect_t drawRect;
			drawRect.x = ((p_widget_base_t)(Object))->rect.x + 10;
			drawRect.y = ((p_widget_base_t)(Object))->rect.y + 10;
			drawRect.w = 35;
			drawRect.h = 45;
			uintColor temp = ((p_widget_base_t)(Object))->pencil.DrawColor;
			((p_widget_base_t)(Object))->pencil.DrawColor = 0x0000;
			DrawCutRect(Object, &drawRect);
			((p_widget_base_t)(Object))->pencil.DrawColor = temp;
		}
		break;
	}
	return TRUE;
}
p_win_t hWin;
// xbitmap_t btnBitmapUp;
// xbitmap_t btnBitmapDown;
void gui_init(void)
{
	GUIInit();

	p_win_t hWin3;
	p_win_t hWIN;

	InitBitmap(&(xBitmap[0]), gImage_tu001000, 240, 160, 16);
	InitBitmap(&(xBitmap[1]), gImage_tu001001, 240, 160, 16);
	InitBitmap(&(xBitmap[2]), gImage_tu001002, 240, 160, 16);
	InitBitmap(&(xBitmap[3]), gImage_tu001003, 240, 160, 16);
	InitBitmap(&(xBitmap[4]), gImage_tu001004, 240, 160, 16);
	InitBitmap(&(xBitmap[5]), gImage_tu001005, 240, 160, 16);
	InitBitmap(&(xBitmap[6]), gImage_tu001006, 240, 160, 16);
	InitBitmap(&(xBitmap[7]), gImage_tu001007, 240, 160, 16);
	InitBitmap(&(xBitmap[8]), gImage_tu001008, 240, 160, 16);
	InitBitmap(&(xBitmap[9]), gImage_tu001009, 240, 160, 16);
	InitBitmap(&(xBitmap[10]), gImage_tu001010, 240, 160, 16);
	InitBitmap(&(xBitmap[11]), gImage_tu001011, 240, 160, 16);
	InitBitmap(&(xBitmap[12]), gImage_tu001012, 240, 160, 16);
	InitBitmap(&(xBitmap[13]), gImage_tu001013, 240, 160, 16);
	InitBitmap(&(xBitmap[14]), gImage_tu001014, 240, 160, 16);
	InitBitmap(&(xBitmap[15]), gImage_tu001015, 240, 160, 16);
	InitBitmap(&(xBitmap[16]), gImage_tu001016, 240, 160, 16);
	InitBitmap(&(xBitmap[17]), gImage_tu001017, 240, 160, 16);
	InitBitmap(&(xBitmap[18]), gImage_tu001018, 240, 160, 16);
	InitBitmap(&(xBitmap[19]), gImage_tu001019, 240, 160, 16);
	InitBitmap(&(xBitmap[20]), gImage_tu001020, 240, 160, 16);
	InitBitmap(&(xBitmap[21]), gImage_tu001021, 240, 160, 16);
	InitBitmap(&(xBitmap[22]), gImage_tu001022, 240, 160, 16);
	InitBitmap(&(xBitmap[23]), gImage_tu001023, 240, 160, 16);
	InitBitmap(&(xBitmap[24]), gImage_tu001024, 240, 160, 16);
	InitBitmap(&(xBitmap[25]), gImage_tu001025, 240, 160, 16);
	InitBitmap(&(xBitmap[26]), gImage_tu001026, 240, 160, 16);
	InitBitmap(&(xBitmap[27]), gImage_tu001027, 240, 160, 16);
	InitBitmap(&(xBitmap[28]), gImage_tu001028, 240, 160, 16);
	InitBitmap(&(xBitmap[29]), gImage_tu001029, 240, 160, 16);
	InitBitmap(&(xBitmap[30]), gImage_tu001030, 240, 160, 16);
	InitBitmap(&(xBitmap[31]), gImage_tu001031, 240, 160, 16);

	/*美女图,放到桌面*/
	InitBitmap(&xGirl1Bitmap, gImage_girl, 320, 225, 16);
	hBITMAP_WIDGE = BitmapWidgeCreate(0, 0, 320, 225, &xGirl1Bitmap);
	WinListAdd(hBITMAP_WIDGE);

	hWIN = WindowsCreate("test", 5, 5,
						 200, 200);
	// _SET_IS_DPY(hWIN);
	// WindowsSetColor(hWIN, ARGB565_GEN(128, 0xff, 0xff, 0xff));
	WinListAdd(hWIN);

#include "imgs.h"
	// InitBitmap(&btnBitmapDown, (uint8_t*)playBitmap, 50, 50, 32);
	// InitBitmap(&btnBitmapUp, (uint8_t*)pauseBitmap, 50, 50, 32);
	// BitmapSetFlag(&btnBitmapDown, BITMAP_DRAW_ARGB8888);
	// BitmapSetFlag(&btnBitmapUp, BITMAP_DRAW_ARGB8888);
	/*窗口中添加一个button*/
	hXButton = BUTTON_MARK_HEAD(Create)("BTN", 10, 10, 50, 50);
	_SetBMPMode(hXButton);
	// _SET_IS_DPY(hXButton);
	BUTTON_MARK_HEAD(SetClickBack)
	(hXButton, hXButton, btnCallBack);
	// BUTTON_MARK_HEAD(SetDownBitmap)(hXButton, &btnBitmapDown);
	// BUTTON_MARK_HEAD(SetUpBitmap)(hXButton, &btnBitmapUp);
	WindowsAdd(hWIN, hXButton);

	/*文本模式*/
	p_switch_button_widget_t hSwBtnWidge = SWITCH_BUTTON_WIDGE_MARK_HEAD(Create)(65, 10, 50, 50, "On", "Off");
	_SET_SW_TEXT_MODE(hSwBtnWidge);
	WindowsAdd(hWIN, hSwBtnWidge);

	/*图片模式*/
	// hSwBtnWidge = SWITCH_BUTTON_WIDGE_MARK_HEAD(Create)(10, 65, 50, 50, NULL, NULL);
	// _SET_SW_BMP_MODE(hSwBtnWidge);
	// _SET_IS_DPY(hSwBtnWidge);
	// hSwBtnWidge->offBitmap = &btnBitmapDown;
	// hSwBtnWidge->onBitmap = &btnBitmapUp;
	// WindowsAdd(hWIN, hSwBtnWidge);

	p_radio_button_widget_t hRadioButtonWidge = RADIO_BUTTON_MARK_HEAD(Create)(65, 65, 100, 16, "RadioBtn");
	WindowsAdd(hWIN, hRadioButtonWidge);

	hRadioButtonWidge = RADIO_BUTTON_MARK_HEAD(Create)(65, 80, 100, 16, "RadioBtn1");
	WindowsAdd(hWIN, hRadioButtonWidge);
	hRadioButtonWidge = RADIO_BUTTON_MARK_HEAD(Create)(65, 100, 100, 16, "RadioBtn1");
	WindowsAdd(hWIN, hRadioButtonWidge);
	hRadioButtonWidge = RADIO_BUTTON_MARK_HEAD(Create)(65, 120, 100, 16, "RadioBtn1");
	WindowsAdd(hWIN, hRadioButtonWidge);

	hWin = WindowsCreate("Windows1", 200, 200, 500, 400);
	WindowsSetProcessCallBack(hWin, WinProcessFunction);
	// _SET_IS_DPY(hWin);
	// WindowsSetColor(hWin, ARGB565_GEN(128, 0xff, 0x00, 0xff));
	WinListAdd(hWin);

	hWinDPY = WindowsCreate("Windows2", 5, 5, 550, 380);
	_SET_IS_DPY(hWinDPY);
	WindowsSetColor(hWinDPY, ARGB565_GEN(200, 0xff, 0xff, 0x33));
	WindowsSetProcessCallBack(hWinDPY, WinProcess1Function);
	WinListAdd(hWinDPY);
	/*NES模拟器*/
	hWin4 = WindowsCreate("NES", 200, 200, 256, 256);
	WindowsSetProcessCallBack(hWin4, WinProcessNESFunction);
	// _SET_IS_DPY(hWin4);
	WinListAdd(&hWin4->groupWidge.widgeBase);

	nes_main();
}
