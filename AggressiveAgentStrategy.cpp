//
// Created by Nikki on 4/2/2016.
//

#include "AggressiveAgentStrategy.h"
#include "Game.h"

namespace Gaming
{
	const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

	AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy)
	{ }


	AggressiveAgentStrategy::~AggressiveAgentStrategy()
	{

	}

	ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const
	{
		return S;
	}
}