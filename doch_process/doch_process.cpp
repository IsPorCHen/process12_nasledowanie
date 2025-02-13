#include <windows.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[]) {
    setlocale(0, "rus");
    if (argc < 2) {
        std::wcerr << L"Ошибка: не передан путь к файлу\n";
        return 1;
    }

    std::wcout << L"Открываем файл: " << argv[1] << L"\n";

    HANDLE hFile = CreateFile(
        argv[1], GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Ошибка открытия файла: " << GetLastError() << L"\n";
        return 1;
    }

    char buffer[256] = { 0 };
    DWORD read = 0;
    BOOL success = ReadFile(hFile, buffer, sizeof(buffer) - 1, &read, NULL);

    if (success && read > 0) {
        buffer[read] = '\0';
        std::wcout << L"Содержимое файла: " << buffer << L"\n";
    }
    else {
        std::wcerr << L"Ошибка чтения файла: " << GetLastError() << L"\n";
    }

    CloseHandle(hFile);
    return 0;
}
