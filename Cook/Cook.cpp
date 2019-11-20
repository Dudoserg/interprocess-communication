// Cook.cpp: определяет точку входа для консольного приложения.
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

			} else if(messageBaseFromDispatcher->message == "pizza") {

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



