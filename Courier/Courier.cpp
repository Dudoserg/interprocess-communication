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

	FileMemory * toDispatcher;
	FileMemory * toBuyer;
	FileMemory * toCourier;
	
	
	int money;

	A() {
		personName = "COURIER";
		personType = PersonType::Enum::COURIER;

		toDispatcher = new FileMemory("toDispatcher");
		toBuyer = new FileMemory("toBuyer");
		toCourier = new FileMemory("toCourier");

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
			Message *messageFromCook = toCourier->getMessage();
			
			cout << "Получил сообщение от повара, нужно доставить его покупателю. Отправляюсь" << endl;
			this->printWithDelay("Еду к покупателю", this->getRandomNumber(3 * TIME_COEFF, 8 * TIME_COEFF), TIME_COEFF);

			cout << "Приехал к покупателю" << endl;
			sleep(2 * TIME_COEFF);

			cout << "Отдаю заказ покупателю" << endl;
			sleep(2 * TIME_COEFF);

			Message *messageToBuyer = new Message(*this, messageFromCook->message);
			toBuyer->putMessage(messageToBuyer);
			cout << "Заказ отдан" << endl;
			sleep(2 * TIME_COEFF);

			cout << "Ожидаю оплату" << endl;
			sleep(2 * TIME_COEFF);
			Message *messageToCourier = toCourier->getMessage();

			cout << "Оплата получена" << endl;
			sleep(2 * TIME_COEFF);

			cout << "Выезжаем к диспетчеру, чтобы отдать деньги" << endl;
			sleep(2 * TIME_COEFF);
			this->printWithDelay("Еду к диспетчеру", this->getRandomNumber(3 * TIME_COEFF, 8 * TIME_COEFF), TIME_COEFF);

			cout << "Приехал к диспетчеру" << endl;
			sleep(2 * TIME_COEFF);

			cout << "Отдаю деньги диспетчеру" << endl;
			sleep(2 * TIME_COEFF);
			Message *messageToDispatcher = new Message(*this, messageToCourier->message);
			toDispatcher->putMessage(messageToDispatcher);

			cout << "Деньги отданы диспетчеру" << endl;
			sleep(2 * TIME_COEFF);

			delete messageToDispatcher;
			delete messageToCourier;
			delete messageFromCook;
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


