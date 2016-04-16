//
// Created by Nikki on 4/2/2016.
//
#include <sstream>

#include "Food.h"

using std::stringstream;

namespace Gaming
{
	const char Food::FOOD_ID = 'F';

	Food::Food(const Game &g, const Position &p, double capacity): Resource(g, p, capacity)
	{}

	Food::~Food()
	{}

	void Food::print(std::ostream &os) const
	{
		stringstream ss;
		ss << FOOD_ID << __id;
		os << ss.str();
	}
}