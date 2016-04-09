//
// Created by Nikki on 4/2/2016.
//

#include "Strategic.h"

namespace Gaming
{
	class Game;

	const char Strategic::STRATEGIC_ID = 'T';

	Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s): Agent(g, p, energy)
	{
		__energy = energy;
	}

	Strategic::~Strategic()
	{}

	void Strategic::print(std::ostream &os) const
	{
		os << Strategic::STRATEGIC_ID << Strategic::__id;
	}

	ActionType Strategic::takeTurn(const Surroundings &s) const
	{
		return S;
	}
}