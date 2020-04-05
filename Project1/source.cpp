#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("19032087 최병준 오목");

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

#define width 19
#define length 19
#define gap 50
#define XPOS(x) (50 + (x) * gap)
#define YPOS(y) (50 + (y) * gap)
#define Black 1
#define White 2

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static int x, y;
	static int cal_x = 0, cal_y = 0;
	static int turn = 0;
	static int Board[length][width] = { NULL };
	static int board_x, board_y;
	static int color = 1;
	static int temp;
	static int score[5] = { 0, 0, 0, 0, 0 };
	static bool restart = false;

	switch (iMessage) {

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_clear:				//재시작 메뉴
			if (MessageBox(hWnd, TEXT("초기화 하시겠습니까? "), TEXT("Menu"), MB_YESNO) == IDYES)
			{
				restart = true;
				for (int i = 0; i < width; i++)
				{
					for (int j = 0; j < length; j++)
					{
						Board[i][j] = { NULL };
					}
				}
				turn = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;

		case ID_exit:				//종료 메뉴
			if (MessageBox(hWnd, TEXT("종료 하시겠습니까? "), TEXT("Menu"), MB_YESNO) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
	}

	case WM_LBUTTONDOWN:
	{
		x = LOWORD(lParam);			
		y = HIWORD(lParam);

		if ((x % 50) > 24)					//입력받은 좌표값을 바둑돌의 칸으로 위치 보정
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

		board_x = (cal_x - gap) / gap;		//바둑돌의 2중배열의 위치값 계산
		board_y = (cal_y - gap) / gap;

		if (Board[board_y][board_x] == 0)	//위치값이 위치하는곳에 바둑돌이 없는 경우에만 바둑돌 배치
		{
			switch (turn % 2) {
			case 0:
				Board[board_y][board_x] = Black;
				color = Black;
				break;
			case 1:
				Board[board_y][board_x] = White;
				color = White;
				break;
			}
		}
		else if (restart != true)			//위치값에 이미 바둑돌이 있는 경우 출력
		{
			MessageBox(hWnd, L"이미 돌이 놓여있습니다.", TEXT("오류"), MB_OK);
			color = turn % 2 + 1;
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_LBUTTONUP:							//바둑돌이 5개가 되었을 경우 승리 조건 계산
	{
		int i = 0, j = 0, k = 0;

		for (i = 0; i < width; i++)
		{
			for (j = 0; j < length; j++)
			{
				if (Board[i][j] == Black)
				{
					score[0] = Black;
					for (k = 1; k < 5; k++)
					{
						if (Board[i][j + k] == Black)  //가로 검사
						{
							score[k] = Black;
						}
						else if (Board[i + k][j] == Black) //세로 검사
						{
							score[k] = Black;
						}
						else if (Board[i + k][j + k] == Black) // 왼쪽 > 오른쪽 대각선 검사
						{
							score[k] = Black;
						}
						else if (Board[i + k][j - k] == Black) // 오른쪽 > 왼쪽 대각선 검사
						{
							score[k] = Black;
						}
					}

					if (score[0] == Black && score[1] == Black && score[2] == Black && score[3] == Black && score[4] == Black)
					{
						MessageBox(hWnd, L"검은 돌 승리", L"게임 결과", 0);
						PostQuitMessage(0);
						break;
					}
					else
					{
						for (k = 0; k < 5; k++)
						{
							score[k] = 0;
						}
					}
				}

				else if (Board[i][j] == White)
				{
					score[0] = White;
					for (k = 1; k < 5; k++)
					{
						if (Board[i][j + k] == White)  //가로 검사
						{
							score[k] = White;
						}
						else if (Board[i + k][j] == White) //세로 검사
						{
							score[k] = White;
						}
						else if (Board[i + k][j + k] == White) // 왼쪽 > 오른쪽 대각선 검사
						{
							score[k] = White;
						}
						else if (Board[i + k][j - k] == White) // 오른쪽 > 왼쪽 대각선 검사
						{
							score[k] = White;
						}
					}

					
					if (score[0] == White && score[1] == White && score[2] == White && score[3] == White && score[4] == White)
					{
						MessageBox(hWnd, L"흰 돌 승리", L"게임 결과", 0);
						PostQuitMessage(0);
						break;
					}
					else
					{
						for ( k = 0; k < 5; k++)
						{
							score[k] = 0;
						}
					}
				}
			}
		}

		turn++;
		restart = false;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}

	case WM_PAINT:										//바둑판과 바둑돌을 출력
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HBRUSH hBrush, oldBrush;

		for (int x = 0; x < width; x++)
		{
			MoveToEx(hdc, XPOS(x), YPOS(0), NULL);
			LineTo(hdc, XPOS(x), YPOS(length - 1));
		}

		for (int y = 0; y < length; y++)
		{
			MoveToEx(hdc, XPOS(0), YPOS(y), NULL);
			LineTo(hdc, XPOS(width - 1), YPOS(y));
		}

		switch (color) {								//턴수에 따라 바둑돌의 색깔지정
		case 1:
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			break;

		case 2:
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			break;
		}

		if (cal_x >= 50 && cal_x <= 950 && cal_y >= 50 && cal_y <= 950)			//바둑판내에서만 바둑돌이 그려짐
		{
			Ellipse(hdc, cal_x - 25, cal_y - 25, cal_x + 25, cal_y + 25);
		}

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		system("pause>NUL");
		return 0;
	}

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}