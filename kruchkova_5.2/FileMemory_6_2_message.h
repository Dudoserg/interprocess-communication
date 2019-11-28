#include <windows.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <atlstr.h>
#include <string>
#include <atlstr.h>
#include "Message.h"
#include "Message_Fridge.h"

#include"cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/concepts/pair_associative_container.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"

#pragma once
using namespace std;
class FileMemory_6_2_message
{
public:
	string semPutName;
	string semGetName;
	string FileMemory_6_2_messageName;

	FileMemory_6_2_message(string semPutName, string semGetName, string FileMemory_6_2_messageName) {
		this->semPutName = semPutName;
		this->semGetName = semGetName;
		this->FileMemory_6_2_messageName = FileMemory_6_2_messageName;
		init();
	}

	FileMemory_6_2_message(string name) {
		this->semPutName = name + string("_PUT");
		this->semGetName = name + string("_GET");
		this->FileMemory_6_2_messageName = name + string("_FILE");
		init();
	}

	~FileMemory_6_2_message() {
	}

	void init() {
		// ������� ��� ��������� ��������

		hSem_put = createOrOpenSemaphore(semPutName, 1);
		hSem_get = createOrOpenSemaphore(semGetName, 0);
		createFile(FileMemory_6_2_messageName);

	}


	void put(string str) {
		WaitForSingleObject(hSem_put, INFINITE);	// ���� ����� �����, ������ ���� � ����� �����

													// ����������� ������ � ������
		//std::stringstream os;
		//{
		//	cereal::JSONOutputArchive archive_out(os);
		//	archive_out(CEREAL_NVP(*message));
		//}
		//string str = os.str();

		/// ����� ������ � ����
		vector<CHAR> buffer(str.begin(), str.end());
		buffer.push_back(_T('\0'));
		CHAR* p = &buffer[0];

		SIZE_T cbData = (str.size() + 1) * sizeof(CHAR);

		//CopyMemory((PVOID)Buffer, param, (_tcslen(param) * sizeof(TCHAR)));
		CopyMemory((PVOID)Buffer, p, cbData);

		// ����������� ������� �� ������, ������ � ����� ����� ������, �.�. �� �������� � ���� ����
		ReleaseSemaphore(hSem_get, 1, NULL);
	}

	string get() {
		WaitForSingleObject(hSem_get, INFINITE);		// 

														// ������� �� ����� ������
		std::string result((char*)Buffer);

		// ������������� ������ �� ������
	/*	std::stringstream is(result);
		Message *message = new Message();
		{
			cereal::JSONInputArchive archive_in(is);
			archive_in(*message);
		}*/

		// ����������� ������� �� ������, ������ � ���� ����� ������, �.�. ���� �������
		ReleaseSemaphore(hSem_put, 1, NULL);
		return result;
	}

private:
	// fields
	HANDLE hSem_get;
	HANDLE hSem_put;
	HANDLE FileMem;
	char* Buffer;


	// methods
	wstring s2ws(const std::string& s) {
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	HANDLE createOrOpenSemaphore(string hSemName, int startStatus) {
		HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, (s2ws(hSemName)).c_str());
		if (sem == NULL) {
			cout << "������� [" << hSemName << "] �� ��������, ������� " << endl;
			sem = CreateSemaphore(
				NULL,// ��� ��������
				startStatus,// ��������� ���������
				1,// ������������ ���������
				(s2ws(hSemName)).c_str() //  �����
			);
		}
		else {
			cout << "������� [" << hSemName << "] ��������!  =) " << endl;
		}
		return sem;
	}

	void createFile(string name) {
		FileMem = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			// ��� ����� �� ����, ����� FILE_MAP_EXECUTE
			false,     //  handle  �� ����������� ��� CreateProcess
			(s2ws(name)).c_str());
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
		}
		else {
			printf("error: FILE_MAP \n"); // ��� �����!!!!
		}

		this->Buffer = buf;
	}

};

