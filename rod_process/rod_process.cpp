#include <windows.h>
#include <iostream>

int main() {
    setlocale(0, "rus");
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    wchar_t fullPath[MAX_PATH];
    GetFullPathName(L"test.txt", MAX_PATH, fullPath, NULL);

    HANDLE hFile = CreateFile(
        fullPath, GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка создания файла: " << GetLastError() << "\n";
        return 1;
    }

    const char* data = "Hello from Parent!";
    DWORD written;
    WriteFile(hFile, data, strlen(data), &written, NULL);

    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    wchar_t cmdLine[MAX_PATH];
    swprintf(cmdLine, MAX_PATH, L"doch_process.exe %s", fullPath);

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Ошибка запуска дочернего процесса: " << GetLastError() << "\n";
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
