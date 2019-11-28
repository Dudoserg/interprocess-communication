// Writer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include <chrono>

//#include "FileMemory.h"
//#include "PersonType.h"

//#include "Message_Fridge.h"
//#include "Products.h"

#include "Tsemaphore.h"
#include "Person.h"
#include "FileMemory_6_2.h"
#include "FileMemory_6_2_message.h"

class A : Person
{

public:

	const int TIME_COEFF = 1000;

	TSemapfore *e;
	TSemapfore *r;
	TSemapfore *w;

	// Тут хранятся четыре параметра
	FileMemory_6_2 * nums;
	FileMemory_6_2_message * fileMemory_6_2_message;

	A() {
		personName = "writer";
		personType = PersonType::Enum::COOK;

		e = new TSemapfore("SemEEE", 1);		// Вход в неделимое действие
		r = new TSemapfore("SenRRR", 0);		// 
		w = new TSemapfore("SenWWW", 0);

		// Тут лежат параметры 
		// количество работающих и ожидающих
		// читателей и писателей
		nums = new FileMemory_6_2("param");

		// Через это передаем сообщение
		fileMemory_6_2_message = new FileMemory_6_2_message("message");
	};

	~A() {};


	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2 * TIME_COEFF);
		while (true) {
			//cout << "reader" << endl;
			e->P(); // ждем эстафету на ввод
					//cout << "зашел" << endl;

			if (nums->getNumW()  > 0 ) {
				nums->setWaitR(nums->getWaitR() + 1);	//waitR++; // появился читатель
				e->V();
				r->P(); // ждем эстафету на чтение	
			}
			nums->setNumr(nums->getNumR() + 1);	//numR++;
			estafeta();   //эстафету получили – работаем!
			//<read data>
			string str = fileMemory_6_2_message->get();
			cout << str << endl;

			e->P(); //  ждем эстафету  -  отметить конец работы
			nums->setNumr( nums->getNumR() - 1);	// numR--;
			estafeta();
		}
	}

private:
	void estafeta() {        // Передаем эстафету читателю, писателю  вводу запроса на работу
		int numR = nums->getNumR();
		int numW = nums->getNumW();
		int waitW = nums->getWaitW();
		int waitR = nums->getWaitR();

		if ((numR == 0) && (numW == 0) && (waitW > 0)) {
			waitW--;    w->V();  // работа писателя               
		}
		else if ((numW == 0) && (waitW == 0) && (waitR > 0)) {
			waitR--;         r->V();   // читатель  может работать
		}
		else {
			e->V();    // запрос ввода     
		}
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

