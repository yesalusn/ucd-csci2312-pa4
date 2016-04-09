//
// Created by Nikki on 4/2/2016.
//

#include "Food.h"

namespace Gaming
{
	const char Food::FOOD_ID = 'F';

	Food::Food(const Game &g, const Position &p, double capacity): Resource(g, p, __capacity)
	{
		__capacity = capacity;
	}

	Food::~Food()
	{}

	void Food::print(std::ostream &os) const
	{
		os << Food::FOOD_ID << Food::__id;
	}
}