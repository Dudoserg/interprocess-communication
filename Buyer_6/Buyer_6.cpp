// Buyer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "pch.h"
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
		toShop = new FileMemory_6_ToProducer("toShop");
		toBuyer = new FileMemory_6("toBuyer");

		id = this->getRandomNumber(0, 60000);
		personName = "Buyer #" + to_string(id);
		personType = PersonType::Enum::BUYER;

	};

	~A() {};

	// thread
	void operator()() {
		int count = 0;
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;
		
		sleep(1 * TIME_COEFF);
		while (true) {
			double random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			string order = "pizza" + to_string(this->id);

			if (random <= 0.5) {
				order = "pizza" + to_string(this->id);
			}
			else if (0.5 < random <= 1) {
				order = "burger" + std::to_string(this->id);
			}


			Message_6 *messageToShop = new Message_6(*this, order);
			cout << "Посылает сообщение Повару, Текст сообщения : " << messageToShop->message << endl;
			sleep(1 * TIME_COEFF);
			// Тут если че повесим
			toShop->putMessage_6(messageToShop);

			cout << "Начинает ожидать ответа ..." << endl;
				
			
			Message_6 *take = NULL;
			while (take == NULL) {
				Message_6 *take = toBuyer->getMessage_6(id);
			}
			cout << "Сообщение получено:" << take->message << endl;	

			int x; cin >> x;
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