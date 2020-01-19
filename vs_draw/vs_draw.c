#pragma comment(linker,"/SUBSYSTEM:Windows  /ENTRY:mainCRTStartup")
#include <windows.h>/* 使类名成为全局变量 */
#include <stdio.h>

#include "msg.h"
#include "gui.h"
#include "widge.h"
#include "xwindows.h"
#include "button_widge.h"
#include "text_widge.h"
#include "bitmap.h"
#include "bitmap_widge.h"
#include "progress_widge.h"
#include "check_box_widge.h"
TCHAR szClassName[] = TEXT("WindowsApp");
#define WIN_W 1024
#define WIN_H 700

XBITMAP xBitmap[33];
HBITMAP_WIDGE dyBitMap;
XBITMAP xGirl1Bitmap;
XBITMAP xGirlBitmap;
HBITMAP_WIDGE hBITMAP_WIDGE1;
HPROGRESS_WIDGE testProgressWidge;
// 用于注册的窗口类名
const char g_szClassName[] = "myWindowClass";

unsigned int displayMem[WIN_W * WIN_H];
int count = 0;
//HWIN winList[2001];
HDC memDC;
static HDC mdc;
static HBITMAP bmp;
HDC hdc;   // DC(可画图的内存对象) 的句柄
void d_lineH(int x0,int y,int x1,int color) {
	HPEN hpen; // 画笔
	// 创建画笔
	/*RGB(255, 0, 0)*/
	hpen = CreatePen(PS_SOLID, 1, color);
	// DC 选择画笔
	SelectObject(memDC, hpen);
	// (画笔)从初始点移动到 50,50
	MoveToEx(memDC, x0, y, NULL);
	// (画笔)从初始点画线到 100,100
	LineTo(memDC, x1, y);
	DeleteObject(hpen);//释放掉
}

void d_lineV(int x, int y0, int y1, int color) {
	HPEN hpen; // 画笔
			   // 创建画笔
			   /*RGB(255, 0, 0)*/
	hpen = CreatePen(PS_SOLID, 1, color);
	// DC 选择画笔
	SelectObject(memDC, hpen);
	// (画笔)从初始点移动到 50,50
	MoveToEx(memDC, x, y0, NULL);
	// (画笔)从初始点画线到 100,100
	LineTo(memDC, x, y1);

	DeleteObject(hpen);//释放掉
}

void Vsrefresh() {
	SetBitmapBits(bmp, 1024 * 700 * 4, displayMem);
	// 3.一次性复制到设备DC
	BitBlt(hdc, 0, 0, 1024, 700, mdc, 0, 0, SRCCOPY);
}
void d_pix(int x,int y,int color) {
	if (x<0 || x>=LCD_SCREEN_W || y<0 || y>=LCD_SCREEN_H) { return; }
	//需要移植的函数
	//SetPixelV(memDC, x, y, color);
	displayMem[y * WIN_W + x] = color;


}
void d_bmp(
	int x, int y,	/*实际图片绘制的起点*/
	int w, int h,	/*需要绘制的图片的大小*/
	unsigned short* bitmap,
	int x1,int y1,	/*从图片中绘制的开始位置*/
	int w1,int h1	/*图片的大小*/
) {
	int i, j;
	int endy;
	int endx;
	endy = h + y;
	endx = w + x;
	for (i = y; i < endy; i++) {
		for (j = x; j < endx; j++) {
			d_pix(j, i, RGB565TORGB888(bitmap[(i-y+y1) * w1 + (j-x)+x1]));
		}
	}
	//SetBitmapBits(bmp, 1024 * 700 * 4, displayMem);
	//// 3.一次性复制到设备DC
	//BitBlt(hdc, 0, 0, 1024, 700, mdc, 0, 0, SRCCOPY);
}
void d_rect(int x, int y, int w, int h, int color) {

	int i, j;
	for (i = y; i < y + h; i++) {
		for (j = x; j < x + w; j++) {
			d_pix(j,i,color);
		}
	}
	//SetBitmapBits(bmp, 1024 * 700 * 4, displayMem);
	//// 3.一次性复制到设备DC
	//BitBlt(hdc, 0, 0, 1024, 700, mdc, 0, 0, SRCCOPY);
	//RECT rect;
	//rect.left = x;
	//rect.right = x + w;
	//rect.top = y;
	//rect.bottom = y + h;
	//int n_color = color;
	//HBRUSH hBrush = CreateSolidBrush(
	//	n_color
	//);
	//FillRect(memDC, // 绘图设备 
	//	&rect, // 矩形区域 
	//	hBrush// 填充画刷 
	//);
	//DeleteObject(hBrush);
}
void fill_rect(int x,int y,int w,int h,int color) {

	int i, j;
	for (i = y; i < y + h; i++) {
		for (j = x; j < x + w; j++) {
			d_pix(j, i, color);
		}
	}
	//SetBitmapBits(bmp, 1024 * 700 * 4, displayMem);
	//// 3.一次性复制到设备DC
	//BitBlt(hdc, 0, 0, 1024, 700, mdc, 0, 0, SRCCOPY);
	//RECT rect;
	//rect.left = x;
	//rect.right = x + w;
	//rect.top = y;
	//rect.bottom = y + h;
	//int n_color = color;
	//HBRUSH hBrush = CreateSolidBrush(
	//	n_color
	//);
	////需要移植的函数
	//FillRect(
	//	memDC, // 绘图设备 
	//	&rect, // 矩形区域 
	//	hBrush// 填充画刷 
	//);
	//DeleteObject(hBrush);
}
int delay_count = 0;
//extern void RectCutFindX(void);
//extern void RectDraw_text(void);
HWIN hWin;
HWIN hWin1;
int bit_count=0;
void Paint(HWND hwnd)
{
	// paint struct 绘图结构体，存储目标窗口可以绘图的客户端区域(client area)


	/*PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);*/
	hdc = GetDC(hwnd);


	mdc = CreateCompatibleDC(hdc);   // 创建兼容DC 画板
	bmp = CreateCompatibleBitmap(hdc, WIN_W, WIN_H);   // 创建画布
		
	SelectObject(mdc, bmp);   //  将画布选入画板
	memDC = mdc;
	

	GUIEvent();

	
	if (delay_count++ > 10) {
		BITMAP_MARK_HEAD(SetBitmap)(dyBitMap, &xBitmap[bit_count]);
		PROGRESS_MARK_HEAD(SetProgressVal)(testProgressWidge, count);
		bit_count++;
		count++;
		if (count > 100) {
			count = 0;
		}
		if (bit_count > 32) {
			bit_count = 0;
		}
		delay_count = 0;
	}
	extern void NesFrameCycle(void);
	NesFrameCycle();

	//GUISendMsg(hWin, MSG_WIN, MSG_WIN_MOVE, rand() % 800, rand() % 500,0,0);
	SetBitmapBits(bmp, WIN_W * WIN_H* 4, displayMem);
	// 3.一次性复制到设备DC
	BitBlt(hdc, 0, 0, WIN_W, WIN_H, mdc, 0, 0, SRCCOPY);
	////// 4.释放缓冲区DC
	DeleteDC(mdc);
	DeleteObject(bmp);
	DeleteObject(hdc);

	// EndPaint(hwnd, &ps);
}
int flag = 0;
/* 这个函数由Windows内部函数DispatchMessage()调用 */
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	int y;
	switch (message) {
	case WM_LBUTTONDOWN:
		//MessageBox(NULL, L"鼠标左键点击", L"Win32_Mouse", MB_OK); 
		 x = LOWORD(lParam);//鼠标的横坐标		
		 y = HIWORD(lParam);//鼠标的纵坐标 		/*字符串、整形的拼接*/	

		 GUISendTouchMsg(MSG_TOUCH_PRESS, x, y);


		 flag = 1;
		break;
	case WM_LBUTTONUP:
		if (flag) 
		{
			x = LOWORD(lParam);//鼠标的横坐标		
			y = HIWORD(lParam);//鼠标的纵坐标 		/*字符串、整形的拼接*/	

				GUISendTouchMsg(MSG_TOUCH_RELEASE, x, y);


			flag = 0;
		}
		break;
	case WM_RBUTTONDOWN:
		//MessageBox(NULL, L"鼠标右键点击", L"Win32_Mouse", MB_OK);

		break;
	case WM_MOUSEMOVE: {//鼠标移动事件		
		if (flag) 
		{
			//hdc=GetDC(hWnd);//BeginPaint(hWnd, &ps);仅为重绘函数专用的HDC初始化。其余事件大部分都是用GetDC(hWnd);初始化HDC。		
			x = LOWORD(lParam);//鼠标的横坐标		
			y = HIWORD(lParam);//鼠标的纵坐标 		/*字符串、整形的拼接*/	

				GUISendTouchMsg(MSG_TOUCH_MOVE, x, y);

		}
		break;
	}
	case WM_PAINT:	
		Paint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	InvalidateRect(hwnd, NULL, TRUE);
}
char data[10] = { 0 };

HXBUTTON hXButton;
HTEXT_WIDGE hTEXT_WIDGE;
void btnCallBack(void* Object, void* arg, uint8 status) {
	if (status) {
		sprintf(data,"%4d", count);
		BUTTON_MARK_HEAD(SetText)(hXButton, data);
		TextSetText(hTEXT_WIDGE,data);
		count+=5;
		
		if (count > 100) {
			count = 0;;
		}
	/*	HWIN hWIN;
		hWIN = WindowsCreate("test", rand()%700, rand()%400,
		100, 100);
		WindowsSetColor(hWIN, 0x00ff);
		while (!draw_flag);
		draw_flag = 0;
		WinListAdd(hWIN);
		draw_flag = 1;*/
	}
}
HCHECKBOX_WIDGE testCheckBox;
HBITMAP_WIDGE hBITMAP_WIDGE;

void checkCallBack(void* Object, void* arg, uint8 status) {
	if (status) {
		WidgeSetVisable((HWIDGE_BASE)hBITMAP_WIDGE1,TRUE);
	}
	else {
		WidgeSetVisable((HWIDGE_BASE)hBITMAP_WIDGE1, FALSE);
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

unsigned short nesImg[( 256 ) * 260] = { 0 };


void gui_init(void) {
	GUIInit();

	HWIN hWin3;
	HWIN hWin2;
	HWIN hWin4;

	
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
	hBITMAP_WIDGE = BitmapWidgeCreate(100, 100, 640, 360, &xGirl1Bitmap);
	WinListAdd(hBITMAP_WIDGE);



	hWin4 = WindowsCreate("Windows", 100, 100, 256, 240);
	WinListAdd(hWin4);

	/*用来显示nes的图片*/
	InitBitmap(&xGirlBitmap, nesImg, 256, 240, 16);
	hBITMAP_WIDGE1 = BitmapWidgeCreate(0, 0, 256, 240, &xGirlBitmap);
	WindowsWidgeAdd(hWin4, hBITMAP_WIDGE1);

	HWIN hWIN;
	hWIN = WindowsCreate("test", 650, 100,
		150, 150);
	WindowsSetColor(hWIN, 0xfff0);
	WinListAdd(hWIN);

	/*窗口中添加一个button*/
	hXButton = BUTTON_MARK_HEAD(Create)("this is a button", 10, 10, 50, 50);
	WindowsWidgeAdd(hWIN, hXButton);
	BUTTON_MARK_HEAD(SetClickBack)(hXButton, hXButton,btnCallBack);

	hWin = WindowsCreate("Win", 350, 150, 400, 300);
	WindowsSetColor(hWin, 0x0ff0);
	WinListAdd(hWin);


	
	/*多张图片动态切换显示*/
	InitBitmap(&(xBitmap[0]), gImage_tu001032, 240, 160, 16);
	dyBitMap = BitmapWidgeCreate(10, 10, 330, 230, &(xBitmap[0]));
	WindowsWidgeAdd(hWin, dyBitMap);

	/*窗口中添加一个文本框*/
	hTEXT_WIDGE = TextWidegeCreate("test", 50, 50);
	WindowsWidgeAdd(hWin, hTEXT_WIDGE);

	/*添加一个进度条*/
	testProgressWidge = PROGRESS_MARK_HEAD(Create)(10, 200, 100, 20, 100);
	WindowsWidgeAdd(hWin, testProgressWidge);

	/*添加一个checkbox*/
	testCheckBox = CHECKBOX_MARK_HEAD(Create)(250, 50, 140, 18, "CheckBox");
	CHECKBOX_MARK_HEAD(SetClickBack)(testCheckBox, testCheckBox, checkCallBack);
	WindowsWidgeAdd(hWin, testCheckBox);

	extern void nes_main(void);
	nes_main();

	//hWin2 = WindowsCreate("Windows", 10, 10, 50, 50);
	//WindowsSetColor(hWin2, 0x00ff);
	//WindowsWidgeAdd(hWin4,hWin2);
	////WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);
	//hWin4 = WindowsCreate("Windows", 180, 100, 100, 100);
	////WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);

	//hWin4 = WindowsCreate("Windows", 10, 10, 100, 100);
	//WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);

	//hWin4 = WindowsCreate("Windows", -20, -20, 50, 1000);
	////WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);

	//hWin4 = WindowsCreate("Windows", 740, -20, 20, 1000);
	////WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);

	//hWin4 = WindowsCreate("Windows", 760, -20, 50, 1000);
	////WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);

	//hWin4 = WindowsCreate("Windows", -20, 460, 1000, 50);
	////WindowsWidgeAdd(hWin4, ButtonWidgeCreate("Button", 10, 10, 100, 50));
	//WindowsSetColor(hWin4, 0x00ff);
	//WinListAdd(hWin4);

	//WinListAdd(testCheckBox);
	//WinListAdd(testProgressWidge);
	//WinListAdd(hBITMAP_WIDGE1);
	//hXButton = ButtonWidgeCreate("BTN0", 10, 10, 50, 50);
	//WinListAdd(hXButton);
	//hWin1 = WindowsCreate("Windows", 100, 10, 250, 250);
	//WindowsSetColor(hWin1, 0x00ff);

	//WindowsWidgeAdd(hWin, hWin1);

	//hWin3 = WindowsCreate("Windows", 150, 50, 150, 150);
	//WindowsSetColor(hWin3, 0xfff0);

	//WindowsWidgeAdd(hWin, hWin3);

	//hWin4 = WindowsCreate("Windows", 500, 400, 200, 200);
	//WindowsSetColor(hWin4, 0xff00);


	//int i = 0;
	//int x = 0;
	//int flag = 0;
	//char *data=NULL;
	//while (1) {
	//	data=malloc(10);
	//	if (data == NULL) {
	//		break;
	//	}
	//	sprintf(data,"%d",i);
	//	hWIN = WindowsCreate(data, rand()%800, rand()%500,
	//		100, 100);
	//
	//	if (hWIN != 0x00) {
	//		WindowsSetColor(hWIN, rand()%65535);
	//		//WidgeSetColor(hWIN,rand()%65536);
	//		WinListAdd(hWIN);
	//		//HXBUTTON hXButton = ButtonWidgeCreate("this is a button", 10, 10, 50, 50);
	//		//WindowsWidgeAdd(hWIN, hXButton);
	//		winCount++;
	//		/*	if (x > 400&& flag==0) {
	//			x = 200;
	//			i = 0;
	//			flag = 1;
	//		}*/
	//		if (winCount > 100) {
	//			break;
	//		}
	//		//printf("%d", winCount);
	//	}
	//	else {
	//		break;
	//	}
	//}

}

/* 下面是主函数 */int main(void) {
	gui_init();

	HINSTANCE hThisInstance = GetModuleHandle(NULL);
	int nFunsterStil = SW_SHOW;
	HWND hWnd; /* 这是窗口的句柄 */
	MSG messages;/* 应用程序的消息保存在这里 */
	WNDCLASSEX wincl;/* 窗口类的数据结构 */
	 /* 窗口结构 */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;/* 这个函数由Windows操作系统调用 */
	wincl.style = CS_DBLCLKS; /* 获取双击指令 */
	wincl.cbSize = sizeof(WNDCLASSEX);
	/* 使用默认图标和鼠标指针 */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL; /* 没有菜单 */
	wincl.cbClsExtra = 0;/* 窗口类后面没有多余的字节 */
	wincl.cbWndExtra = 0;/* 结构或者窗口实例 */
	/* 使用窗口的默认颜色作为窗口的背景色 */
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	/* 注册窗口类。如果注册失败，那么就退出程序 */
	if (!RegisterClassEx(&wincl))
		return 0;
	/* 窗口类已被注册。创建它的程序 */
	hWnd = CreateWindowEx(0, szClassName,/* 类名 */
	TEXT("Windows App"),/* 窗口标题栏的文字 */
		WS_OVERLAPPEDWINDOW, /* 默认窗口 */
		CW_USEDEFAULT, /* 窗口左上角的位置 */
		CW_USEDEFAULT, /* 窗口右下角的位置 */
		WIN_W, /* 窗口宽度（以“像素”位单位） */
		WIN_H, /* 窗口高度（以“像素”位单位） */
		HWND_DESKTOP,/* 窗口是桌面的子窗口 */
		NULL,/* 该窗口无菜单 */
		hThisInstance, /* 程序实例的句柄 */
		NULL /* 没有窗口创建的数据 */);
	/* 显示窗口 */
	ShowWindow(hWnd, nFunsterStil);
	/* 重绘窗口 */
	UpdateWindow(hWnd);


	//int i = 0;
	int iret = SetTimer(hWnd, 10, 10, (TIMERPROC)TimerProc);
	/* 运行消息循环。循环到GetMessage()函数返回0 */
	while (GetMessage(&messages, NULL, 0, 0)) {
		/* 把虚拟信息翻译成字符信息 */
		TranslateMessage(&messages);
		/* 发送信息到窗口过程 */
		DispatchMessage(&messages);


	}
	/* 返回PostQuitMessage()函数的返回值 */
	return messages.wParam;
}
