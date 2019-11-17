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


	HANDLE hSem_get;	// hSem_get должен быть изначально 0, чтобы мы не смогли считать данный с файла
	HANDLE hSem_put;	// hSem_put должен быть изначально 1, чтобы мы смогли туда что-нибудь записать
	HANDLE FileMem;

	char* Buffer;

private:
	wstring s2ws(const std::string& s);
	HANDLE createOrOpenSemaphore(string name, int startStatus);

	void createFile(string name);
};

