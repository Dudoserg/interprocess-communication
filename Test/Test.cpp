
#include "stdafx.h"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>
#include "GenerateSemaphore.h"
#include "Tsemaphore.h"
#include "FileMemory_6_2.h"
#include "FileMemory_6_2_message.h"

TSemapfore *e = new TSemapfore("SemEEE", 1);
TSemapfore *r = new TSemapfore("SenRRR", 0);
TSemapfore *w = new TSemapfore("SenWWW", 0);


int numR = 0;
int numW = 0;
int waitR = 0;
int waitW = 0;

FileMemory_6_2 * nums;
FileMemory_6_2_message * fileMemory_6_2_message;

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

string mydata = "";
int counter = 0;


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
		//cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;
		while (true) {
			sleep(3 * TIME_COEFF);

			cout << "numR = " << nums->getNumR() << endl;
			cout << "numW = " << nums->getNumW() << endl;
			cout << "waitR = " << nums->getWaitR() << endl;
			cout << "waitW = " << nums->getWaitW() << endl;
			cout << endl << endl;
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
	//nums = new FileMemory_6_2("param");
	//fileMemory_6_2_message = new FileMemory_6_2_message("message");

	//setlocale(LC_ALL, "Russian");

	//std::thread writer([&]() {
	//	while (true) {
	//		cout << "writer" << endl;
	//		e->P(); // ждем эстафету на ввод запроса
	//		if (nums->getNumR() > 0 || nums->getNumW() >0) {//if (numR>0 || numW >0) {
	//			nums->waitW_inc(); //waitW++;
	//			 появился еще один ожидающий писатель
	//			e->V();  
	//			w->P(); // ждем эстафету на запись	  
	//		}
	//		nums->numW_inc();    //numW++;
	//		estafeta();    //эстафету получили – работаем!
	//		<write data>
	//		mydata = "first" + to_string(counter++);
	//		cout << mydata << endl;
	//		fileMemory_6_2_message->put(mydata);

	//		  ждем эстафету - отметить конец
	//		e->P(); 	

	//		nums->numW_dec(); //numW--;
	//		estafeta();
	//	}

	//});

	//std::thread reader([&]() {
	//	while (true) {
	//		cout << "reader" << endl;
	//		e->P(); // ждем эстафету на ввод
	//		cout << "зашел" << endl;

	//		if (nums->getNumW() >0) {//if (numW>0) {
	//			nums->waitR_inc(); //waitR++; // появился читатель
	//			e->V(); 
	//			r->P(); // ждем эстафету на чтение	
	//		}
	//		nums->numR_inc();	//numR++;
	//		estafeta();   //эстафету получили – работаем!
	//		<read data>
	//		string str = fileMemory_6_2_message->get();
	//		cout << str << endl;
	//		e->P(); //  ждем эстафету  -  отметить конец работы
	//		nums->numR_dec(); // numR--;
	//		estafeta();
	//	}

	//});

	//cout << "startWait" << endl;
	//reader.join();
	//writer.join();

	//cout << "end" << endl;

}