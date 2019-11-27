#include <windows.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <atlstr.h>
#include <string>
#include <atlstr.h>
#include "Message_6.h"

#include "Message_arr_6.h"

#include"cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/concepts/pair_associative_container.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"

#pragma once
using namespace std;
class FileMemory_6
{
public:

	// fields
	HANDLE hSem_get;
	HANDLE hSem_put;
	HANDLE hSem_write;
	HANDLE FileMem;
	char* Buffer;

	string semPutName;
	string semGetName;
	string semWriteName;
	string fileMemoryName;

	FileMemory_6(string semPutName, string semGetName, string fileMemoryName) {
		this->semPutName = semPutName;
		this->semGetName = semGetName;
		this->fileMemoryName = fileMemoryName;
		init();
	}

	FileMemory_6(string name) {
		this->semPutName = name + string("_PUT");
		this->semGetName = name + string("_GET");
		this->semWriteName = name + string("_WRITE");
		this->fileMemoryName = name + string("_FILE");
		init();
	}

	~FileMemory_6() {
	}

	void init() {
		// ������� ��� ��������� ��������

		int maxState = 8;
		int startState = maxState;
		hSem_put = createOrOpenSemaphore(semPutName, startState, maxState);
		hSem_get = createOrOpenSemaphore(semGetName, 0, maxState);

		hSem_write = createOrOpenSemaphore(semWriteName, 1, 1);

		bool opened = openOrcreateFile(fileMemoryName);
		if (opened == false) {
			//������� ���� ������ ��� �������� ���������
			Message_arr_6 * message_arr_6 = new Message_arr_6();
			// ����������� � ������
			std::stringstream os;
			{
				cereal::JSONOutputArchive archive_out(os);
				archive_out(CEREAL_NVP(*message_arr_6));
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


	void putMessage_6(Message_6 *currentMessage) {
		WaitForSingleObject(hSem_put, INFINITE);	// ���� ����� �����, ������ ���� � ����� �����
													
		
		WaitForSingleObject(this->hSem_write, INFINITE);	// ������ ��� ������ � �����
															// �.�. � ����� ����� ������ >1, 
															// � � ���� ������������ ����� ������ ��������� ���������
		// ����� ������ �� �����
		std::string message_arr_string((char*)Buffer);
		cout << message_arr_string << endl;
		// ������������� �� ������ ������ ������ ���������
		std::stringstream is(message_arr_string);
		Message_arr_6 *message_arr_6 = new Message_arr_6();
		{
			cereal::JSONInputArchive archive_in(is);
			archive_in(*message_arr_6);
		}
		// ��������� ���� ������� ���������
		message_arr_6->data->push_back(std::unique_ptr<Message_6>(currentMessage));
		// ����������� � ������
		std::stringstream os;
		{
			cereal::JSONOutputArchive archive_out(os);
			archive_out(CEREAL_NVP(*message_arr_6));
		}
		string message_arr_string_afterPush = os.str();
		cout << message_arr_string_afterPush << endl;
		// ����� ������ � ����
		vector<CHAR> buffer(message_arr_string_afterPush.begin(), message_arr_string_afterPush.end());
		buffer.push_back(_T('\0'));
		CHAR* p = &buffer[0];

		SIZE_T cbData = (message_arr_string_afterPush.size() + 1) * sizeof(CHAR);
		CopyMemory((PVOID)Buffer, p, cbData);

																	
		ReleaseSemaphore(this->hSem_write, 1, NULL);					// ��������� ������� �� ������ � ����


		// ����������� ������� �� ������, ������ � ����� ����� ������, �.�. �� �������� � ���� ����
		ReleaseSemaphore(hSem_get, 1, NULL);
	}

	Message_6 *getMessage_6(int id) {
		WaitForSingleObject(hSem_get, INFINITE);		// 

		WaitForSingleObject(this->hSem_write, INFINITE);	// ������ ��� ������ � �����
															// �.�. � ����� ����� ������ >1, 
															// � � ���� ������������ ����� ������ ��������� ���������
		// ������� ������ �� �����
		std::string result((char*)Buffer);
		cout << result << endl;
		// �������������
		std::stringstream is(result);
		Message_arr_6 *message_arr_6 = new Message_arr_6();
		{
			cereal::JSONInputArchive archive_in(is);
			archive_in(*message_arr_6);
		}
		// ���������, ���� �� ��������� ��� ������ id.
		bool foundMessage = false;
		int index = -1;
		for (int i = 0; i < message_arr_6->data->size(); i++) {
			if ((*message_arr_6->data)[i]->idToUser == id) {
				foundMessage = true;
				index = i;
			}
		}
		Message_6 * message = NULL;
		if (foundMessage == true) {
			//		���� ����, �� :	
			//						������� ���������
			message = (*message_arr_6->data)[index].release();
			(*message_arr_6->data).erase((*message_arr_6->data).begin() + index);
			//						����������� ������ ��� ������ ���������
			std::stringstream os;
			{
				cereal::JSONOutputArchive archive_out(os);
				archive_out(CEREAL_NVP(*message_arr_6));
			}
			string message_arr_string_afterGET = os.str();
			cout << message_arr_string_afterGET << endl;
			//						����� ������ � ����
			vector<CHAR> buffer(message_arr_string_afterGET.begin(), message_arr_string_afterGET.end());
			buffer.push_back(_T('\0'));
			CHAR* p = &buffer[0];

			SIZE_T cbData = (message_arr_string_afterGET.size() + 1) * sizeof(CHAR);
			CopyMemory((PVOID)Buffer, p, cbData);
			//						����������� ������� �� ������, ������ � ���� ����� ������, �.�. �� �
			//							������� � ���� ����
									ReleaseSemaphore(hSem_put, 1, NULL);
		}
		else {
			//		���� ���, �� :
			//						����������� ������� �� ������, ������ � ����� ����� ������, �.�. �� 
			//							�� ������ � ���� ����
									ReleaseSemaphore(hSem_get, 1, NULL);
		}
		ReleaseSemaphore(this->hSem_write, 1, NULL);					// ��������� ������� �� ������ � ����

		return message;
	}

private:



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

