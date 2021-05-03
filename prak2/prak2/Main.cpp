#include <Windows.h>
#include <tchar.h> 
#include <stdlib.h> 
#include <string.h>
#include <iostream>

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND hwndButton1;
HWND hwndButton2;
HWND hwndButton3;
HWND hwndButton4;
HWND hwndButton5;

HWND hwndText, hwndTextInt;

HANDLE hFileFirstTxt, hFileFirstBin, hOutTxt, hInTxt, hOutBin, hInBin, hOutBinDump;

using namespace std;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstane,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	//window 1 
	WNDCLASSEX wcex;

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

	HWND hWnd = CreateWindow(
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

	ShowWindow(hWnd,
		nCmdShow);
	
	hwndButton1 = CreateWindow(L"BUTTON",   // Название класса
		L"Записать в текстовый",       // Текст на кнопке
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Стиль кнопки
		30,         //  x координаты
		300,         //  y координаты
		170,        // ширина кнопки
		30,        // высота кнопки
		hWnd,       // указатель родительского окна
		(HMENU)1001,       
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	hwndButton2 = CreateWindow(L"BUTTON",   // Название класса
		L"Читать с текстового",       // Текст на кнопке
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Стиль кнопки
		280,         //  x координаты
		300,         //  y координаты
		170,        // ширина кнопки
		30,        // высота кнопки
		hWnd,       // указатель родительского окна
		(HMENU)1002,       
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	hwndButton3 = CreateWindow(L"BUTTON",   // Название класса
		L"Записать в бинарный",       // Текст на кнопке
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Стиль кнопки
		30,         //  x координаты
		350,         //  y координаты
		170,        // ширина кнопки
		30,        // высота кнопки
		hWnd,       // указатель родительского окна
		(HMENU)1003,       
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	hwndButton4 = CreateWindow(L"BUTTON",   // Название класса
		L"Читать с бинарного",       // Текст на кнопке
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Стиль кнопки
		280,         //  x координаты
		350,         //  y координаты
		170,        // ширина кнопки
		30,        // высота кнопки
		hWnd,       // указатель родительского окна
		(HMENU)1004,       
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);

	hwndText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), NULL,
		WS_CHILD | WS_VISIBLE, 100, 20, 140,
		20, hWnd, NULL, NULL, NULL);

	hwndTextInt = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), NULL,
		WS_CHILD | WS_VISIBLE, 100, 50, 140,
		20, hWnd, NULL, NULL, NULL);

	UpdateWindow(hWnd);

	hFileFirstTxt = CreateFile(_T("C:\\Users\\deftr\\OneDrive\\Documents\\spo\\readme.txt"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(HANDLE(hFileFirstTxt));

	hFileFirstBin = CreateFile(_T("C:\\Users\\deftr\\OneDrive\\Documents\\spo\\readme.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(HANDLE(hFileFirstBin));

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
};

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
	case WM_COMMAND:
	{

		if (wParam == 1001)
		{

			hInTxt = CreateFile(_T("C:\\Users\\deftr\\OneDrive\\Documents\\spo\\readme.txt"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hInTxt == INVALID_HANDLE_VALUE) {
				//для вывода ошибки
				TCHAR s[100];
				_stprintf_s(s, _T("%X"), GetLastError());
				MessageBox(0, s, L"Ошибка", MB_OK);

				return 2;
			}

			TCHAR str[100];

			wchar_t* wEndString;
			char* cEndString;

			char s[100];
			int iBuff;

			//SendMessage(hwndText, WM_GETTEXT, 100, LPARAM(str)); //считали
			GetWindowText(hwndText, str, sizeof(str));
			GetWindowTextA(hwndTextInt, s, sizeof(iBuff));

			iBuff = strtol(s, &cEndString, 0);
			wsprintf(str, _T("%s %d\n"), str, iBuff);

			WriteFile(hInTxt, str, sizeof(str), NULL, NULL);

			CloseHandle(HANDLE(hInTxt));
			break;
		}

		if (wParam == 1002)
		{
			hOutTxt = CreateFile(_T("C:\\Users\\deftr\\OneDrive\\Documents\\spo\\readme.txt"), GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hOutTxt == INVALID_HANDLE_VALUE) {
				//для вывода ошибки
				TCHAR s[100];
				_stprintf_s(s, _T("%X"), GetLastError());
				MessageBox(0, s, L"Ошибка", MB_OK);

				return 2;
			}

			DWORD dwTemp;
			TCHAR str[100];

			ReadFile(hOutTxt, str, sizeof(str), &dwTemp, NULL);

			MessageBox(hWnd, str, L"TXT", MB_OK);

			CloseHandle(HANDLE(hOutTxt));
			break;
		}

		if (wParam == 1003)
		{
			hInBin = CreateFile(_T("C:\\Users\\deftr\\OneDrive\\Documents\\spo\\readme.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hInBin == INVALID_HANDLE_VALUE) {
				//для вывода ошибки
				TCHAR s[100];
				_stprintf_s(s, _T("%X"), GetLastError());
				MessageBox(0, s, L"Ошибка", MB_OK);

				return 2;
			}

			TCHAR text[100];
			//SendMessage(hwndText, WM_GETTEXT, 100, LPARAM(text)); //считали

			wchar_t* wEndString;
			char* cEndString;

			char s[100];
			int iBuff, sizeOfStr;

			GetWindowText(hwndText, text, sizeof(text));
			GetWindowTextA(hwndTextInt, s, sizeof(iBuff));

			iBuff = strtol(s, &cEndString, 0);
			sizeOfStr = (GetWindowTextLength(hwndText) + 1) * sizeof(TCHAR);


			WriteFile(hInBin, &sizeOfStr, sizeof(int), NULL, 0);
			WriteFile(hInBin, text, sizeOfStr, NULL, 0);
			WriteFile(hInBin, &iBuff, sizeof(int), NULL, 0);

			CloseHandle(HANDLE(hInBin));
			break;
		}

		if (wParam == 1004)
		{
			hOutBin = CreateFile(_T("C:\\Users\\deftr\\OneDrive\\Documents\\spo\\readme.bin"), GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hOutBin == INVALID_HANDLE_VALUE) {
				//для вывода ошибки
				TCHAR s[100];
				_stprintf_s(s, _T("%X"), GetLastError());
				MessageBox(0, s, L"Ошибка", MB_OK);

				return 2;
			}

			DWORD dwTemp;
			TCHAR str[100];
			int sizeOfStr, number;

			ReadFile(hOutBin, &sizeOfStr, sizeof(int), &dwTemp, NULL);
			ReadFile(hOutBin, str, sizeOfStr, &dwTemp, NULL);
			ReadFile(hOutBin, &number, sizeof(number), &dwTemp, NULL);

			wsprintf(str, _T("%s %d\n") , str, number);
			

			MessageBox(hWnd, str, L"BIN", MB_OK);


			CloseHandle(HANDLE(hOutBin));
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

