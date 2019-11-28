#include <Windows.h>
#include <iostream>
#include <string>

#include "GenerateSemaphore.h"

using namespace std;

#pragma once
class TSemapfore
{
public:
	HANDLE h;
	string status = "";

	TSemapfore(string name, int x) {
		h = GenerateSemaphore::createOrOpenSemaphore(name, x, 1);
	};
	~TSemapfore() {};

	void V() {
		ReleaseSemaphore(h, 1, NULL);
		status = "open";
	}
	void P() {
		WaitForSingleObject(h, INFINITE);
		status = "close";
	}

	// thread
	void operator()() {
	}
private:

};