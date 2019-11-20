#include <map>
#include <iostream>
#include <cassert>
#include <string>
#include "PersonType.h"
#include "Person.h"
#include "Products.h"

#pragma once

using namespace std;

class Message_Fridge
{
private :

	PersonType::Enum personType;
	string personName;
	map<Products::Enum, int> *m;
	int status;
public:



	Message_Fridge(Person &person) {
		this->personType = person.personType;
		this->personName = person.personName;
		m = new map<Products::Enum, int>;
		status = 99;
	}

	Message_Fridge() {};
	~Message_Fridge() {};

	void putInMap(Products::Enum str, int count) {
		(*m)[str] = count;
	}

	map<Products::Enum, int>* getMap() {
		return this->m;
	}

	string getPersonName() {
		return personName;
	}

	string getPersonTypeSTR() {
		return PersonType::toString(personType);
	}

	PersonType::Enum getPersonType() {
		return personType;
	}

	void setStatus(int x) {
		status = x;
	}
	int getStatus() {
		return status;
	}

	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		//ar(personType, personName, message, *vec, *help);
		ar(personType, personName, *m);
	}
};

