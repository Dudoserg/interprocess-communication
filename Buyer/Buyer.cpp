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
	FileMemory * toBuyer;
	FileMemory * toCourier;
	FileMemory * toDispatcher;

	A() {
		toBuyer = new FileMemory("toBuyer_Put", "toBuyer_Get", "toBuyer_File");
		toCourier = new FileMemory("toCourier_Put", "toCourier_Get", "toCourier_File");
		toDispatcher = new FileMemory("toDispatcher_Put", "toDispatcher_Get", "toDispatcher_File");
		personName = "Buyer228";
		personType = PersonType::Enum::BUYER;
	};

	~A() {};

	// thread
	void operator()() {
		int count = 0;
		cout << personName << "(" << PersonType::toString(personType) <<") начал работу" << endl;
		sleep(2000);
		while (true) {
			double random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			string order = "pizza";

			if (random <= 0.5) {
				order = "pizza";
			}
			else if (0.5 < random <= 1) {
				order = "pizza";
			}
			

			Message *messageToDispatcher = new Message(*this, "message");

			cout << "Посылает сообщение диспетчеру, Текст сообщения : " << messageToDispatcher->message << endl;
			sleep(2000);
			toDispatcher->putMessage(messageToDispatcher);


			cout << "Начинает ожидать ответа от Диспетчера или курьера..." << endl;
			
			Message *take = toBuyer->getMessage();
			sleep(1000);
			cout << "Получено сообщение [" << take->message << "] от [" << take->personName << "] " << endl;
			
			switch (take->personType)
			{
				case PersonType::Enum::DISPATCHER: {
					cout << "Получен ответ от диспетчера. Ответ = " << take->message << endl;
					this->printWithDelay("Пользователь обдумывает новый заказ", this->getRandomNumber(5000, 10000));
					break;
				}
				case PersonType::Enum::COURIER: {
					cout << "Получен ответ от Курьера. Ответ = " << take->message << endl;
					sleep(1000);
					cout << "Платим курьеру";
					sleep(1000);
					toCourier->putMessage(new Message(*this, "228"));
					printWithDelay("Кушаем", this->getRandomNumber(5000, 10000));
					cout << "Съел свой заказ " << endl;
					break;
				}
				default:
					break;
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

