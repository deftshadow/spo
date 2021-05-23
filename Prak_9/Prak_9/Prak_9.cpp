#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

HANDLE hCell;
LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");

BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName)
{
    hCell = CreateMailslot(lpszSlotName,
        0,                      
        MAILSLOT_WAIT_FOREVER,
        (LPSECURITY_ATTRIBUTES)NULL);

    if (hCell == INVALID_HANDLE_VALUE)
    {
        printf("CreateMailslot failed with %d\n", GetLastError());
        return FALSE;
    }
    return TRUE;
}

BOOL WriteSlot(HANDLE hCell, LPCTSTR lpszMessage)
{
    BOOL fResult;
    DWORD cbWritten;

    fResult = WriteFile(hCell,
        lpszMessage,
        (DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),
        &cbWritten,
        (LPOVERLAPPED)NULL);

    if (!fResult)
    {
        printf("WriteFile failed with %d.\n", GetLastError());
        return FALSE;
    }

    printf("Cell written to successfully.\n");

    return TRUE;
}

int main()
{
    MakeSlot(SlotName);

    HANDLE hFile;

    hFile = CreateFile(SlotName,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed with %d.\n", GetLastError());
    }


    while (TRUE)
    {
        WriteSlot(hFile, TEXT("I'm here!"));
        Sleep(3000);
    }
}
