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
#include <fstream>
#include "FileMemory.h"
#include "PersonType.h"
#include "Person.h"
#include "Message_Fridge.h"
#include "Products.h"

using namespace std::chrono;

class A : Person
{

public:

	const int TIME_COEFF = 1000;

	FileMemory * toFridge;

	FileMemory * fridgeToCook;

	map<Products::Enum, int> *ingridients;

	A() {
		personName = "**Fridge**";
		personType = PersonType::Enum::FRIDGE;

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
		std::ifstream asdf("bel.txt");
		for (std::string line; getline(asdf, line); )
		{
			cout << line << endl;
		}
		sleep(2 * TIME_COEFF);
		while (true) {

			cout << "Жду сообщение" << endl;
			sleep(2 * TIME_COEFF);
			Message_Fridge *takedMessage = toFridge->getMessage_Fridge();
			cout << "Получил сообщение от Повара : " << endl << endl;
			cout << "-----------------------------------" << endl;
			for (std::map<Products::Enum, int>::iterator it = takedMessage->getMap()->begin();
				it != takedMessage->getMap()->end(); ++it) {
				cout << "Запрашиваю " << Products::toString(it->first) << " в количестве " << it->second << endl;
			}
			cout << "-----------------------------------" << endl;
			sleep(2 * TIME_COEFF);

			cout << "===================================" << endl;
			cout << "Начинаю проверку наличия необходимых продуктов" << endl << endl;

			bool flag_enough_products = true;
			for (std::map<Products::Enum, int>::iterator it = takedMessage->getMap()->begin();
				it != takedMessage->getMap()->end(); ++it)
			{
				Products::Enum needProduct = it->first;
				int needCount = it->second;
				map<Products::Enum, int>::iterator found = this->ingridients->find(needProduct);
				if (found != this->ingridients->end() && found->second >= needCount) {
					// Если в холодильнике достаточно текущего продукта
					cout << Products::toString(needProduct) << " присутствует в количестве: " << needCount
						<< ". Всего: " << found->second << endl;
				}
				else {
					// Увы такого не хватает
					cout << Products::toString(needProduct) << " ОТСУТСТВУЕТ в количестве " << needCount 
						<< ". Всего: " << found->second << endl;
					flag_enough_products = false;
				}
				sleep(1 * TIME_COEFF);
			}

			cout << endl  <<  "Проверка закончена" << endl << endl;
			sleep(2 * TIME_COEFF);
			Message_Fridge *messageToCook = new Message_Fridge(*this);

			if (flag_enough_products) {
				messageToCook->status = 1;
				cout << "Все необходимые продукты в наличии" << endl << endl;
				sleep(2 * TIME_COEFF);
				for (std::map<Products::Enum, int>::iterator it = takedMessage->getMap()->begin();
					it != takedMessage->getMap()->end(); ++it)
				{
					Products::Enum needProduct = it->first;
					int needCount = it->second;
					map<Products::Enum, int>::iterator found = this->ingridients->find(needProduct);
					// Обновляем количество оставшееся в холодильнике
					(*ingridients)[needProduct] = (*ingridients)[needProduct] - needCount;
					messageToCook->putInMap(needProduct, needCount);
					cout << Products::toString(needProduct) << " осталось в количестве " << (*ingridients)[needProduct] << endl;
					sleep(1 * TIME_COEFF);
				}
				cout << endl << "===================================" << endl << endl;
				cout << "Отправляем продукты повару" << endl;
			}
			else {
				// Какого то продукта не хватает
				messageToCook->status = 0;
				cout << "Одного\\некоторых продуктов не хватает" << endl;
				sleep(2 * TIME_COEFF);
				cout << endl << "===================================" << endl << endl;
				cout << "Отправляем Повару сообщение об отсутствии продуктов" << endl;
				
			}
			fridgeToCook->putMessage(messageToCook);
			

			delete takedMessage;
			delete messageToCook;
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



