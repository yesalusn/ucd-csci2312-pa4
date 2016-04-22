//
// Created by Nikki on 4/2/2016.
//

#include "Resource.h"
#include "Agent.h"

namespace Gaming
{
	const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

	Resource::Resource(const Game &g, const Position &p, double __capacity):
	Piece(g, p), __capacity(__capacity)
	{}

	Resource::~Resource()
	{}

	double Resource::consume()
	{
		double capacity = __capacity;
		__capacity = 0.00;
		finish();
		return capacity;
	}

	void Resource::age()
	{
		__capacity =  __capacity / RESOURCE_SPOIL_FACTOR;

		if(__capacity < 0.01)
		{
			finish();
			__capacity = 0.00;
		}
	}

	ActionType Resource::takeTurn(const Surroundings &s) const
	{
		return Gaming::STAY;
	}

	Piece &Resource::operator*(Piece &other)
	{
		Agent* a = dynamic_cast<Agent*>(&other);
		if(a)   interact(a);
		else
		{
			//this will not get used, but it's necessary
			Resource *r = dynamic_cast<Resource *>(&other);
			interact(r);
		}
		return *this;
	}

	Piece &Resource::interact(Agent *agent)
	{
		//resource will be consumed
		agent->addEnergy(consume());
		return *this;
	}

	Piece &Resource::interact(Resource *resource)
	{
		//resources do not interact
		return *this;
	}
}