//
// Created by Nikki on 4/2/2016.
//
#include <sstream>
#include <vector>

#include "Simple.h"

using std::stringstream;
using std::vector;

namespace Gaming
{

	const char Simple::SIMPLE_ID = 'S';

	Simple::Simple(const Game &g, const Position &p, double energy):
	Agent(g, p, energy)
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
		ActionType actionType = STAY;
		Position position1(1,1);
		vector<int> positionIndices (0);
		for(int i = 0; i < 9; ++i)
		{
			if(s.array[i] == PieceType::ADVANTAGE || s.array[i] == PieceType::FOOD)
			{positionIndices.push_back(i);}
		}
		if(positionIndices.size() != 0)
		{
			PositionRandomizer positionRandomizer;
			Position position = positionRandomizer(positionIndices);
			actionType = Game::reachSurroundings(position1, position);
			return actionType;
		}
		for(int i = 0; i < 9; ++i)
		{
			if(s.array[i] == PieceType::EMPTY)
			{positionIndices.push_back(i);}
		}
		if(positionIndices.size() != 0)
		{
			PositionRandomizer positionRandomizer;
			Position position = positionRandomizer(positionIndices);
			actionType = Game::reachSurroundings(position1, position);
		}
		return actionType;

	}
}