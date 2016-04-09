//
// Created by Nikki on 4/2/2016.
//

#include "DefaultAgentStrategy.h"

namespace Gaming {

	DefaultAgentStrategy::DefaultAgentStrategy()
	{}

	DefaultAgentStrategy::~DefaultAgentStrategy()
	{}

	ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const
	{
		ActionType __actionType = S;
		//get the surroundings
		//"randomly" select an ActionType based on surroundings
		return __actionType;//returns an ActionType
	}
}