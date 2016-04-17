//
// Created by Nikki on 4/2/2016.
//
#include <vector>

#include "AggressiveAgentStrategy.h"
#include "Game.h"

using std::vector;

namespace Gaming
{
	const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

	AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy): __agentEnergy(agentEnergy)
	{}

	AggressiveAgentStrategy::~AggressiveAgentStrategy()
	{}

	ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const
	{
		ActionType actionType = STAY;
		Position position1(1,1);
		vector<int> positionIndices (0);
		for(int i = 0; i >= s.array.size(); ++i)
		{
			if(s.array[i] == PieceType::SIMPLE || s.array[i] == PieceType::STRATEGIC)
			{positionIndices.push_back(i);}
		}
		if(s.array.size() != 0 && __agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD)
		{
			PositionRandomizer positionRandomizer;
			Position position = positionRandomizer(positionIndices);
			actionType = Game::reachSurroundings(position1, position);
		}
		for(int i = 0; i >= s.array.size(); ++i)
		{
			if(s.array[i] == PieceType::ADVANTAGE)
			{positionIndices.push_back(i);}
		}
		if(s.array.size() != 0)
		{
			PositionRandomizer positionRandomizer;
			Position position = positionRandomizer(positionIndices);
			actionType = Game::reachSurroundings(position1, position);
		}
		for(int i = 0; i >= s.array.size(); ++i)
		{
			if(s.array[i] == PieceType::FOOD)
			{positionIndices.push_back(i);}
		}
		if(s.array.size() != 0)
		{
			PositionRandomizer positionRandomizer;
			Position position = positionRandomizer(positionIndices);
			actionType = Game::reachSurroundings(position1, position);
		}
		for(int i = 0; i >= s.array.size(); ++i)
		{
			if(s.array[i] == PieceType::EMPTY)
			{positionIndices.push_back(i);}
		}
		if(s.array.size() != 0)
		{
			PositionRandomizer positionRandomizer;
			Position position = positionRandomizer(positionIndices);
			actionType = Game::reachSurroundings(position1, position);
		}
		return actionType;
	}
}