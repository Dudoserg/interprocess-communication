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
#include <chrono>

using namespace std::chrono;

class A : public Person
{
public:

	const int TIME_COEFF = 1000;

	FileMemory * toBuyer;
	FileMemory * toCourier;
	FileMemory * toDispatcher;

	A() {
		toBuyer = new FileMemory("toBuyer");
		toCourier = new FileMemory("toCourier");
		toDispatcher = new FileMemory("toDispatcher");
		personName = "Buyer228";
		personType = PersonType::Enum::BUYER;
	};

	~A() {};

	// thread
	void operator()() {
		int count = 0;
		cout << personName << "(" << PersonType::toString(personType) <<") начал работу" << endl;
		sleep(2 * TIME_COEFF);
		while (true) {
			double random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			string order = "pizza";

			if (random <= 0.5) {
				order = "pizza";
			}
			else if (0.5 < random <= 1) {
				order = "burger";
			}
			

			Message *messageToDispatcher = new Message(*this, order);

			cout << "Посылает сообщение диспетчеру, Текст сообщения : " << messageToDispatcher->message << endl;
			sleep(2 * TIME_COEFF);
			toDispatcher->putMessage(messageToDispatcher);


			cout << "Начинает ожидать ответа от Диспетчера или курьера..." << endl;
			
			Message *take = toBuyer->getMessage();
			sleep(1 * TIME_COEFF);
			cout << "Получено сообщение [" << take->message << "] от [" << take->personName << "] " << endl;
			
			switch (take->personType)
			{
				case PersonType::Enum::DISPATCHER: {
					cout << "Получен ответ от диспетчера. Ответ = " << take->message << endl;
					this->printWithDelay("Пользователь обдумывает новый заказ",
						this->getRandomNumber(5 * TIME_COEFF, 10 * TIME_COEFF), TIME_COEFF);
					break;
				}
				case PersonType::Enum::COURIER: {
					cout << "Получен ответ от Курьера. Ответ = " << take->message << endl;
					sleep(1 * TIME_COEFF);
					cout << "Платим курьеру";
					sleep(1 * TIME_COEFF);
					toCourier->putMessage(new Message(*this, "228"));
					printWithDelay("Кушаем", this->getRandomNumber(5 * TIME_COEFF, 1 * TIME_COEFF), TIME_COEFF);
					cout << "Съел свой заказ " << endl;
					break;
				}
				default:
					break;
			}
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

