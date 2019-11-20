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
		
		sleep(2000);
		while (true) {

			cout << "Начинаю ожидать сообщения от Кого-либо" << endl;
			sleep(2000);
			Message *messageBaseTaked = toDispatcher->getMessage();
			
			switch (messageBaseTaked->personType) {
				case PersonType::Enum::BUYER : {
					cout << "Получено сообщение от покупателя [ " << messageBaseTaked->personName << " ]" << endl;
					sleep(2000);
					printWithDelay("Обрабатываю заказ", 5000);
					dispatcherToCook->putMessage(new Message(*this, messageBaseTaked->message));
					cout << "Заказ обработан, передан повару" << endl;
					sleep(2000);
					break;
				}
				case PersonType::Enum::COOK: {
					cout << "Получено сообщение от Повара [ " << messageBaseTaked->personName << " ]" << endl;
					sleep(2000);
					printWithDelay("Звоним покупателю", getRandomNumber(1000, 2000));
					toBuyer->putMessage(new Message(*this, messageBaseTaked->message));
					cout << "Покупатель осведомлен";
					sleep(2000);
					break;
				}
				case PersonType::Enum::COURIER : {
					cout << "Получено сообщение от Курьера [ " << messageBaseTaked->personName << " ]" << endl;
					sleep(2000);
					money += stoi(messageBaseTaked->message);
					cout << "Казна пополнена: +" << messageBaseTaked->message << " Всего в казне: " << money << endl;
					sleep(2000);
					break;
				}
				default:{
					cout << "switch error";
					break;
				}
			}

			/*cout << "Получено сообщение [" << messageToDispatcher->message << "] от [" << messageToDispatcher->personName << "] " << endl;
			sleep(2000);
			cout << "Готовим ответ..." << endl;
			sleep(2000);
			Message *messageToBuyer = new Message(*this, "feedback");
			cout << "Посылаем ответ..." << endl;
			sleep(2000);
			toBuyer->putMessage(messageToBuyer);*/
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


