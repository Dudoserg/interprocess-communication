// kruchkova_5.2(2).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include "FileMemory.h"


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

HANDLE createOrOpenSemaphore(string name) {

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
	
	string str;;
	
	A() {
		
	}

	A* operator<<(int x) {
		str += std::to_string(x);

		return this;
	}
	A* operator<<(string x) {
		str += x;
		
		return this;
	}

	
	void operator()()
	{
		HANDLE hMapping = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,
			// все права на файл, кроме FILE_MAP_EXECUTE
			false,     //  handle  не наследуется при CreateProcess
			L"MY_NAME");
		if (hMapping == NULL)
			hMapping = CreateFileMapping(
			(HANDLE)0xFFFFFFFF,
				//   INVALID_HANDLE_VALUE --- СОЗДАЕМ НОВЫЙ
				NULL,  // LPSECURITY_ATTRIBUTES
				PAGE_READWRITE,    //  вид доступа к данным
				0, 4096,   // размер
				L"MY_NAME");
		if (hMapping == NULL)
			cout << " (" << endl;


		unsigned char* Buffer = NULL;
		//  Буфер для записи  - чтения данных
		if (hMapping != NULL)
			Buffer = (unsigned char*)MapViewOfFile(
				hMapping,   // Handle файла
				FILE_MAP_ALL_ACCESS,
				0, 0,  // смещение
				4096);   // длина данных
		else {
			printf("error: FILE_MAP \n"); // Все плохо!!!!
		}

		while (true) {
			WaitForSingleObject(hSem, INFINITE);
			std::cout << "Close" << std::endl;

			int number = Buffer[0] | Buffer[1] << 8;

			ReleaseSemaphore(hSem, 1, NULL);

			cout << "readen num = " << number << "\t";
			std::cout << "Release" << std::endl;

			this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
};

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");

	A *test = new A();
	test = *(*test << 4) << 8;
	
	cout << test->str << endl;
	

	

	///*hSem = createOrOpenSemaphore(hSemName);

	//A p;
	//thread th(p);
	//if (th.joinable())
	//	th.join();*/

	//FileMemory *myFileMemoory = new FileMemory();
	///*myFileMemoory->put("hello привет 69");
	//cout << myFileMemoory->get() << endl;*/

	//cout << "Я буду Писать сообщения в память" << endl;
	//while (true) {
	//	cout << "Введите строку для передачи" << endl;
	//	string str;
	//	std::getline(std::cin, str);
	//	myFileMemoory->put(str);
	//	cout << endl;
	//}

	return 0;
}

