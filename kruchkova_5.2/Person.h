#include "PersonType.h"
#include <string>
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
};

