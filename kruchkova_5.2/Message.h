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
		init();
	}

	Message(Person &person, string message) {
		init();
		this->personType = person.personType;
		this->personName = person.personName;
		this->message = message;
	}

	void init() {
		//this->vec = new vector<string>;
		//this->help = new Help;
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

