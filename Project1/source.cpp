#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = CreateSolidBrush(RGB(244, 176, 77));
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 1024,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
#define MAX_CIRCLE 20

#include <math.h>
#define BSIZE 50
#define width 19
#define length 19
#define gap 50
#define XPOS(x) (50 + (x) * gap)
#define YPOS(y) (50 + (y) * gap)

void onPaint(HWND ah_wnd, int cal_x , int cal_y)
{
	PAINTSTRUCT ps;
	HDC h_dc = BeginPaint(ah_wnd, &ps);
	static HBRUSH hBrush, oldBrush;
	static TCHAR temp[100];
	static int Board[length][width] = { 0 };
	static int board_x, board_y;
	static int turn = 0;

	board_x = (cal_x - gap) / gap;
	board_y = (cal_y - gap) / gap;

	for (int x = 0; x < width; x++)
	{
		MoveToEx(h_dc, XPOS(x), YPOS(0), NULL);
		LineTo(h_dc, XPOS(x), YPOS(length - 1));
	}

	for (int y = 0; y < length; y++)
	{
		MoveToEx(h_dc, XPOS(0), YPOS(y), NULL);
		LineTo(h_dc, XPOS(width - 1), YPOS(y));
	}

	switch (turn % 2) {
	case 0:
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(h_dc, hBrush);
		break;
	case 1:
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(h_dc, hBrush);
		break;
	}

	if (Board[board_x][board_y] > 0)
	{
		MessageBox(ah_wnd, temp, TEXT("이미 돌이 놓여있습니다."), MB_OK);
	}
	else if (cal_x >= 50 && cal_x <= 950 && cal_y >= 50 && cal_y <= 950)
	{
		Ellipse(h_dc, cal_x - 25, cal_y - 25, cal_x + 25, cal_y + 25);
		Board[board_x][board_y] = turn % 2;
		turn++;
	}

	SelectObject(h_dc, oldBrush);
	DeleteObject(hBrush);
	EndPaint(ah_wnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x, y;
	static int cal_x = 0, cal_y = 0;

	switch (iMessage) {
	case WM_CREATE:
		return 0;

	case WM_PAINT:
	{
		onPaint(hWnd, cal_x, cal_y);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		if ((x % 50) > 24)
		{
			cal_x = x - (x % 50) + 50;
		}
		else
		{
			cal_x = (x - (x % 50));
		}

		if ((y % 50) > 24)
		{
			cal_y = y - (y % 50) + 50;
		}
		else
		{
			cal_y = (y - (y % 50));
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
