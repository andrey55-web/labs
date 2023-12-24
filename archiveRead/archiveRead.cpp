#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

#pragma pack(push,1)

const BYTE RarSignature[] = { 0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00 };

typedef struct {
	WORD header_crc;
	BYTE header_type;
	WORD header_flags;
	WORD header_size;
}VolumeHeader;

typedef struct {
	DWORD PackSize;
	DWORD UnpSize;
	BYTE HostOS;
	DWORD FileCRC;
	DWORD FileTime;
	BYTE UnpVer;
	BYTE Method;
	WORD NameSize;
	DWORD FileAttr;
}FileHead;

#pragma pack(pop)

void readArchive(vector<BYTE>& buffer, DWORD fileSize) {
	int numFiles = 0;
	DWORD fileOffset = sizeof(RarSignature);
	cout << "Файлы: " << endl;
	while (fileOffset < fileSize) {
		VolumeHeader vh;
		FileHead fh;
		memcpy(&vh, &buffer[fileOffset], sizeof(vh));
		if (vh.header_type == 0x74) {
			numFiles++;
			int ind = fileOffset;
			fileOffset += sizeof(vh);
			memcpy(&fh, &buffer[fileOffset], sizeof(fh));
			fileOffset += sizeof(fh);
			BYTE* filename = new BYTE[fh.NameSize];
			for (int i = 0; i < fh.NameSize; i++) {
				filename[i] = buffer[fileOffset];
				cout << filename[i];
				fileOffset++;
			}
			cout << endl;
			delete[] filename;
			fileOffset = ind;
			fileOffset += fh.PackSize + vh.header_size;
		}
		else {
			fileOffset += vh.header_size;
		}
		
	}
	cout << "Кол-во файлов в архиве: " << numFiles << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	HANDLE fileHandle = CreateFile(L"Example.rar", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE) {
		cout << "Ошибка открытия файла!" << endl;
		CloseHandle(fileHandle);
		return -1;
	}
	DWORD fileSize = GetFileSize(fileHandle, NULL);
	vector<BYTE> buffer(fileSize);
	DWORD bytesRead;
	bool readOK = ReadFile(fileHandle, buffer.data(), fileSize, &bytesRead, NULL);
	if (!readOK) {
		cout << "Ошибка чтения файла!" << endl;
		CloseHandle(fileHandle);
		cout << GetLastError();
		return -1;
	}
	CloseHandle(fileHandle);
	readArchive(buffer,fileSize);
	cout << "Размер архива: " << fileSize << " байт";
	return 0;
}
