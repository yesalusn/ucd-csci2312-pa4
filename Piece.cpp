//
// Created by Nikki on 4/2/2016.
//

#include "Piece.h"

namespace Gaming
{
	class Game;

	unsigned int Piece::__idGen = 0;

	Piece::Piece(const Game &g, const Position &p): __game(g), __position(p)
	{
		__id = this->__idGen++;
		__finished = false;
		__turned = false;
	}

	Piece::~Piece()
	{}

	std::ostream &operator<<(std::ostream& os, const Piece &piece)
	{
		piece.print(os);
		return os;
	}
}