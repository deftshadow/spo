#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

HANDLE hCell;
LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");

BOOL ReadSlot()
{
    BOOL fResult;
    DWORD cAllMessages;
    OVERLAPPED ov;
    DWORD cbMessage, cMessage, cbRead;
    LPTSTR lpszBuffer;
    TCHAR achID[80];
    HANDLE hEvent;


    cbMessage = cMessage = cbRead = 0;

    hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleCell"));
    if (NULL == hEvent)
        return FALSE;
    ov.Offset = 0;
    ov.OffsetHigh = 0;
    ov.hEvent = hEvent;

    fResult = GetMailslotInfo(hCell,
        (LPDWORD)NULL,     
        &cbMessage,               
        &cMessage,                   
        (LPDWORD)NULL);              

    if (!fResult)
    {
        printf("GetMailslotInfo failed with %d.\n", GetLastError());
        return FALSE;
    }

    if (cbMessage == MAILSLOT_NO_MESSAGE)
    {
        printf("Waiting for a message...\n");
        return TRUE;
    }

    cAllMessages = cMessage;

    while (cMessage != 0)
    {

        StringCchPrintf((LPTSTR)achID,
            80,
            TEXT("\n# #%d/%d\n"),
            cAllMessages - cMessage + 1,
            cAllMessages);

        lpszBuffer = (LPTSTR)GlobalAlloc(GPTR,
            lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage);
        if (NULL == lpszBuffer)
            return FALSE;
        lpszBuffer[0] = '\0';

        fResult = ReadFile(hCell,
            lpszBuffer,
            cbMessage,
            &cbRead,
            &ov);

        if (!fResult)
        {
            printf("ReadFile failed with %d.\n", GetLastError());
            GlobalFree((HGLOBAL)lpszBuffer);
            return FALSE;
        }

        StringCbCat(lpszBuffer,
            lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage,
            (LPTSTR)achID);

        _tprintf(TEXT("Contents of the mailslot: %s\n"), lpszBuffer);

        GlobalFree((HGLOBAL)lpszBuffer);

        fResult = GetMailslotInfo(hCell,
            (LPDWORD)NULL,     
            &cbMessage,       
            &cMessage,                     
            (LPDWORD)NULL);         

        if (!fResult)
        {
            printf("GetMailslotInfo failed (%d)\n", GetLastError());
            return FALSE;
        }
    }
    CloseHandle(hEvent);
    return TRUE;
}

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

void main()
{
    MakeSlot(SlotName);

    while (TRUE)
    {
        ReadSlot();
        Sleep(3000);
    }
}
