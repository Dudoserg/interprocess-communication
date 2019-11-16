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

class A
{

public:
	void operator()()
	{
		while (true) {
			WaitForSingleObject(hSem, INFINITE);

			std::cout << "Wait" << std::endl;

			this_thread::sleep_for(std::chrono::seconds(1));

			ReleaseSemaphore(hSem, 1, NULL);
			std::cout << "Release" << std::endl;
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	hSem = CreateSemaphore(NULL, 1, 1, L"MySemaphore1");
	A p;
	thread th(p);
	if (th.joinable()) 
		th.join();

    return 0;
}

