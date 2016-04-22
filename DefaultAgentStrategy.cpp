//
// Created by Nikki on 4/2/2016.
//
#include <vector>

#include "DefaultAgentStrategy.h"
#include "Game.h"

using std::vector;

namespace Gaming {

	DefaultAgentStrategy::DefaultAgentStrategy()
	{}

	DefaultAgentStrategy::~DefaultAgentStrategy()
	{}

	ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const
	{
		ActionType actionType = STAY;
		Position position1(1,1);
		vector<int> positionIndices (0);
		for(int i = 0; i < 9; ++i)
		{
			if(s.array[i] == PieceType::ADVANTAGE)
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
			if(s.array[i] == PieceType::FOOD)
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
			return actionType;
		}
		for(int i = 0; i < 9; ++i)
		{
			if(s.array[i] == PieceType::SIMPLE)
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