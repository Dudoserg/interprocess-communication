// CerealTest.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include"cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/concepts/pair_associative_container.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "FileMemory.h"
#include "PersonType.h"

using namespace std;

class MyClass {
public: //function declarations 

	PersonType::Enum personType;
	string personName;
	string message;
	vector<string> *vec;

	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		ar(personType, personName, message, *vec);
	}
	MyClass() {
		init();
	}


	void init() {
		this->vec = new vector<string>;
	}

	
	

private: // member variables 
	
};

int main()
{
	MyClass myclass;
	myclass.personName = "personName";
	myclass.personType = PersonType::Enum::BUYER;
	myclass.message = "message";
	myclass.vec->push_back("raz");
	myclass.vec->push_back("dwa");
	myclass.vec->push_back("tri");
	myclass.vec->push_back("chetire");

	
	std::stringstream os;
	{
		cereal::JSONOutputArchive archive_out(os);
		archive_out(CEREAL_NVP(myclass));
	}
	string json_str = os.str();
	cout << json_str << endl;

	// deserialize
	std::stringstream is(json_str);
	MyClass data_new;
	{
		cereal::JSONInputArchive archive_in(is);
		archive_in(data_new);
		cout << "personName = " << data_new.personName << endl;
		cout << "personType = " << PersonType::toString(data_new.personType) << endl;
		cout << "message = " << data_new.message << endl;
		for (int i = 0; i < data_new.vec->size(); i++) {
			cout << (*data_new.vec)[i] << endl;
		}
	}
	
	return 0;
}
