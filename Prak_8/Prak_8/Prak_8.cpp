#include <windows.h>
#include <stdio.h>
#include <iostream>

CRITICAL_SECTION cs;
HANDLE hMutex;
HANDLE hEvent1, hEvent2;

int val;
unsigned long uThrID;

using namespace std;

void CriticalThread()
{
    while (1)
    {
        EnterCriticalSection(&cs);
        val++;
        LeaveCriticalSection(&cs);
    }
}

void MutexlThread()
{
    while (1)
    {
        WaitForSingleObject(hMutex, INFINITE);
        val++;
        ReleaseMutex(hMutex);
    }
}

void EventThread()
{
    while (1)
    {
        WaitForSingleObject(hEvent2, INFINITE);
        val++;
        SetEvent(hEvent1);
    }
}

int main()
{
    cout << "1. Critical section \n2. Mutex \n3. Event \n";

    int v;
    cin >> v;

    switch (v)
    {
    case 1:
        InitializeCriticalSection(&cs);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CriticalThread, NULL, 0, &uThrID);

        while (1)
        {
            EnterCriticalSection(&cs);
            printf("%d\n", val);
            LeaveCriticalSection(&cs);
        }

        break;

    case 2:
        hMutex = CreateMutex(NULL, FALSE, NULL);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MutexlThread, NULL, 0, &uThrID);

        while (1)
        {
            WaitForSingleObject(hMutex, INFINITE);
            printf("%d\n", val);
            ReleaseMutex(hMutex);
        }

        break;

    case 3:
        hEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL);
        hEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EventThread, NULL, 0, &uThrID);

        while (1)
        {
            WaitForSingleObject(hEvent1, INFINITE);
            printf("%d\n", val);
            SetEvent(hEvent2);
        }

        break;

    default:
        cout << "Wrong choice";
        break;

    }

    return 0;
}