// LoveYou.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "LoveYou.h"
#include <ctime>
#include <vector>
using namespace std;
#pragma comment(lib,"msimg32.lib")

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
int cxWidth;                                    // 屏幕宽度
int cyHeight;                                   // 屏幕高度
DWORD g_tPre = 0, g_tNow = 0; 
HWND g_hwnd;
HWND hWnd;
MSG msg;
HDC hdc,hMemDc,hBufDc;
HBITMAP hBmpbg, hBmpSnow;
BITMAP bmpBg, bmpSnow;
typedef struct SNOW
{
	int x, y, cx, cy;
}Snow;

Snow snow = { 0, 0, 0, 0 };
vector<Snow> vsnow(60,snow);

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID                DrawBackground();
VOID                DrawSnow();
VOID                ShowTime();
VOID                ShowText();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOVEYOU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOVEYOU));

	
	// 主消息循环: 
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre>100)
			{
				//Sleep(50);
				DrawBackground();
				g_tPre = GetTickCount();
				ShowTime();
				ShowText();
				DrawSnow();
				
			}
			
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOVEYOU));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = NULL; //(HBRUSH)(COLOR_WINDOW+3);
	wcex.lpszMenuName   = NULL;// MAKEINTRESOURCE(IDC_LOVEYOU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hwnd = hWnd;
   /****************************/
   hdc = GetDC(hWnd);
   hMemDc = CreateCompatibleDC(hdc);
   hBufDc = CreateCompatibleDC(hdc);
   hBmpSnow = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_SNOW));
   SelectObject(hBufDc, hBmpSnow);
   GetObject(hBmpSnow, sizeof(bmpSnow), &bmpSnow);
   hBmpbg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_BG1));
   SelectObject(hMemDc, hBmpbg);
   GetObject(hBmpbg, sizeof(bmpBg), &bmpBg);

   DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
   dwStyle &= ~WS_BORDER;
   dwStyle &= ~WS_CAPTION;
   dwStyle &= ~WS_THICKFRAME;
   SetWindowLong(hWnd, GWL_STYLE, dwStyle);
   cxWidth = GetSystemMetrics(SM_CXSCREEN);
   cyHeight = GetSystemMetrics(SM_CYSCREEN);
   //设置全屏
   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, cxWidth, cyHeight, SWP_SHOWWINDOW);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   srand(time(0));

   /*雪花初始化*/
   for (size_t i = 0; i < vsnow.size(); i++)
   {
	   vsnow[i].x = rand() % cxWidth;
	   vsnow[i].y = rand() % cyHeight;
	   vsnow[i].cx = rand() % 50;
	   vsnow[i].cy = rand() % 50;
   }
   //for (auto s : vsnow)
   //{
	  // s.x = rand() % cxWidth;
	  // s.y = rand() % cyHeight;
	  // /* TransparentBlt(hdc, s.x, s.y, 30, 30,
	  // hBufDc, 0, 0, bmpSnow.bmWidth, bmpSnow.bmHeight, RGB(0, 0, 0));*/

   //}
   ShowCursor(FALSE);
   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_LBUTTONUP:
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	case WM_SYSKEYDOWN:
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	case WM_KEYDOWN:
		SendMessage(hWnd, WM_DESTROY, 0, 0);
	case WM_DESTROY:
		DeleteDC(hBufDc);
		DeleteDC(hMemDc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

VOID DrawBackground()
{
	/*背景*/
	StretchBlt(hdc, 0, 0, cxWidth, cyHeight, hMemDc, 0, 0, bmpBg.bmWidth, bmpBg.bmHeight, SRCCOPY);
}

VOID DrawSnow()
{
	static int x = 0, y = 0;
	for (size_t i = 0; i < vsnow.size(); i++)
	{
		//s.x = rand() % cxWidth;
		TransparentBlt(hdc, vsnow[i].x, vsnow[i].y, vsnow[i].cx, vsnow[i].cx,
			hBufDc, 0, 0, bmpSnow.bmWidth, bmpSnow.bmHeight, RGB(0, 0, 0));
		if (rand() % 2 == 0)
			vsnow[i].x += rand() % 6;
		else
			vsnow[i].x -= rand() % 6;
		vsnow[i].y += 10;
		if (vsnow[i].y > cyHeight)
		{
			vsnow[i].x = rand() % cxWidth;
			vsnow[i].y = 0;
		}
	}
	
}

VOID ShowTime()
{
	HFONT hfont;
	hfont = CreateFontW(
		30,	//高度
		0,		//宽度
		0,	//显示角度
		0,		//
		0,		//磅数
		FALSE,              //非斜体
		FALSE,              //无下划线
		FALSE,	//无删除线
		DEFAULT_CHARSET,    //使用缺省字符集
		OUT_DEFAULT_PRECIS, //缺省输出精度
		CLIP_DEFAULT_PRECIS,//缺省裁减精度
		DEFAULT_QUALITY,    //nQuality=缺省值
		DEFAULT_PITCH,      //nPitchAndFamily=缺省值
		_T("隶书"));         //字体名=@system
	SelectObject(hdc, hfont);
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc, RGB(138, 43, 226));

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	wchar_t bufTime[MAXBYTE];
	wsprintf(bufTime, L"%4d-%02d-%02d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond);
	TextOut(hdc, 950, 600, L"晓东&彩红", wcslen(L"晓东&彩红"));
	TextOut(hdc, 900, 650,bufTime, wcslen(bufTime));
	DeleteObject(hfont);
}

VOID ShowText()
{
	HFONT hfont;
	hfont = CreateFontW(
		30,	//高度
		0,		//宽度
		2700,	//显示角度
		0,		//
		0,		//磅数
		FALSE,              //非斜体
		FALSE,              //无下划线
		FALSE,	//无删除线
		DEFAULT_CHARSET,    //使用缺省字符集
		OUT_DEFAULT_PRECIS, //缺省输出精度
		CLIP_DEFAULT_PRECIS,//缺省裁减精度
		DEFAULT_QUALITY,    //nQuality=缺省值
		DEFAULT_PITCH,      //nPitchAndFamily=缺省值
		_T("@system"));         //字体名=@system
	SelectObject(hdc, hfont);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
	TextOut(hdc, 200, 400, L"最漂亮的人是你，", wcslen(L"最漂亮的人是你,"));
	TextOut(hdc, 250, 400, L"最温柔的人是你，", wcslen(L"最温柔的人是你,"));
	TextOut(hdc, 300, 400, L"最会关心人的人是你，", wcslen(L"最会关心人的人是你,"));
	TextOut(hdc, 350, 400, L"最善解人意的人是你，", wcslen(L"最善解人意的人是你,"));
	TextOut(hdc, 400, 400, L"最爱我的人是你，", wcslen(L"最爱我的人是你,"));
	TextOut(hdc, 450, 400, L"我最想珍惜的人，", wcslen(L"我最想珍惜的人,"));
	TextOut(hdc, 500, 400, L"也是你。。。", wcslen(L"也是你。。。"));
	DeleteObject(hfont);
}
