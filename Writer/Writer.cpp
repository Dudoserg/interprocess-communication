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

		e = new TSemapfore("SemEEE", 1);
		r = new TSemapfore("SenRRR", 0);
		w = new TSemapfore("SenWWW", 0);


		nums = new FileMemory_6_2("param");
		fileMemory_6_2_message = new FileMemory_6_2_message("message");
	};

	~A() {};

	int counter = 96;
	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2 * TIME_COEFF);
		while (true) {
			e->P(); // ждем эстафету на ввод запроса
			if (nums->getNumR() > 0 || nums->getNumW > 0) {
				nums->setWaitW(nums->getWaitW + 1);
				// появился еще один ожидающий писатель
				e->V();
				w->P(); // ждем эстафету на запись	  
			}
			nums->setNumW(nums->getNumW + 1);	//numW++;
			
			estafeta();    //эстафету получили – работаем!
			//<write data>

			string mydata = "first" + to_string(counter++);
			fileMemory_6_2_message->put(mydata);

			//  ждем эстафету - отметить конец
			e->P();

			nums->setNumW(nums->getNumW - 1);	//numW--;
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
	return 0;
}

