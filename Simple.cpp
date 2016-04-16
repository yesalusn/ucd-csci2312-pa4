//
// Created by Nikki on 4/2/2016.
//
#include <sstream>

#include "Simple.h"

using std::stringstream;

namespace Gaming
{
	const char Simple::SIMPLE_ID = 'S';

	Simple::Simple(const Game &g, const Position &p, double energy): Agent(g, p, energy)
	{}

	Simple::~Simple()
	{}

	void Simple::print(std::ostream &os) const
	{
		stringstream ss;
		ss << SIMPLE_ID << __id;
		os << ss.str();
	}

	ActionType Simple::takeTurn(const Surroundings &s) const
	{
		return S;
	}
}