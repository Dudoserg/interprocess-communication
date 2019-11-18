// Buyer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include "FileMemory.h"
#include "PersonType.h"

class A
{

public:
	FileMemory * toBuyer;
	FileMemory * toCourier;
	FileMemory * toDispatcher;

	A() {
		toBuyer = new FileMemory("toBuyer_Put", "toBuyer_Get", "toBuyer_File");
		toBuyer = new FileMemory("toCourier_Put", "toCourier_Get", "toCourier_File");
		toBuyer = new FileMemory("toDispatcher_Put", "toDispatcher_Get", "toDispatcher_File");
	};

	~A() {};

	// thread
	void operator()() {
		while (true) {
			string order = "pizza";
			string message = std::string(order);
			
			cout << "Посылает сообщение диспетчеру, Текст сообщения : " << message << endl;
			toDispatcher->put(order);

			cout << "Начинает ожидать ответа от Диспетчера или курьера..." << endl;

			string messageToBuyer = toBuyer->get();
			cout << "Получен ответ от " << 

			break;
		}
	}
private:;
		
};


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	A p;
	thread th(p);
	if (th.joinable())
		th.join();
	return 0;
}

