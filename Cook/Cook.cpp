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

	const int TIME_COEFF = 10;

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

		sleep(2 * TIME_COEFF);
		while (true) {

			cout << "Начинаю ожидать сообщения от Кого-либо" << endl;
			sleep(2 * TIME_COEFF);
			Message *messageBaseFromDispatcher = dispatcherToCook->getMessage();
			cout << "Получено сообщение от Диспетчера [ " << messageBaseFromDispatcher->message << " ]" << endl;
			sleep(2 * TIME_COEFF);

			this->printWithDelay("Вспоминаю рецепт", 2 * TIME_COEFF);
			sleep(2 * TIME_COEFF);

			if (messageBaseFromDispatcher->message.compare("pizza") == 0) {

				Message_Fridge *pizza = new Message_Fridge(*this);
				pizza->putInMap(Products::Enum::Dough, 350);
				pizza->putInMap(Products::Enum::Sausage, 150);
				pizza->putInMap(Products::Enum::Tomatoes, 150);
				pizza->putInMap(Products::Enum::Cheese, 100);

				cout << "Обращаемся к холодильнику за ингридиентами" << endl;
				sleep(2 * TIME_COEFF);
				toFridge->putMessage(pizza);

			} else if(messageBaseFromDispatcher->message.compare("burger") == 0) {
				Message_Fridge *burger = new Message_Fridge(*this);
				burger->putInMap(Products::Enum::Cutlet, 1);
				burger->putInMap(Products::Enum::Bun, 1);
				burger->putInMap(Products::Enum::Tomatoes, 50);

				cout << "Обращаемся к холодильнику за ингридиентами" << endl;
				sleep(2 * TIME_COEFF);
				toFridge->putMessage(burger);
			}

			// Ждем ответа холодильника
			cout << "Ждем ответа от холодильника" << endl;
			sleep(2 * TIME_COEFF);
			Message_Fridge *messageFromFridge = fridgeToCook->getMessage_Fridge();
			cout << "Ответ от холодильника пришел" << endl;
			sleep(2 * TIME_COEFF);


			if (messageFromFridge->getStatus() == 1) {
				// Все окей готовим
				cout << "Ответ положительный, необходимые продукты есть, начинаю готовить" << endl;
				sleep(2 * TIME_COEFF);
				printWithDelay("Готовлю", this->getRandomNumber(3 * TIME_COEFF, 10 * TIME_COEFF));
				// Шлем сообщение курьеру
				cout << "Шлю сообщение Курьеру" << endl;
				sleep(2 * TIME_COEFF);
				Message *message = new Message(*this, messageBaseFromDispatcher->message + string(" готовая"));
				toCourier->putMessage(message);
			} else {
				// не хватает продукта
				cout << "Ответ отрицательный, нет нужных продуктов" << endl;
				sleep(2 * TIME_COEFF);
				
				Message *message = new Message(*this, "Не хватает ингридиентов");
				cout << "Шлем сообщение диспетчеру: " << message->message << endl;
				sleep(2 * TIME_COEFF);
				toDispatcher->putMessage(message);

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



