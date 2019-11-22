// Dispatcher.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <random>
#include "FileMemory.h"
#include "EndFlagFileMemory.h"
#include "PersonType.h"
#include "Person.h"
#include "MySem.h"
#include <chrono>

using namespace std::chrono;

class A : Person
{

public:

	const int TIME_COEFF = 1000;

	FileMemory * kanal;


	HANDLE semEnd;

	bool flagWork_Job;
	bool flagWork_BackgroundTask;

	int money;

	A() {
		personName = "COURIER";
		personType = PersonType::Enum::COURIER;

		kanal = new FileMemory("kanal");
		
		semEnd = MySem::createOrOpenSemaphore("TestClose", 0);

		flagWork_Job = true;
		flagWork_BackgroundTask = true;

		money = 0;
	};

	~A() {};

	void job() {
		cout << personName << "(" << PersonType::toString(personType) << ") начал работу" << endl;

		sleep(2 * TIME_COEFF);
		while (flagWork_Job) {

			/*kanal->putMessage(new Message(*this, "first"));
			cout << "sended first messsage" << endl;
			kanal->putMessage(new Message(*this, "second"));
			cout << "sended first messsage" << endl;*/
			cout << rand() << endl;
			sleep(2 * TIME_COEFF);
		}
	}
	void backgroundTask() {
		cout << personName << "(" << PersonType::toString(personType) << ") BackgroundTask start" << endl;
		while (flagWork_BackgroundTask) {
			// Ложимся спать на пару секунд
			sleep(1000);
			// СЮДА НЕ ЗАЙДЕМ, ПОКА НЕ СБРОСИМ В УПРАВЛЯЮЩЕЙ ПРОГЕ
			WaitForSingleObject(semEnd, INFINITE);
			ReleaseSemaphore(semEnd, 1, NULL);
		
			cout << endl << "завершаемся..." << endl;
			exit(0);
			// Если завершаем работу, то закрываем все ресурсы, и останавливаем основной поток "JOB"
			// Если не завершаем работу, то ничего не делаем =)
		}
	}

	// thread
	void operator()() {
		cout << "before join" << endl;

		// Поток с основнйо работой
		std::thread mainJob(std::bind(&A::job, this));

		// Поток-диспетчер, наблюдает не нужно ли завершить прогу
		std::thread task_background(std::bind(&A::backgroundTask, this));

		vector<std::thread*> threadsArray;
		threadsArray.push_back(&mainJob);
		threadsArray.push_back(&task_background);
		//threadsArray.push_back(task_background);

		for (int i = 0; i < threadsArray.size(); i++) {
			cout << " start thread # " << (i + 1) << "             "<< endl;
			if (threadsArray[i]->joinable())
				threadsArray[i]->join();
		}

		cout << "after join" << endl;

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


