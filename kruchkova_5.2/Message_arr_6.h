#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include "Message_6.h"
#pragma once

using namespace std;

class Message_arr_6
{
public:

	vector<unique_ptr<Message_6>> *data;

	Message_arr_6() {
		data = new vector<unique_ptr<Message_6>>;
	};
	~Message_arr_6() {};

	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		//ar(personType, personName, message, *vec, *help);
		ar(*data);
	}
};

