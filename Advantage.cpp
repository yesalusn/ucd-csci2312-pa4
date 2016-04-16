//
// Created by Nikki on 4/2/2016.
//

#include <sstream>

#include "Game.h"
#include "Resource.h"
#include "Advantage.h"

using std::stringstream;

namespace Gaming
{
	const char Advantage::ADVANTAGE_ID = 'D';

	const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

	Advantage::Advantage(const Game &g, const Position &p, double capacity):
	Resource(g, p, (capacity * ADVANTAGE_MULT_FACTOR))
	{}

	Advantage::~Advantage()
	{}

	void Advantage::print(std::ostream &os) const
	{
		stringstream ss;
		ss << ADVANTAGE_ID << __id;
		os << ss.str();
	}

	double Advantage::getCapacity() const
	{
		return __capacity;
	}

	double Advantage::consume()
	{
		return 0.0;
	}
}