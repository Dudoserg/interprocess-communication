// kruchkova_5.2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>

HANDLE hSem;
int a[5];
HANDLE hThr;

unsigned long uThrID;
using namespace std;

string hSemName = "MySemaphore1";

std::wstring s2ws(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

HANDLE createSem(string name) {
	HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, (s2ws(hSemName)).c_str());
	if (sem == NULL) {
		cout << "Семафор не открылся, Создаем " << endl;
		sem = CreateSemaphore(NULL, 1, 1, (s2ws(hSemName)).c_str());
	}
	else {
		cout << "Семафор открылся!  =) " << endl;
	}
	return sem;
}

struct FileMapping {
	HANDLE hFile;
	HANDLE hMapping;
	size_t fsize;
	unsigned char* dataPtr;
};


class A
{

public:
	void operator()()
	{
		while (true) {
			WaitForSingleObject(hSem, INFINITE);

			std::cout << "Close" << std::endl;


			std::cout << "Some info" << rand() << std::endl;

			this_thread::sleep_for(std::chrono::seconds(1));

			ReleaseSemaphore(hSem, 1, NULL);
			std::cout << "Release" << std::endl;
			this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");


	//hSem = createSem(hSemName);
	/*A p;
	thread th(p);
	if (th.joinable())
		th.join();*/
	

	const TCHAR szCounterFileName[] = L"data.txt";
	LPCWSTR fname = szCounterFileName;

	HANDLE hFile = CreateFile(szCounterFileName, GENERIC_READ | GENERIC_WRITE, 0, nullptr,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cerr << "fileMappingCreate - CreateFile failed, fname = "
			<< fname << std::endl;
		return NULL;
	}

	DWORD dwFileSize = GetFileSize(hFile, nullptr);
	if (dwFileSize == INVALID_FILE_SIZE) {
		std::cerr << "fileMappingCreate - GetFileSize failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return NULL;
	}

	
	//HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
	HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READWRITE,
		0, 0, nullptr);
	if (hMapping == nullptr) {
		cout << "ХУЙ ТЕБЕ" << endl;
		int x = -1;
		//cin >> x;
		std::cerr << "fileMappingCreate - CreateFileMapping failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return NULL;
	}
	else {
		cout << "КРАСАВА" << endl;
		int x = -1;
		//cin >> x;
	}

	unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		dwFileSize);
	if (dataPtr == nullptr) {
		std::cerr << "fileMappingCreate - MapViewOfFile failed, fname = "
			<< fname << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return NULL;
	}

	FileMapping* mapping = (FileMapping*)malloc(sizeof(FileMapping));
	if (mapping == nullptr) {
		std::cerr << "fileMappingCreate - malloc failed, fname = "
			<< fname << std::endl;
		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return NULL;
	}

	mapping->hFile = hFile;
	mapping->hMapping = hMapping;
	mapping->dataPtr = dataPtr;
	mapping->fsize = (size_t)dwFileSize;



	int number = mapping->dataPtr[0] | mapping->dataPtr[1] << 8;
	
	for (int i = 2; i < number * 2; i += 2) {
		int number = mapping->dataPtr[i] | mapping->dataPtr[i+1] << 8;
		cout << (char)number;
	}
	cout << endl;


	byte firstByte = 30 & 0xFF;
	byte secondByte = (30 >> 8) & 0xFF;

	TCHAR szMsg[] = TEXT(" Message from first process.");
	szMsg[0] = 569;


	CopyMemory((PVOID)mapping->dataPtr, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));
	



	return 0;
}

