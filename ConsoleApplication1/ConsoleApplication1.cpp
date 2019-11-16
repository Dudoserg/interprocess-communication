// kruchkova_5.2(2).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
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


int main()
{
	setlocale(LC_ALL, "Russian");

	hSem = createSem(hSemName);

	int num = -1;

	while (true) {
		cout << "Заблочить семафор? (0) Отпустить семафор?(1)" << endl;
		cin >> num;
		if (num == 0) {
			WaitForSingleObject(hSem, INFINITE);
			cout << "Заблокирован" << endl;
		}
		else if (num == 1) {
			ReleaseSemaphore(hSem, 1, NULL);
			cout << "Отпущен" << endl;
		}
		else {
			cout << "Ты че ввел, паскуда" << endl;
		}
	}

	return 0;
}

