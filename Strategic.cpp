//
// Created by Nikki on 4/2/2016.
//
#include <sstream>

#include "Strategic.h"

using std::stringstream;

namespace Gaming
{
	class Game;

	const char Strategic::STRATEGIC_ID = 'T';

	Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s): Agent(g, p, energy)
	{}

	Strategic::~Strategic()
	{}

	void Strategic::print(std::ostream &os) const
	{
		stringstream ss;
		ss << STRATEGIC_ID << __id;
		os << ss.str();
	}

	ActionType Strategic::takeTurn(const Surroundings &s) const
	{
		return S;
	}
}