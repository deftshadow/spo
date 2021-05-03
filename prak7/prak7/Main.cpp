#include <Windows.h> 
#include <iostream> 
#include <conio.h>

using namespace std;

SYSTEM_INFO SysInfo;

void writeSystemInfo() {

    
    GetSystemInfo(&SysInfo);

    cout << "Размер страницы: " << SysInfo.dwPageSize << endl;
    cout << "Минимальный адрес приложения: " << SysInfo.lpMinimumApplicationAddress << endl;
    cout << "Максимальный адрес приложения: " << SysInfo.lpMaximumApplicationAddress << endl;
    cout << "Активная маска процессора: " << SysInfo.dwActiveProcessorMask << endl;
    cout << "Количество ядер процессора: " << SysInfo.dwNumberOfProcessors << endl;
    cout << "Тип процессора: " << SysInfo.dwProcessorType << endl;
    cout << "Гранулярность: " << SysInfo.dwAllocationGranularity << endl;
    cout << "Уровень процессора: " << SysInfo.wProcessorLevel << endl;
    cout << "Пересмотр процессора?: " << SysInfo.wProcessorRevision << endl;
}
void writeMemoryInfo() {

    MEMORYSTATUS MemStat;
    MemStat.dwLength = { sizeof(MemStat) };
    GlobalMemoryStatus(&MemStat);

    cout << "Размер страницы: " << MemStat.dwLength << endl;
    cout << "Процент использования памяти: " << MemStat.dwMemoryLoad << endl;
    cout << "Размер физической памяти в байтах: " << MemStat.dwTotalPhys << endl;
    cout << "Доступная память в байтах: " << MemStat.dwAvailPhys << endl;
    cout << "Размер файла подкачки: " << MemStat.dwTotalPageFile << endl;
    cout << "доступный размер файла подкачки: " << MemStat.dwAvailPageFile << endl;
    cout << "Вс вирутальная память: " << MemStat.dwTotalVirtual << endl;
    cout << "Доступаня виртуальная память: " << MemStat.dwAvailVirtual << endl;

}


void workWithHeap() {
    HANDLE SysHeap = GetProcessHeap(); // максимальное количество выделений памяти 

    UINT MAX_ALLOCATIONS = 32; // текущее количество выделений памяти 
    UINT NumOfAllocations; 
    
    for (NumOfAllocations = 0; NumOfAllocations < MAX_ALLOCATIONS; NumOfAllocations++) // выделение из кучи 1 Кб памяти 
        if (HeapAlloc(SysHeap, 0, 1024) == NULL) break; // вывод соответствующих сообщений в зависимости от ситуации 

    if (NumOfAllocations == 0) 
        cout << "Память из кучи не выделялась.\n"; 
    else 
        printf("Память из кучи выделялась %d раз.", NumOfAllocations); 

    cout << endl;
}


void workWithRAM() {
    SIZE_T MEMSIZE = 4096; 
    LPVOID Mem = VirtualAlloc(NULL, MEMSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 
    
    if (Mem != NULL) { 
        if (VirtualLock(Mem, MEMSIZE)) 
            printf("Привязка\n"); 
        else 
            printf("Ошибка привязки\n"); 
        
        printf("Нажмите любую клавишу, чтобы продолжить...\n"); 
        _getch(); 
        
        if (VirtualUnlock(Mem, MEMSIZE)) 
            printf("Привязка снята\n"); 
        else 
            printf("Ошибка снятия привязки\n"); 

        if (VirtualFree(Mem, 0, MEM_RELEASE)) 
            printf("Память освобождена\n"); 
        else 
            printf("Память не освобождена\n"); 
    }

    else printf("Память не выделена\n");
}

int main() {
    setlocale(LC_ALL, "Russian");

    writeSystemInfo();
    cout << endl;

    writeMemoryInfo();
    cout << endl;

    workWithHeap();
    cout << endl;

    workWithRAM();
    cout << endl;
}