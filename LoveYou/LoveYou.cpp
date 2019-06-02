// LoveYou.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LoveYou.h"
#include <ctime>
#include <vector>
using namespace std;
#pragma comment(lib,"msimg32.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
int cxWidth;                                    // ��Ļ���
int cyHeight;                                   // ��Ļ�߶�
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

// �˴���ģ���а����ĺ�����ǰ������: 
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

 	// TODO:  �ڴ˷��ô��롣
	
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOVEYOU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOVEYOU));

	
	// ����Ϣѭ��: 
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
   //����ȫ��
   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, cxWidth, cyHeight, SWP_SHOWWINDOW);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   srand(time(0));

   /*ѩ����ʼ��*/
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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
	/*����*/
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
		30,	//�߶�
		0,		//���
		0,	//��ʾ�Ƕ�
		0,		//
		0,		//����
		FALSE,              //��б��
		FALSE,              //���»���
		FALSE,	//��ɾ����
		DEFAULT_CHARSET,    //ʹ��ȱʡ�ַ���
		OUT_DEFAULT_PRECIS, //ȱʡ�������
		CLIP_DEFAULT_PRECIS,//ȱʡ�ü�����
		DEFAULT_QUALITY,    //nQuality=ȱʡֵ
		DEFAULT_PITCH,      //nPitchAndFamily=ȱʡֵ
		_T("����"));         //������=@system
	SelectObject(hdc, hfont);
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc, RGB(138, 43, 226));

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	wchar_t bufTime[MAXBYTE];
	wsprintf(bufTime, L"%4d-%02d-%02d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond);
	TextOut(hdc, 950, 600, L"����&�ʺ�", wcslen(L"����&�ʺ�"));
	TextOut(hdc, 900, 650,bufTime, wcslen(bufTime));
	DeleteObject(hfont);
}

VOID ShowText()
{
	HFONT hfont;
	hfont = CreateFontW(
		30,	//�߶�
		0,		//���
		2700,	//��ʾ�Ƕ�
		0,		//
		0,		//����
		FALSE,              //��б��
		FALSE,              //���»���
		FALSE,	//��ɾ����
		DEFAULT_CHARSET,    //ʹ��ȱʡ�ַ���
		OUT_DEFAULT_PRECIS, //ȱʡ�������
		CLIP_DEFAULT_PRECIS,//ȱʡ�ü�����
		DEFAULT_QUALITY,    //nQuality=ȱʡֵ
		DEFAULT_PITCH,      //nPitchAndFamily=ȱʡֵ
		_T("@system"));         //������=@system
	SelectObject(hdc, hfont);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
	TextOut(hdc, 200, 400, L"��Ư���������㣬", wcslen(L"��Ư����������,"));
	TextOut(hdc, 250, 400, L"������������㣬", wcslen(L"�������������,"));
	TextOut(hdc, 300, 400, L"�������˵������㣬", wcslen(L"�������˵�������,"));
	TextOut(hdc, 350, 400, L"���ƽ�����������㣬", wcslen(L"���ƽ������������,"));
	TextOut(hdc, 400, 400, L"��ҵ������㣬", wcslen(L"��ҵ�������,"));
	TextOut(hdc, 450, 400, L"��������ϧ���ˣ�", wcslen(L"��������ϧ����,"));
	TextOut(hdc, 500, 400, L"Ҳ���㡣����", wcslen(L"Ҳ���㡣����"));
	DeleteObject(hfont);
}
