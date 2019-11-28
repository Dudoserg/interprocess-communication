
#include "stdafx.h"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>
#include "GenerateSemaphore.h"
#include "TSemapfore.h"


TSemapfore *e = new TSemapfore("SemEEE", 1);
TSemapfore *r = new TSemapfore("SenRRR", 0);
TSemapfore *w = new TSemapfore("SenWWW", 0);


int numR = 0;
int numW = 0;
int waitR = 0;
int waitW = 0;

void estafeta() {        // Передаем эстафету читателю, писателю  вводу запроса на работу            
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

string mydata = "";
int counter = 0;
int main()
{
	setlocale(LC_ALL, "Russian");

	std::thread writer([&]() {
		while (true) {
			//cout << "writer" << endl;
			e->P(); // ждем эстафету на ввод запроса
			if (numR>0 || numW >0) {
				waitW++;
				// появился еще один ожидающий писатель
				e->V();  
				w->P(); // ждем эстафету на запись	  
			}
			numW++;
			estafeta();    //эстафету получили – работаем!
			//<write data>
			mydata = "first" + to_string(counter++);
			//  ждем эстафету - отметить конец
			e->P(); 	

			numW--;
			//estafeta();
		}

	});

	std::thread reader([&]() {
		while (true) {
			//cout << "reader" << endl;
			e->P(); // ждем эстафету на ввод
			//cout << "зашел" << endl;

			if (numW>0) {
				waitR++; // появился читатель
				e->V(); 
				r->P(); // ждем эстафету на чтение	
			}
			numR++;
			estafeta();   //эстафету получили – работаем!
			//<read data>
			cout << mydata << endl;
			e->P(); //  ждем эстафету  -  отметить конец работы
			numR--;
			//estafeta();
		}

	});

	//cout << "startWait" << endl;
	reader.join();
	writer.join();

	cout << "end" << endl;

}