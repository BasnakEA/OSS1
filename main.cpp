#include <iostream>
#define _WIN32_WINNT 0x0510
#include <windows.h>
#include <cmath>
#include <Lmcons.h>
#include <string.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Точный замер времени (начало)
    printf("Точные замеры времени\n\n");
    LARGE_INTEGER t0, t1, frequency;
    QueryPerformanceCounter(&t0);
    double result = sqrt(t0.QuadPart);
    QueryPerformanceCounter(&t1);
    QueryPerformanceFrequency(&frequency);

    double ticks = t1.QuadPart - t0.QuadPart;
    double ticks_per_sec = frequency.QuadPart;
    double usec_per_sec = 1e6;
    double usec = usec_per_sec * ticks / ticks_per_sec;
    printf("result = %g, duration = %.1f usec\n", result, usec);

    // Точный замер времени (конец)


    // Получение версии Windows (начало)
    printf("\nПолучение версии Windows\n\n");

    DWORD info = GetVersion();
    DWORD mask = 0x0000'ffff;
    DWORD version = info & mask;
    DWORD platform = info >> 16;

    DWORD major_mask = 0x00'ff;
    DWORD version_major = version & major_mask;
    DWORD version_minor = version >> 8;

    if ((info & 0x8000'0000) == 0) {
        DWORD build = platform >> 8;
        printf("Windows v%d.%d (build %d)\n", version_major, version_minor, build);
    }
    // Получение версии Windows (конец)


    // Получение текстовых данных (начало)
    printf("\nПолучение текстовых данных\n\n");

    char system_dir[MAX_PATH];
    if (GetSystemDirectoryA(system_dir, MAX_PATH))
        printf("System directory: %s\n", system_dir);
    else
        printf("Ошибка SysDir: %u\n", GetLastError());

    char UserName[UNLEN+1];
    DWORD UserNameLen = UNLEN + 1;
    if (GetUserNameA(UserName, &UserNameLen))
        printf("Username: %s\n", UserName);
    else
        printf("Ошибка UserName: %u\n", GetLastError());

    char CompName[MAX_COMPUTERNAME_LENGTH+1];
    DWORD CompNameLen = MAX_COMPUTERNAME_LENGTH + 1;
    if (GetComputerNameA(CompName, &CompNameLen))
        printf("Computer name: %s\n", CompName);
    else
        printf("Ошибка CompName: %u\n", GetLastError());
    // Получение текстовых данных (конец)


    // Получение информации о дисках + Получение пути к тому в файловой системе + Получение информации о свободном месте на томе (начало)
    printf("\nПолучение информации о дисках + Получение пути к тому в файловой системе\n\n");

    char buffer[MAX_PATH];

    HANDLE search = FindFirstVolumeA(buffer, sizeof(buffer));
    do {
        printf("Volume: %s\n", buffer);

        char path_buffer[MAX_PATH * 4];
        PDWORD retlen = 0;
        GetVolumePathNamesForVolumeNameA(buffer, path_buffer, MAX_PATH*4, retlen);
        char* path = path_buffer;
        int i = 1;
        while (path[0])
        {
            printf("Mount point %d: %s\n", i, path);
            path += strlen(path) + 1;
            i++;
        }

        // Получение информации о свободном месте на томе (начало)
        printf("\nИнформация о свободном месте на томе\n\n");

        ULARGE_INTEGER FreeBytes, TotalBytes, TotalFreeBytes;
        if (GetDiskFreeSpaceExA(buffer, &FreeBytes, &TotalBytes, &TotalFreeBytes))
        {
            printf("Total number of MB on a disk: %u MB\n", TotalBytes.QuadPart / (1024*1024));
            printf("Total number of free MB: %u MB\n", TotalFreeBytes.QuadPart / (1024*1024));
            printf("Total number of free MB available to the caller: %u MB\n", FreeBytes.QuadPart / (1024*1024));
        }
        else
        {
            printf("GetDiskFreeSpaceExA Error: %u\n", GetLastError());
        }
        // Получение информации о свободном месте на томе (конец)

        printf("\n");
    } while (FindNextVolumeA(search, buffer, sizeof(buffer)));

    if (GetLastError() != ERROR_NO_MORE_FILES) {
        printf("Произошла ошибка %u\n", GetLastError());
    }

    FindVolumeClose(search);
    // Получение информации о дисках + Получение пути к тому в файловой системе + Получение информации о свободном месте на томе (конец)

    return 0;
}
