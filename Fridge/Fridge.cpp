// Cook.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <map>
#include <iostream>
#include <cassert>

#include "FileMemory.h"
#include "PersonType.h"
#include "Person.h"
#include "Message_Fridge.h"
#include "Products.h"

using namespace std::chrono;

class A : Person
{

public:

	const int timeCoeff = 1000;

	FileMemory * toFridge;

	FileMemory * fridgeToCook;

	map<Products::Enum, int> *ingridients;

	A() {
		personName = "**Fridge**";
		personType = PersonType::Enum::DISPATCHER;

		toFridge = new FileMemory("toFridge");
		fridgeToCook = new FileMemory("fridgeToCook");

		ingridients = new map<Products::Enum, int>;
		// Заполняем холодильник при инициализации
		(*ingridients)[Products::Enum::Dough] = 1000;
		(*ingridients)[Products::Enum::Sausage] = 1000;
		(*ingridients)[Products::Enum::Cheese] = 1000;
		(*ingridients)[Products::Enum::Tomatoes] = 1000;
		(*ingridients)[Products::Enum::Cutlet] = 3;
		(*ingridients)[Products::Enum::Bun] = 3;

	};

	~A() {};

	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2 * timeCoeff);
		while (true) {

			cout << "Жду сообщение" << endl;
			sleep(2 * timeCoeff);
			Message_Fridge *takedMessage = toFridge->getMessage_Fridge();
			cout << "Получил сообщение от Повара : " << endl;
			sleep(2 * timeCoeff);

			cout << "===================================" << endl;
			cout << "Начинаю проверку наличия необходимых продуктов" << endl;

			for (std::map<Products::Enum, int>::iterator it = takedMessage->getMap()->begin();
				it != takedMessage->getMap()->end(); ++it)
			{
				
			}

		}
	}
private:;
		int getRandomNumber(int min, int max)
		{
			static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
			// Равномерно распределяем рандомное число в нашем диапазоне
			return static_cast<int>(rand() * fraction * (max - min + 1) + min);
		}
		void printWithDelay(string str, int millis) {
			milliseconds time = getTime();
			int countPoint = 0;
			while ((getTime() - time) < (milliseconds)millis) {
				cout << "\r";
				for (int i = 0; i < str.length() + 1; i++) {
					cout << " ";
				}
				cout << std::flush;

				cout << "\r";
				cout << str;
				for (int i = 0; i < countPoint; i++) {
					cout << ".";
				}
				cout << std::flush;

				countPoint++;
				if (countPoint > 3)
					countPoint = 0;
				sleep(300);
			}
			cout << endl;
		}
		void sleep(int millis) {
			this_thread::sleep_for(std::chrono::milliseconds(millis));
		}
		milliseconds getTime() {
			milliseconds ms = duration_cast< milliseconds >(
				system_clock::now().time_since_epoch()
				);
			return ms;
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



