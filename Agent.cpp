//
// Created by Nikki on 4/2/2016.
//

#include "Agent.h"
#include "Resource.h"

namespace Gaming
{
	const double Agent::AGENT_FATIGUE_RATE = 0.30;

	Agent::Agent(const Game &g, const Position &p, double energy):
	Piece(g, p), __energy(energy)
	{}

	Agent::~Agent()
	{}

	void Agent::age()
	{
		__energy = __energy - AGENT_FATIGUE_RATE;

		if(__energy <= 0.3)
		{
			finish();
			__energy = 0.00;
		}
	}

	Piece &Agent::operator*(Piece &other)
	{
		Agent* a = dynamic_cast<Agent*>(&other);
		if(a)   interact(a);
		else
		{
			Resource* r = dynamic_cast<Resource*>(&other);
			interact(r);
		}
		return *this;
	}

	Piece &Agent::interact(Agent *agent)
	{
		if(getEnergy() == agent->getEnergy())
		{
			finish();
			agent->finish();
			__energy = 0.00;
			agent->__energy = 0.00;
		}
		else if(getEnergy() > agent->getEnergy())
		{
			__energy -= agent->__energy;
			agent->finish();
			agent->__energy = 0.00;
		}
		else
		{
			agent->__energy -= __energy;
			finish();
			__energy = 0.00;
		}
		return *this;
	}

	Piece &Agent::interact(Resource *resource)
	{
		addEnergy(resource->consume());
		return *this;
	}

}