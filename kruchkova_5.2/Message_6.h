#include <string>

#include "PersonType.h"
#include "Person.h"
#pragma once

using namespace std;

class Message_6
{
public: 

	PersonType::Enum personType;
	string personName;

	string message;

	int id;





	Message_6() {
	}

	Message_6(Person &person, string message) {
		this->personType = person.personType;
		this->personName = person.personName;

	}



	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		//ar(personType, personName, message, *vec, *help);
		ar(personType, personName, message);
	}

	string getPersonTypeSTR() {
		return PersonType::toString(personType);
	}

private: 

};

