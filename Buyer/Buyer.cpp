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
#include "Person.h"

class A : Person
{

public:
	FileMemory * toBuyer;
	FileMemory * toCourier;
	FileMemory * toDispatcher;


	A() {
		toBuyer = new FileMemory("toBuyer_Put", "toBuyer_Get", "toBuyer_File");
		toCourier = new FileMemory("toCourier_Put", "toCourier_Get", "toCourier_File");
		toDispatcher = new FileMemory("toDispatcher_Put", "toDispatcher_Get", "toDispatcher_File");
		personName = "Buyer228";
		personType = PersonType::Enum::BUYER;
	};

	~A() {};

	// thread
	void operator()() {
		int count = 0;
		cout << personName << "(" << PersonType::toString(personType) <<") начал работу" << endl;
		sleep(2000);
		while (true) {
			string order = "pizza";
			Message messageToDispatcher;
			messageToDispatcher.message = std::string(order);
			messageToDispatcher.personName = personName;
			messageToDispatcher.personType = personType;
			
			cout << "Посылает сообщение диспетчеру, Текст сообщения : " << messageToDispatcher.message << endl;
			sleep(2000);
			toDispatcher->putMessage(messageToDispatcher);


			cout << "Начинает ожидать ответа от Диспетчера или курьера..." << endl;
			sleep(2000);

			Message messageToBuyer = toBuyer->getMessage();
			cout << "Получено сообщение [" << messageToBuyer.message << "] от [" << messageToBuyer.personName << "] " << endl;
			sleep(2000);
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

