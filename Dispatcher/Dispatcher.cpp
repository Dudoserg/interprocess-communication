// Dispatcher.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include "FileMemory.h"
#include "PersonType.h"
#include "Person.h"

class A : Person
{

public:
	FileMemory * toBuyer;
	FileMemory * toCook;
	FileMemory * toDispatcher;

	

	A() {
		toDispatcher = new FileMemory("toDispatcher_Put", "toDispatcher_Get", "toDispatcher_File");
		toBuyer = new FileMemory("toBuyer_Put", "toBuyer_Get", "toBuyer_File");
		toCook = new FileMemory("toCook_Put", "toCook_Get", "toCook_File");
		personName = "Dispatcher282";
		personType = PersonType::Enum::DISPATCHER;
	};

	~A() {};

	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;
		sleep(2000);
		while (true) {
			cout << "Начинает ожидать сообщения от кого-либо..." << endl;
			Message *messageToDispatcher = toDispatcher->getMessage();
			sleep(2000);

			cout << "Получено сообщение [" << messageToDispatcher->message << "] от [" << messageToDispatcher->personName << "] " << endl;


			cout << "Готовим ответ..." << endl;
			sleep(2000);

			Message *messageToBuyer = new Message();
			messageToBuyer->message = "feedback";
			messageToBuyer->personName = personName;
			messageToBuyer->personType = personType;

			cout << "Посылаем ответ..." << endl;
			sleep(2000);
			toBuyer->putMessage(messageToBuyer);
		}
	}
private:;
		void sleep(int millis) {
			this_thread::sleep_for(std::chrono::milliseconds(millis));
		}
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


