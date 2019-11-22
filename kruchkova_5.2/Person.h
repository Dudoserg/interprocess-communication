#include <windows.h>
#include <stdio.h>

#include <thread>
#include <random>




#include <string>
#include <chrono>
#include <iostream>
#include "PersonType.h"

using namespace std::chrono;
#pragma once

using namespace std;

class Person
{
public:

	PersonType::Enum personType;
	string personName;

	Person(PersonType::Enum personType, string personName) {
		this->personName = personName;
		this->personType = personType;
	};
	Person() {};
	~Person() {};

	int getRandomNumber(int min, int max)
	{
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
		// Равномерно распределяем рандомное число в нашем диапазоне
		return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	}
	void printWithDelay(string str, int millis, int TIME_COEFF) {
		milliseconds time = getTime();
		int countPoint = 0;
		while ((getTime() - time) < (milliseconds)millis) {
			cout << "\r";
			for (int i = 0; i < str.length() + 4; i++) {
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
			sleep(0.3 * TIME_COEFF);
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

