#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <atlstr.h>
#include <string>

#pragma once
using namespace std;
class FileMemory
{
public:
	FileMemory();
	~FileMemory();

	void init();

	void put(string str);

	string get();
	

	// fields


	HANDLE hSem_get;	// hSem_get ������ ���� ���������� 0, ����� �� �� ������ ������� ������ � �����
	HANDLE hSem_put;	// hSem_put ������ ���� ���������� 1, ����� �� ������ ���� ���-������ ��������
	HANDLE FileMem;

	char* Buffer;

private:
	wstring s2ws(const std::string& s);
	HANDLE createOrOpenSemaphore(string name, int startStatus);

	void createFile(string name);
};

