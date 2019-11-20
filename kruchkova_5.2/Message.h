#include <string>
#include "PersonType.h"
#include "Person.h"
#pragma once

using namespace std;

class Message
{
public: //function declarations 

	PersonType::Enum personType;
	string personName;

	string message;

	//vector<string> *vec;

	//Help *help;

	Message() {
	}

	Message(Person &person, string message) {
		this->personType = person.personType;
		this->personName = person.personName;
		this->message = message;
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

private: // member variables 

};

