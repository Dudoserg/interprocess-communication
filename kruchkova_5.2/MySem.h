#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

#pragma once

using namespace std;

class MySem
{
public:
	MySem();
	~MySem();

	static HANDLE createOrOpenSemaphore(string hSemName, int startStatus) {
		HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, (s2ws(hSemName)).c_str());
		if (sem == NULL) {
			cout << "Семафор [" << hSemName << "] не открылся, Создаем " << endl;
			sem = CreateSemaphore(
				NULL,// нет атрибута
				startStatus,// начальное состояние
				1,// максимальное состояние
				(s2ws(hSemName)).c_str() //  имени
			);
		}
		else {
			cout << "Семафор [" << hSemName << "] открылся!  =) " << endl;
		}
		return sem;
	}
private:
	// methods
	static wstring s2ws(const std::string& s) {
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	
};

