#include <string>
#pragma once

class PersonType
{
public:
	enum class Enum
	{
		BUYER,
		DISPATCHER,
		COURIER,
		COOK,
		FRIDGE,
		SHOP
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
			case PersonType::Enum::COURIER: {
				return "COURIER";
				break;
			}
			case PersonType::Enum::COOK: {
				return "COOK";
				break;
			}
			case PersonType::Enum::FRIDGE: {
				return "FRIDGE";
				break;
			}
			case PersonType::Enum::SHOP: {
				return "SHOP";
				break;
			}
		}
	}
	PersonType();
	~PersonType();

};

