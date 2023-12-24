#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    PCWSTR filename = L"file.rar";
    HANDLE handleFile = CreateFile(filename, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handleFile == INVALID_HANDLE_VALUE) {
        wcout << L'Ошибка создания файла' << endl;
        return 1;
    }
    LARGE_INTEGER fileOffset;
    fileOffset.QuadPart = 0;
    auto hp = fileOffset.HighPart;
    auto* ptr_hp = &hp;
    DWORD currentPosition = SetFilePointer(handleFile, fileOffset.LowPart, ptr_hp, FILE_BEGIN);
    if (currentPosition == fileOffset.LowPart and GetLastError() == NO_ERROR) {
        DWORD fileSize = GetFileSize(handleFile, NULL);
        vector<unsigned char> dataBuffer(fileSize);
        DWORD bytesRead;
        bool readOK = ReadFile(handleFile, &dataBuffer, fileSize, &bytesRead, NULL);
        if (readOK) {
            cout << &dataBuffer << endl;
        }
    }
    else {
        wcout << L'Ошибка' << endl;
        return 1;
    }
    CloseHandle(handleFile);
}