//
// Created by Nikki on 4/2/2016.
//

#include "Game.h"
#include "Resource.h"
#include "Advantage.h"

namespace Gaming
{
	const char Advantage::ADVANTAGE_ID = 'D';

	const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

	Advantage::Advantage(const Game &g, const Position &p, double capacity): Resource(g, p, __capacity)
	{
		__capacity = capacity;
	}

	Advantage::~Advantage()
	{}

	void Advantage::print(std::ostream &os) const
	{
		os << Advantage::ADVANTAGE_ID << Advantage::__id;
	}

	double Advantage::getCapacity() const
	{
		return this->__capacity;
	}

	double Advantage::consume()
	{
		return 0.0;
	}
}