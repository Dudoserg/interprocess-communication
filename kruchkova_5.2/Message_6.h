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
	int personId;
	int idToUser;

	Message_6() {
	}

	Message_6(Person &person, string message) {
		this->personType = person.personType;
		this->personName = person.personName;
		this->message = message;
		this->personId = person.id;
	}



	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		//ar(personType, personName, message, *vec, *help);
		ar(personType, personName, message, personId, idToUser);
	}

	string getPersonTypeSTR() {
		return PersonType::toString(personType);
	}

private: 

};

