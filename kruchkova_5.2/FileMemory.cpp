#include "stdafx.h"
#include "FileMemory.h"
#include <atlstr.h>

FileMemory::FileMemory()
{
	init();
}


FileMemory::~FileMemory()
{
}

void FileMemory::init()
{
	// ������� ��� ��������� ��������

	hSem_put = createOrOpenSemaphore("SemPut_1", 1);
	hSem_get = createOrOpenSemaphore("SemGet_1", 1);
	createFile("File_1");
}

void FileMemory::put(string str)
{
	WaitForSingleObject(hSem_put, INFINITE);	// ���� ����� �����, ������ ���� � ����� �����

	/// ���� ������,


	vector<CHAR> buffer(str.begin(), str.end());
	buffer.push_back(_T('\0'));
	CHAR* p = &buffer[0];

	SIZE_T cbData = (str.size() + 1) * sizeof(CHAR);

	//CopyMemory((PVOID)Buffer, param, (_tcslen(param) * sizeof(TCHAR)));
	CopyMemory((PVOID)Buffer, p, cbData);


	ReleaseSemaphore(hSem_get, 1, NULL);		// ����������� ������� �� ������, ������ � ����� ����� ������, �.�. �� �������� � ���� ����
}

// hSem_get ������ ���� ���������� 0, ����� �� �� ������ ������� ������ � �����
// hSem_put ������ ���� ���������� 1, ����� �� ������ ���� ���-������ ��������

string FileMemory::get()
{
	WaitForSingleObject(hSem_get, INFINITE);		// 

	// ������� �� ����� ������
	std::string result((char*)Buffer);


	
	ReleaseSemaphore(hSem_put, 1, NULL);			// 
	return result;

}

// ��������� ������ � ������ ������ ��� �������� ��������
wstring FileMemory::s2ws(const std::string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// ������� ��������� ������� � �������� ������
HANDLE FileMemory::createOrOpenSemaphore(string hSemName, int startStatus)
{
	HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, (s2ws(hSemName)).c_str());
	if (sem == NULL) {
		cout << "������� �� ��������, ������� " << endl;
		sem = CreateSemaphore(
			NULL,// ��� ��������
			startStatus,// ��������� ���������
			1,// ������������ ���������
			(s2ws(hSemName)).c_str() //  �����
		);
	}
	else {
		cout << "������� ��������!  =) " << endl;
	}
	return sem;
}

void FileMemory::createFile(string name)
{
	FileMem = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		// ��� ����� �� ����, ����� FILE_MAP_EXECUTE
		false,     //  handle  �� ����������� ��� CreateProcess
		(s2ws(name)).c_str() );
	if (FileMem == NULL) {
		FileMem = CreateFileMapping(
			(HANDLE)0xFFFFFFFF,
			//   INVALID_HANDLE_VALUE --- ������� �����
			NULL,  // LPSECURITY_ATTRIBUTES 
			PAGE_READWRITE,    //  ��� ������� � ������
			0, 4096,   // ������
			(s2ws(name)).c_str());
	}

	if (FileMem == NULL)
		cout << "error: FileMem == NULL \n" << endl;

	char* buf = NULL;
	//  ����� ��� ������  - ������ ������
	if (FileMem != NULL) {
		buf = (char*)MapViewOfFile(
			FileMem,   // Handle �����
			FILE_MAP_ALL_ACCESS,
			0, 0,  // ��������
			4096);   // ����� ������
	} else {
		printf("error: FILE_MAP \n"); // ��� �����!!!!
	}
	
	this->Buffer = buf;
}
