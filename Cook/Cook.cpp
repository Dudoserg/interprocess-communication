// Cook.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <chrono>

#include "FileMemory.h"
#include "PersonType.h"
#include "Person.h"
#include "Message_Fridge.h"
#include "Products.h"

using namespace std::chrono;

class A : Person
{

public:
	FileMemory * toDispatcher;

	FileMemory * dispatcherToCook;
	FileMemory * fridgeToCook;

	FileMemory * toFridge;

	FileMemory * toCourier;
	int money;

	A() {
		personName = "CoOoK";
		personType = PersonType::Enum::DISPATCHER;

		toDispatcher = new FileMemory("toDispatcher");
		dispatcherToCook = new FileMemory("dispatcherToCook");

		fridgeToCook = new FileMemory("fridgeToCook");

		toFridge = new FileMemory("toFridge");

		toCourier = new FileMemory("toCourier");

		money = 0;
	};

	~A() {};

	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2000);
		while (true) {

			cout << "Начинаю ожидать сообщения от Кого-либо" << endl;
			sleep(2000);
			Message *messageBaseFromDispatcher = dispatcherToCook->getMessage();
			cout << "Получено сообщение от Диспетчера [ " << messageBaseFromDispatcher->personName << " ]" << endl;
			sleep(2000);

			this->printWithDelay("Вспоминаю рецепт", 2000);
			sleep(2000);

			if (messageBaseFromDispatcher->message == "pizza") {

				Message_Fridge *pizza = new Message_Fridge(*this);
				pizza->putInMap(Products::Enum::Dough, 350);
				pizza->putInMap(Products::Enum::Sausage, 150);
				pizza->putInMap(Products::Enum::Tomatoes, 150);
				pizza->putInMap(Products::Enum::Cheese, 100);

				cout << "Обращаемся к холодильнику за ингридиентами" << endl;
				sleep(2000);
				toFridge->putMessage(pizza);

			} else if(messageBaseFromDispatcher->message == "burger") {
				Message_Fridge *pizza = new Message_Fridge(*this);
				pizza->putInMap(Products::Enum::Cutlet, 1);
				pizza->putInMap(Products::Enum::Bun, 1);
				pizza->putInMap(Products::Enum::Tomatoes, 50);

				cout << "Обращаемся к холодильнику за ингридиентами" << endl;
				sleep(2000);
				toFridge->putMessage(pizza);
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



