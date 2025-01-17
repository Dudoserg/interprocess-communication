// Buyer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <fstream>
#include "FileMemory_6.h"
#include "FileMemory_6_ToProducer.h"
#include "PersonType.h"
#include "Person.h"
#include "Message_6.h"
#include "Message_arr_6.h"
#include <chrono>

using namespace std::chrono;
using namespace std;

class A : public Person
{
public:

	const int TIME_COEFF = 1000;

	FileMemory_6_ToProducer * toShop;
	FileMemory_6 * toBuyer;


	A() {
		toShop = new FileMemory_6_ToProducer("toShop6");
		toBuyer = new FileMemory_6("toBuyer6");

		id = this->getRandomNumber(0, 60000);
		personName = "Shop #" + to_string(id);
		personType = PersonType::Enum::SHOP;

	};

	~A() {};

	// thread
	void operator()() {
		int count = 0;
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2 * TIME_COEFF);
		while (true) {
	
			Message_6 *messageToShop = toShop->getMessage_6();
			cout << "Получили сообщение : " << messageToShop->message << " От " 
				<< messageToShop->personName << "(id=" << messageToShop->personId << ")" << endl;
			sleep(2 * TIME_COEFF);
			
			this->printWithDelay("Произвожу че надо пользователю", this->getRandomNumber(4000, 10000), TIME_COEFF);
			cout << "Приготовил : " << messageToShop->message + " ГОТОВАЯ" << endl;
			Message_6 *messageToBuyer = new Message_6(*this, messageToShop->message + " ГОТОВАЯ");
			messageToBuyer->idToUser = messageToShop->personId;
			cout << "Начинаю отправку готового продукта" << endl;

			// Тут если че повесим
			toBuyer->putMessage_6(messageToBuyer);

			
			cout << "Сообщение Отправлено! " << messageToBuyer->message << endl;

		}
	}
private:;


};


int main()
{
	 srand(static_cast<unsigned int>(time(0)));
	setlocale(LC_ALL, "Russian");
	A p;
	thread th(p);
	if (th.joinable())
		th.join();
	return 0;
}

