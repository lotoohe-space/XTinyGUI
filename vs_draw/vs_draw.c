#pragma comment(linker,"/SUBSYSTEM:Windows  /ENTRY:mainCRTStartup")
#include <windows.h>/* 使类名成为全局变量 */
#include <stdio.h>

#include "msg.h"
#include "gui.h"
#include "widge.h"
#include "xwindows.h"
#include "button_widge.h"
TCHAR szClassName[] = TEXT("WindowsApp");


// 用于注册的窗口类名
const char g_szClassName[] = "myWindowClass";
HWIN winList[2001];
HDC memDC;
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


void d_pix(int x,int y,int color) {
	//需要移植的函数
	SetPixel(memDC, x, y, color);
}

void fill_rect(int x,int y,int w,int h,int color) {
	RECT rect;
	rect.left = x;
	rect.right = x + w;
	rect.top = y;
	rect.bottom = y + h;
	int n_color = color;
	HBRUSH hBrush = CreateSolidBrush(
		n_color
	);
	//需要移植的函数
	FillRect(
		memDC, // 绘图设备 
		&rect, // 矩形区域 
		hBrush// 填充画刷 
	);
	DeleteObject(hBrush);
}
//extern void RectCutFindX(void);
//extern void RectDraw_text(void);
HWIN hWin;
HWIN hWin1;
unsigned char r=0, g=0, b=0;
int flagGarHV = 0;
int i = 50;
int draw_flag = 0;
HWIN bottomHWIN;
void Paint(HWND hwnd)
{
	// paint struct 绘图结构体，存储目标窗口可以绘图的客户端区域(client area)
//	PAINTSTRUCT ps;
	HDC hdc;   // DC(可画图的内存对象) 的句柄
	RECT rect;

	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	memDC = hdc;


	draw_flag = 1;

	GUIEvent();
	GUIExec();
	
	//GUISendMsg(winList[rand()%1000], MSG_WIN, MSG_WIN_MOVE, rand()%800, rand()%500);
	//if (i % 3 == 0) {
	//	GUISendMsg(bottomHWIN, MSG_WIN, MSG_WIN_MOVE, i, i);
	//}
	////WindowsMoveTo(bottomHWIN, i, i);
	//i += 3;
	//if (i > 500) {
	//	i = 50;
	//}
	//draw_flag = 0;

	ReleaseDC(hwnd, hdc);
	ValidateRect(hwnd, &rect);//如果没有这个函数调用，会一直闪动，说明在在一直重绘

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
		 if (draw_flag == 1) {
			 GUISendTouchMsg(MSG_TOUCH_PRESS, x, y);
			 draw_flag = 0;
		 }

		 flag = 1;
		break;
	case WM_LBUTTONUP:
		if (flag) 
		{
			x = LOWORD(lParam);//鼠标的横坐标		
			y = HIWORD(lParam);//鼠标的纵坐标 		/*字符串、整形的拼接*/	
			if (draw_flag == 1) {
				GUISendTouchMsg(MSG_TOUCH_RELEASE, x, y);
				draw_flag = 0;
			}

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
		   /*WindowsMoveTo(hWin1, x, y);*/
			if (draw_flag == 1) {
				GUISendTouchMsg(MSG_TOUCH_MOVE, x, y);
				draw_flag = 0;
			}

			//InvalidateRect(hWnd, NULL, TRUE);
			//printf("x:%d,y:%d\n", x, y);
			
			//UpdateWindow(hWnd); 		
			//TextOut(hdc,20,20,msg.c_str(),msg.length());		
			//ReleaseDC(hWnd,hdc);//搞完人走带门，释放这个HDC。	
		}
		break;
	}
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);		// TODO: 在此添加任意绘图代码...		
		Paint(hWnd);
		//EndPaint(hWnd, &ps);
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

void btnCallBack(void* Object, uint8 status) {
	if (status) {
		HWIN hWIN;
		hWIN = WindowsCreate("test", rand()%700, rand()%400,
		100, 100);
		WindowsSetColor(hWIN, 0x00ff);
		WinListAdd(hWIN);
	}
}
int winCount = 0;
#include <stdlib.h>
void gui_init(void) {
	GUIInit();
	
	HWIN hWin3;
	HWIN hWin2;
	HWIN hWin4;


	//hWin4 = WindowsCreate("Windows", 100, 100, 100, 100);

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


	HWIN hWIN;
	hWIN = WindowsCreate("test", 650, 100,
		150, 150);
	WindowsSetColor(hWIN, 0xfff0);
	WinListAdd(hWIN);
	HXBUTTON hXButton = ButtonWidgeCreate("this is a button", 10, 10, 50, 50);
	WindowsWidgeAdd(hWIN, hXButton);
	WidgeSetClickBack(hXButton,btnCallBack);

	hWin = WindowsCreate("Win", 350, 150, 400, 300);
	WindowsSetColor(hWin, 0x0ff0);
	WinListAdd(hWin);

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
	//	winList[i] = hWIN;
	//	if (i == 0) {
	//		bottomHWIN = hWIN;
	//	}
	//	i ++;
	//	x++;
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
	//		if (winCount > 1000) {
	//			break;
	//		}
	//		//printf("%d", winCount);
	//	}
	//	else {
	//		break;
	//	}
	//}


}

/* 下面是主函数 */
int main(void) {
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
		1024, /* 窗口宽度（以“像素”位单位） */
		700, /* 窗口高度（以“像素”位单位） */
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
