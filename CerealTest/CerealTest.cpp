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

using namespace std;

class MyClass {
public: //function declarations 
	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		ar(x, y, z, *vec);
	}
	MyClass(string x, int y, bool z) {
		init();
		this->x = x;
		this->y = y;
		this->z = z;
	}
	MyClass() { init(); };

	void init() {
		this->vec = new vector<string>;
	}

	string getX() {
		return this->x;
	}
	void setX(string tmp) {
		this->x = tmp;
	}


	int getY() {
		return this->y;
	}
	void setY(int tmp) {
		this->y = tmp;
	}


	bool getZ() {
		return this->z;
	}
	void setZ(bool tmp) {
		this->z = tmp;
	}


	vector<string>* getVec() {
		return this->vec;
	}
	

private: // member variables 
	string x;
	int y;
	bool z;
	vector<string> *vec;
};

int main()
{
	MyClass myclass;
	myclass.setX("hello");
	myclass.setY(123);
	myclass.setZ(true);
	myclass.getVec()->push_back("raz");
	myclass.getVec()->push_back("dwa");
	myclass.getVec()->push_back("tri");
	myclass.getVec()->push_back("chetire");
	
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
		for (int i = 0; i < data_new.getVec()->size(); i++) {
			cout << (*data_new.getVec())[i] << endl;
		}
	}
	
	return 0;
}
