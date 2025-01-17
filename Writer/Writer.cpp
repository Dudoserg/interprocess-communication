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

	int counter = 96;
	// thread
	void operator()() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2 * TIME_COEFF);
		bool flagWort = true;
		string mydata;
		while (flagWort) {
			//cout << "writer" << endl;
			e->P(); // ждем эстафету на ввод запроса
			if (nums->getNumR() > 0 || nums->getNumW() >0) {//if (numR>0 || numW >0) {
				nums->waitW_inc(); //waitW++;
								   // появился еще один ожидающий писатель
				e->V();
				w->P(); // ждем эстафету на запись	  
			}
			nums->numW_inc();    //numW++;
			estafeta();    //эстафету получили – работаем!
						   //<write data>
		
			cout << "Я написал : " << mydata << endl;

			fileMemory_6_2_message->put(mydata);

			//  ждем эстафету - отметить конец
			e->P();

			nums->numW_dec(); //numW--;
			estafeta();
			
			cout << "Enter : \n";
			cin >> mydata;
		}
	}

private:
	void estafeta() {        // Передаем эстафету читателю, писателю  вводу запроса на работу            
		if ((nums->getNumR() == 0) && (nums->getNumW() == 0) && (nums->getWaitW() > 0)) {//if ((numR == 0) && (numW == 0) && (waitW > 0)) {
			nums->waitW_dec();	//waitW--;   
			w->V();  // работа писателя               
		}
		else if ((nums->getNumW() == 0) && (nums->getWaitW() == 0) && (nums->getWaitR() > 0)) {	//else if ((numW == 0) && (waitW == 0) && (waitR > 0)) {
			nums->waitR_dec(); //waitR--;         
			r->V();   // читатель  может работать
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

