// ���� ������������ �� ������, ������ ��� ������ ������� ���������� �� ������ Message_6_2
// ��� ��������� �������� ��������


#include <windows.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <atlstr.h>
#include <string>
#include <atlstr.h>
#include "Message_6_2.h"


#include"cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/concepts/pair_associative_container.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"

#pragma once
using namespace std;
class FileMemory_6_2
{
public:

	// fields
	HANDLE hSem_get;
	HANDLE hSem_put;

	HANDLE hSem_change;
	HANDLE FileMem;
	char* Buffer;

	string semPutName;
	string semGetName;
	string semChange;
	string fileMemoryName;

	FileMemory_6_2(string semPutName, string semGetName, string fileMemoryName) {
		this->semPutName = semPutName;
		this->semGetName = semGetName;
		this->fileMemoryName = fileMemoryName;
		init();
	}

	FileMemory_6_2(string name) {
		this->semPutName = name + string("_PUT");
		this->semGetName = name + string("_GET");
		this->semChange = name + string("_CHANGE");
		this->fileMemoryName = name + string("_FILE");
		init();
	}

	~FileMemory_6_2() {}

	int getNumR() {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
		return message->numR;
	}
	void setNumr(int value) {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		message->numR = value;
		this->putMessage(message);
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
	}

	int getNumW() {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
		return message->numW;
	}
	void setNumW(int value) {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		message->numW = value;
		this->putMessage(message);
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
	}

	int getWaitR() {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
		return message->waitR;
	}
	void setWaitR(int value) {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		message->waitR = value;
		this->putMessage(message);
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
	}

	int getWaitW() {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
		return message->waitW;
	}
	void setWaitW(int value) {
		WaitForSingleObject(hSem_change, INFINITE);
		//
		Message_6_2 *message = this->getMessage();
		message->waitW = value;
		this->putMessage(message);
		//
		ReleaseSemaphore(hSem_change, 1, NULL);
	}

private:
	void init() {
		// ������� ��� ��������� ��������

		int maxState = 1;
		int startState = maxState;
		hSem_put = createOrOpenSemaphore(semPutName, startState, maxState);
		hSem_get = createOrOpenSemaphore(semGetName, 0, maxState);

		hSem_change = createOrOpenSemaphore(semChange, 1, 1);

		bool opened = openOrcreateFile(fileMemoryName);
		if (opened == false) {
			//������� ���� ������ ��� �������� ���������
			Message_6_2 * message_6_2 = new Message_6_2();
			// ����������� � ������
			std::stringstream os;
			{
				cereal::JSONOutputArchive archive_out(os);
				archive_out(CEREAL_NVP(*message_6_2));
			}
			string message_arr_string_afterPush = os.str();
			// ����� ������ � ����
			vector<CHAR> buffer(message_arr_string_afterPush.begin(), message_arr_string_afterPush.end());
			buffer.push_back(_T('\0'));
			CHAR* p = &buffer[0];

			SIZE_T cbData = (message_arr_string_afterPush.size() + 1) * sizeof(CHAR);
			CopyMemory((PVOID)Buffer, p, cbData);
		}
	}

	void putMessage(Message_6_2 *message) {
		//WaitForSingleObject(hSem_put, INFINITE);	// ���� ����� �����, ������ ���� � ����� �����

													// ����������� ������ � ������
		std::stringstream os;
		{
			cereal::JSONOutputArchive archive_out(os);
			archive_out(CEREAL_NVP(*message));
		}
		string str = os.str();

		/// ����� ������ � ����
		vector<CHAR> buffer(str.begin(), str.end());
		buffer.push_back(_T('\0'));
		CHAR* p = &buffer[0];

		SIZE_T cbData = (str.size() + 1) * sizeof(CHAR);

		//CopyMemory((PVOID)Buffer, param, (_tcslen(param) * sizeof(TCHAR)));
		CopyMemory((PVOID)Buffer, p, cbData);

		// ����������� ������� �� ������, ������ � ����� ����� ������, �.�. �� �������� � ���� ����
		//ReleaseSemaphore(hSem_get, 1, NULL);
	}

	Message_6_2 *getMessage() {
		//WaitForSingleObject(hSem_get, INFINITE);		// 

														// ������� �� ����� ������
		std::string result((char*)Buffer);

		// ������������� ������ �� ������
		std::stringstream is(result);
		//cout << result << endl;
		Message_6_2 *message = new Message_6_2();
		{
			cereal::JSONInputArchive archive_in(is);
			archive_in(*message);
		}

		// ����������� ������� �� ������, ������ � ���� ����� ������, �.�. ���� �������
		//ReleaseSemaphore(hSem_put, 1, NULL);
		return message;
	}

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

	HANDLE createOrOpenSemaphore(string hSemName, int startStatus, int MaxState) {
		HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, (s2ws(hSemName)).c_str());
		if (sem == NULL) {
			cout << "������� [" << hSemName << "] �� ��������, ������� " << endl;
			sem = CreateSemaphore(
				NULL,// ��� ��������
				startStatus,// ��������� ���������
				MaxState,// ������������ ���������
				(s2ws(hSemName)).c_str() //  �����
			);
		}
		else {
			cout << "������� [" << hSemName << "] ��������!  =) " << endl;
		}
		return sem;
	}

	bool openOrcreateFile(string name) {
		FileMem = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			// ��� ����� �� ����, ����� FILE_MAP_EXECUTE
			false,     //  handle  �� ����������� ��� CreateProcess
			(s2ws(name)).c_str());
		bool open = true;

		if (FileMem == NULL) {
			open = false;
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
		return open;
	}

};

