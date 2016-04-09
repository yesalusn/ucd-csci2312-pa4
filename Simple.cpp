//
// Created by Nikki on 4/2/2016.
//

#include "Simple.h"

namespace Gaming
{
	const char Simple::SIMPLE_ID = 'S';

	Simple::Simple(const Game &g, const Position &p, double energy): Agent(g, p, energy)
	{
		__energy = energy;
	}

	Simple::~Simple()
	{}

	void Simple::print(std::ostream &os) const
	{
		os << Simple::SIMPLE_ID + Simple::__id;
	}

	ActionType Simple::takeTurn(const Surroundings &s) const
	{
		return S;
	}
}