#include <Windows.h>
#include <tchar.h> 
#include <stdlib.h> 
#include <string.h>
#include <iostream>

HINSTANCE hInst;

HWND hWndScrollR, hWndScrollG, hWndScrollB;
HWND hWndAccpetButton, hWndProsto;
WNDCLASSEX wcex, wcex2;
HWND hWnd, hWnd2;

TCHAR ChildName[] = _T("ChildFrame");

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

// Текущая позиция полосы просмотра
int nPosition;

using namespace std;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstane,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	//window 1 
	wcex;

	HBRUSH greenBrush = CreateSolidBrush(RGB(204, 255, 204));
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = greenBrush;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"window class1";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL,
			_T("Window class creation failed for window 1"),
			_T("Window Class Failed"),
			MB_ICONERROR);

		return 1;
	}

	hWnd = CreateWindow(
		wcex.lpszClassName,
		L"Parent Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 500,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow1 failed!"),
			_T("Window Creation Filed"),
			MB_ICONERROR);

		return 1;
	}
	
	UpdateWindow(hWnd);

	HBRUSH brush = CreateSolidBrush(RGB(204, 200, 204));
	ZeroMemory(&wcex2, sizeof(WNDCLASSEX));
	wcex2.cbSize = sizeof(WNDCLASSEX);
	wcex2.style = CS_HREDRAW | CS_VREDRAW;
	wcex2.lpfnWndProc = ChildProc;
	wcex2.cbClsExtra = 0;
	wcex2.cbWndExtra = 0;
	wcex2.hInstance = hInstance;
	wcex2.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex2.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex2.hbrBackground = greenBrush;
	wcex2.lpszMenuName = NULL;
	wcex2.lpszClassName = L"window class2";
	wcex2.hIconSm = LoadIcon(wcex2.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex2)) {
		MessageBox(NULL,
			_T("Window class creation failed for window 2"),
			_T("Window Class Failed"),
			MB_ICONERROR);

		return 1;
	}

	hWnd2 = CreateWindow(
		wcex2.lpszClassName,
		L"Parent Window",
		WS_THICKFRAME | WS_CHILD,
		250, 0,
		200, 200,
		hWnd,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd2)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow1 failed!"),
			_T("Window Creation Filed"),
			MB_ICONERROR);

		return 1;
	}

	
	UpdateWindow(hWnd2);
	
	hWndScrollR = CreateWindow(_T("scrollbar"), NULL, WS_CHILD | WS_VISIBLE,
		10, 10, 200, 20, hWnd, (HMENU)1, hInst, NULL);
	SetScrollRange(hWndScrollR, SB_CTL, 0, 255, TRUE);

	hWndScrollG = CreateWindow(_T("scrollbar"), NULL, WS_CHILD | WS_VISIBLE,
		10, 50, 200, 20, hWnd, (HMENU)2, hInst, NULL);
	SetScrollRange(hWndScrollG, SB_CTL, 0, 255, TRUE);

	hWndScrollB = CreateWindow(_T("scrollbar"), NULL, WS_CHILD | WS_VISIBLE,
		10, 90, 200, 20, hWnd, (HMENU)3, hInst, NULL);
	SetScrollRange(hWndScrollB, SB_CTL, 0, 255, TRUE);
	

	hWndAccpetButton = CreateWindow(_T("BUTTON"), _T("ПРИМЕНИТЬ"), WS_CHILD | WS_VISIBLE,
		10, 150, 100, 20, hWnd, (HMENU)1001, hInst, NULL);

	SetParent(hWnd2, hWnd);
	ShowWindow(hWnd,
		nCmdShow);
	ShowWindow(hWnd2,
		nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
};

int i;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc,
			5, 5,
			NULL, NULL);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_HSCROLL:
	{
		if (hWndScrollR == (HWND)lParam) i = 1;
		if (hWndScrollG == (HWND)lParam) i = 2;
		if (hWndScrollB == (HWND)lParam) i = 3;

		// В зависимости от параметра сообщения
		// изменяем текущую позицию
		switch (wParam)
		{
		case SB_PAGEDOWN:
		{
			nPosition += 10;
			break;
		}
		case SB_LINEDOWN:
		{
			nPosition += 1;
			break;
		}
		case SB_PAGEUP:
		{
			nPosition -= 10;
			break;
		}
		case SB_LINEUP:
		{
			nPosition -= 1;
			break;
		}
		case SB_TOP:
		{
			nPosition = 0;
			break;
		}
		case SB_BOTTOM:
		{
			nPosition = 200;
			break;
		}
		case SB_THUMBPOSITION:
		{
			nPosition = LOWORD(lParam);
			break;
		}
		case SB_THUMBTRACK:
		{
			nPosition = LOWORD(lParam);
			break;
		}
		default:
			break;
		}

		// Ограничиваем пределы изменения текущей
		// позиции значениями от 1 до 200
		if (nPosition > 255) nPosition = 255;
		if (nPosition < 0) nPosition = 0;

		// Устанавливаем ползунок полосы просмотра
		// в соответствии с новым значением
		// текущей позиции
		switch (i) {
		case 1:
			SetScrollPos(hWndScrollR, SB_CTL, nPosition, TRUE);
			break;
		case 2:
			SetScrollPos(hWndScrollG, SB_CTL, nPosition, TRUE);
			break;
		case 3:
			SetScrollPos(hWndScrollB, SB_CTL, nPosition, TRUE);
			break;
		}
		

		return 0;
	}
	case WM_COMMAND:
	{
		

		switch(wParam) {

			case 1001: { 
				int r = GetScrollPos(hWndScrollR, SB_CTL);
				int g = GetScrollPos(hWndScrollG, SB_CTL);
				int b = GetScrollPos(hWndScrollB, SB_CTL);

				HBRUSH brush = CreateSolidBrush(RGB(r, g, b));

				SetClassLongPtr(hWnd2, GCL_HBRBACKGROUND, (LONG)brush);
				InvalidateRect(hWnd2, NULL, TRUE);

				break;
			}
			

			default:
				break;
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
};

LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc,
			5, 5,
			NULL, NULL);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
	
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
};


