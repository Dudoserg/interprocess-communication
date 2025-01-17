// Dispatcher.cpp: определяет точку входа для консольного приложения.
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
#include <chrono>

using namespace std::chrono;

class A : Person
{

public:

	const int TIME_COEFF = 1000;

	FileMemory * toBuyer;
	FileMemory * dispatcherToCook;
	FileMemory * toDispatcher;
	int money;

	A() {
		personName = "Dispatcher282";
		personType = PersonType::Enum::DISPATCHER;

		toDispatcher = new FileMemory("toDispatcher");
		toBuyer = new FileMemory("toBuyer");
		dispatcherToCook = new FileMemory("dispatcherToCook");
		money = 0;
	};

	~A() {};



	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;
		
		sleep(2 * TIME_COEFF);
		while (true) {

			cout << "Начинаю ожидать сообщения от Кого-либо" << endl;
			sleep(2 * TIME_COEFF);
			Message *messageBaseTaked = toDispatcher->getMessage();
			
			switch (messageBaseTaked->personType) {
				case PersonType::Enum::BUYER : {
					cout << "Получено сообщение от покупателя [ " << messageBaseTaked->personName << " ]" << endl;
					sleep(2 * TIME_COEFF);
					printWithDelay("Обрабатываю заказ", 5 * TIME_COEFF, TIME_COEFF);
					dispatcherToCook->putMessage(new Message(*this, messageBaseTaked->message));
					cout << "Заказ обработан, передан повару" << endl;
					sleep(2 * TIME_COEFF);
					break;
				}
				case PersonType::Enum::COOK: {
					cout << "Получено сообщение от Повара [ " << messageBaseTaked->personName << " ]" << endl;
					sleep(2 * TIME_COEFF);
					printWithDelay("Звоним покупателю", getRandomNumber(1 * TIME_COEFF, 3 * TIME_COEFF), TIME_COEFF);
					toBuyer->putMessage(new Message(*this, messageBaseTaked->message));
					cout << "Покупатель осведомлен" << endl;
					sleep(2 * TIME_COEFF);
					break;
				}
				case PersonType::Enum::COURIER : {
					cout << "Получено сообщение от Курьера [ " << messageBaseTaked->personName << " ]" << endl;
					sleep(2 * TIME_COEFF);
					money += stoi(messageBaseTaked->message);
					cout << "Казна пополнена: +" << messageBaseTaked->message << " Всего в казне: " << money << endl;
					sleep(2 * TIME_COEFF);
					break;
				}
				default:{
					cout << "switch error";
					break;
				}
			}

			//delete messageBaseTaked;

			/*cout << "Получено сообщение [" << messageToDispatcher->message << "] от [" << messageToDispatcher->personName << "] " << endl;
			sleep(2 * TIME_COEFF);
			cout << "Готовим ответ..." << endl;
			sleep(2 * TIME_COEFF);
			Message *messageToBuyer = new Message(*this, "feedback");
			cout << "Посылаем ответ..." << endl;
			sleep(2 * TIME_COEFF);
			toBuyer->putMessage(messageToBuyer);*/
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


