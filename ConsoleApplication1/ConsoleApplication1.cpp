// kruchkova_5.2(2).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
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

int main()
{
	setlocale(LC_ALL, "Russian");
	HANDLE hSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, true, L"MySemaphore1");
	if (hSem == NULL) {
		cout << "Семафор не открылся, отъебнуло -__-" << endl;
	}
	else {
		cout << "Семафор открылся!  =) " << endl;
	}



	int num = -1;

	while (true) {
		cout << "Заблочить семафор? (0) Отпустить семафор?(1)" << endl;
		cin >> num;
		if (num == 0) {
			WaitForSingleObject(hSem, INFINITE);
			cout << endl;
		}
		else if (num == 1) {
			ReleaseSemaphore(hSem, 1, NULL);
			cout << endl;
		}
		else {
			cout << "Ты че ввел, паскуда" << endl;
		}
	}

	return 0;
}

