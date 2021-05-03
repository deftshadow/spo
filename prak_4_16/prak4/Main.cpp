#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>


#define ID_TIMER_1 1
#define ID_TIMER_2 2


static TCHAR szMainWindowClass[] = _T("MainWindowClass");
static TCHAR szMainWindowTitle[] = _T("MainWindow");

HINSTANCE hInst;

UINT uFirstDeltaTime = 1000;

BOOL bControlFlag = false;

int nCmdSh;

RECT rect;

WNDCLASSEX wcexChild;
HWND hWndChild;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcChild(HWND, UINT, WPARAM, LPARAM);


int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW; // CS_DBLCLKS ��������� ����������� ������� ���� ����
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szMainWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance;
    nCmdSh = nCmdShow;

    HWND hWnd = CreateWindow(
        szMainWindowClass,
        szMainWindowTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        600, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    SetTimer(hWnd, ID_TIMER_1, uFirstDeltaTime, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int sx, sy;
    switch (message)
    {
    case WM_TIMER:
        switch (wParam) {
        case ID_TIMER_1: {

            KillTimer(hWnd, ID_TIMER_1);

            wcexChild.cbSize = sizeof(WNDCLASSEX);
            wcexChild.style = CS_HREDRAW | CS_VREDRAW;
            wcexChild.lpfnWndProc = WndProcChild;
            wcexChild.cbClsExtra = 0;
            wcexChild.cbWndExtra = 0;
            wcexChild.hInstance = hInst;
            wcexChild.hIcon = LoadIcon(hInst, IDI_APPLICATION);
            wcexChild.hCursor = LoadCursor(NULL, IDC_ARROW);
            wcexChild.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcexChild.lpszMenuName = NULL;
            wcexChild.lpszClassName = _T("ChildWindow");
            wcexChild.hIconSm = LoadIcon(wcexChild.hInstance, IDI_APPLICATION);

            if (!RegisterClassEx(&wcexChild))
            {
                MessageBox(NULL,
                    _T("Call to RegisterClassEx failed! (wcexChild)"),
                    _T("Windows Desktop Guided Tour"),
                    NULL);

                return 1;
            }


            hWndChild = CreateWindow(
                _T("ChildWindow"),
                _T("ChildWindow"),
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                200, 200,
                200, 200,
                hWnd,
                NULL,
                hInst,
                NULL
            );

            if (!hWndChild)
            {
                MessageBox(NULL,
                    _T("Call to CreateWindow failed! (hWndSecondChild)"),
                    _T("Windows Desktop Guided Tour"),
                    NULL);

                return 1;
            }     

            HWND hWindow = hWnd; // ���������� ������� ����, �������� ����� �������� �����
            DWORD dwThreadID = GetWindowThreadProcessId(hWindow, NULL); // �������� ������������� ������ ����
            DWORD dwThisThreadID = GetWindowThreadProcessId(hWnd, NULL);
            AttachThreadInput(dwThisThreadID, dwThreadID, TRUE); // �������������� � ������ ��������� �����
            SetFocus(hWindow); // �������� ��������� ��� ��������� ������
            AttachThreadInput(dwThisThreadID, dwThreadID, FALSE); // �������������

            break;
        }
        default:
            break;
        }
        break;

    case WM_KEYDOWN:

        switch (wParam) {

        case VK_RETURN: // ������������ ������� ENTER 
            TCHAR str[128];
            GetClientRect(hWndChild, &rect); // ����� ��������� ������� ����?

            sx = rect.right; //������
            sy = rect.bottom; //������

            swprintf_s(str, _T("������: %d ������: %d"),
                sx, sy);

            CreateWindow(TEXT("STATIC"), str,
                WS_VISIBLE | WS_CHILD,
                0, 0, 100, 30,
                hWndChild, (HMENU)NULL, NULL, NULL);
            break;

        default:
            break;
        }

        break;
    
    case WM_LBUTTONDOWN:
        ShowWindow(hWndChild, SW_HIDE);
        break;
    case WM_RBUTTONDOWN:
        ShowWindow(hWndChild, SW_SHOW);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}




LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}