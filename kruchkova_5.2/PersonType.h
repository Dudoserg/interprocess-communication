#include <string>
#pragma once

class PersonType
{
public:
	enum class Enum
	{
		BUYER,
		DISPATCHER
	};

	static std::string toString(Enum myEnum) {
		switch (myEnum) {
			case PersonType::Enum::BUYER : {
				return "BUYER";
				break;
			}
			case PersonType::Enum::DISPATCHER: {
				return "DISPATCHER";
				break;
			}
		}
	}
	PersonType();
	~PersonType();

};

