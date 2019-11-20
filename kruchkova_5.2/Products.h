#include <string>
#pragma once

class Products
{
public:
	enum class Enum {
		Cutlet,
		Bun,
		Tomatoes,
		Dough,
		Sausage,
		Cheese
	};

	Products() {};
	~Products() {};

	static std::string toString(Enum myEnum) {
		switch (myEnum) {
			case Products::Enum::Cutlet: {
				return "Cutlet";
				break;
			}
			case Products::Enum::Bun: {
				return "Bun";
				break;
			}
			case Products::Enum::Tomatoes: {
				return "Tomatoes";
				break;
			}
			case Products::Enum::Dough: {
				return "Dough";
				break;
			}
			case Products::Enum::Sausage: {
				return "Sausage";
				break;
			}
			case Products::Enum::Cheese: {
				return "Cheese";
				break;
			}
		}
	}
};

