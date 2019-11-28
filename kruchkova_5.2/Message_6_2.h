#pragma once
class Message_6_2
{
public:

	int numR = 0;
	int numW = 0;
	int waitR = 0;
	int waitW = 0;

	Message_6_2() {};
	~Message_6_2() {};


	template<class Archive> // public serialization (normal)
	void serialize(Archive & ar)
	{
		//ar(personType, personName, message, *vec, *help);
		ar(numR, numW, waitR, waitW);
	}
};

