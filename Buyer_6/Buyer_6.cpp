// Buyer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <fstream>
#include "FileMemory.h"
#include "PersonType.h"
#include "Person.h"
#include <chrono>

using namespace std::chrono;
using namespace std;

class A : public Person
{
public:

	const int TIME_COEFF = 1000;

	FileMemory * toCook6;
	FileMemory * toBuyer6;


	A() {
		toCook6 = new FileMemory("toCook6");
		toBuyer6 = new FileMemory("toBuyer6");

		personName = "Buyer #" + this->getRandomNumber(0, 100);
		personType = PersonType::Enum::BUYER;

	};

	~A() {};

	// thread
	void operator()() {
		int count = 0;
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;
		
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


			Message *messageTotoCook6 = new Message(*this, order);
			cout << "Посылает сообщение Повару, Текст сообщения : " << messageTotoCook6->message << endl;


			sleep(2 * TIME_COEFF);
			// Тут если че повесим
			toCook6->putMessage(messageTotoCook6);

			cout << "Начинает ожидать ответа ..." << endl;
			Message *take = toBuyer6->getMessage();

			
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

