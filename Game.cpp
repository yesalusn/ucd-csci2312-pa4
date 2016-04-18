//
// Created by Nikki on 4/2/2016.
//

#include <iomanip>
#include <vector>
#include <set>

#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"

using std::array;
using std::vector;

namespace Gaming
{
	int grid_converter(const Game g, const Position& p)
	{
		int i = (g.getWidth() * p.x) + p.y;
		return i;
	}

	//private consts
	const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
	const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;

	//public consts
	const unsigned Game::MIN_WIDTH = 3, Game::MIN_HEIGHT = 3;
	const double Game::STARTING_AGENT_ENERGY = 20;
	const double Game::STARTING_RESOURCE_CAPACITY = 10;

	PositionRandomizer Game::__posRandomizer;

	//private method to populate the grid
	void Game::populate() // populate the grid (used in automatic random initialization of a Game)
	{
		__numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
		__numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;

		unsigned int numStrategic = __numInitAgents / 2;
		unsigned int numSimple = __numInitAgents - numStrategic;
		unsigned int numAdvantages = __numInitResources / 4;
		unsigned int numFoods = __numInitResources - numAdvantages;

		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, __width * __height);

		//populate Agents
		//populate Strategic Agents
		while(numStrategic > 0)
		{
			int i = d(gen); // random index in the grid vector
			if (__grid[i] == nullptr) // is position empty
			{
				Position pos(i / __width, i % __width);
				__grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
				numStrategic --;
			}
		}

		//populate Simple Agents
		while(numSimple > 0)
		{
			int i = d(gen);
			if(__grid[i] == nullptr)
			{
				Position pos(i / __width, i % __width);
				__grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
				numSimple--;
			}
		}

		//populate Resources
		//populate Advantages
		while(numAdvantages > 0)
		{
			int i = d(gen);
			if(__grid[i] == nullptr)
			{
				Position pos(i / __width, i % __width);
				__grid[i] = new Advantage(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
				numAdvantages--;
			}
		}

		//populate Foods
		while(numFoods > 0)
		{
			int i = d(gen);
			if(__grid[i] == nullptr)
			{
				Position pos(i / __width, i % __width);
				__grid[i] = new Food(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
				numFoods--;
			}
		}
	}

	//default and three argument constructors
	Game::Game():__numInitAgents(0),__numInitResources(0),
	             __width(MIN_WIDTH), __height(MIN_HEIGHT), __grid(__width * __height, nullptr), __round(0),
	             __status(NOT_STARTED), __verbose(false)
	{}

	Game::Game(unsigned int width, unsigned int height, bool manual):
			__numInitAgents(0), __numInitResources(0), __width(width), __height(height),
			__grid(__width * __height, nullptr), __round(0),__status(NOT_STARTED), __verbose(false)
	{
		if(width < MIN_WIDTH || height < MIN_HEIGHT)
			throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
		__width = width;
		__height = height;
		__grid.resize(__width * __height, nullptr);
		if(!(manual))   populate();
	}

	//copy constructor
	Game::Game(const Game &another):
	__numInitAgents(another.getNumAgents()), __numInitResources(another.getNumResources()), __width(another.__width),
	__height(another.__height), __grid(__width * __height, nullptr), __round(another.__round),
	__status(another.__status), __verbose(another.__verbose)
	{
		for(int i = 0; i <= another.__grid.size(); ++i)
		{
			__grid[i] = another.__grid[i];
		}
	}

	//destructor
	Game::~Game()
	{
		if(__grid.size() > 0)
		{
			for(int i = 0; i < __grid.size(); ++i)
			{
				__grid[i] = nullptr;
			}
		}
	}

	//accessors
	unsigned int Game::getNumPieces() const
	{
		unsigned int numPieces = this->getNumResources() + this->getNumAgents();
		return numPieces;
	}

	unsigned int Game::getNumAgents() const
	{
		unsigned int numAgents = this->getNumSimple() + this->getNumStrategic();
		return numAgents;
	}

	unsigned int Game::getNumSimple() const
	{
		unsigned int numSimple = 0;
		for(auto it = __grid.begin(); it != __grid.end(); ++it)
		{
			if(*it != nullptr && (*it)->getType() == PieceType::SIMPLE)
			{
				Simple* simple = dynamic_cast<Simple *>(*it);
				if(simple)   ++numSimple;
			}
		}
		return numSimple;
	}

	unsigned int Game::getNumStrategic() const
	{
		unsigned int numStrategic = 0;
		for(auto it = __grid.begin(); it != __grid.end(); ++it)
		{
			if(*it != nullptr && (*it)->getType() == PieceType::STRATEGIC)
			{
				Strategic* strategic = dynamic_cast<Strategic *>(*it);
				if(strategic)   ++numStrategic;
			}
		}
		return numStrategic;
	}

	unsigned int Game::getNumResources() const
	{
		unsigned int numResources = 0;
		for(auto it = __grid.begin(); it != __grid.end(); ++it)
		{
			if(*it != nullptr && ((*it)->getType() == PieceType::FOOD || (*it)->getType() == PieceType::ADVANTAGE))
			{
				Resource* resource = dynamic_cast<Resource*>(*it);
				if(resource)    ++ numResources;
			}
		}
		return numResources;
	}

	const Piece* Game::getPiece(unsigned int x, unsigned int y) const
	{
		if(y < 0 || y >= __width || x < 0 || x >= __height)
			throw OutOfBoundsEx(__width, __height, y, x);
		Piece* p = nullptr;
		for(auto it = __grid.begin(); it != __grid.end(); ++it)
		{
			if(*it != nullptr && (*it)->getPosition().x == x && (*it)->getPosition().y == y)
				p = *it;
		}
		return p;
	}

	//grid population mutators
	void Game::addSimple(const Position &position)
	{
		if(position.y < 0 || position.y >= __width || position.x < 0 || position.x >= __height)
			throw OutOfBoundsEx(__width, __height, position.y, position.x);
		if(__grid[grid_converter(*this, position)] == nullptr)
		{
			__grid[grid_converter(*this, position)] = new Simple(*this, position, Game::STARTING_AGENT_ENERGY);
			++__numInitAgents;
		}
	}

	void Game::addSimple(const Position &position, double energy)
	{
		int grid_size = __grid.size();
		addSimple(position);
		if(grid_size != __grid.size())
		{
			Agent* agent = dynamic_cast<Agent*>(__grid[grid_converter(*this, position)]);
			if(agent)
			{
				double i = agent->getEnergy();
				agent->addEnergy(energy - i);
			}
		}
	}

	void Game::addSimple(unsigned x, unsigned y)
	{
		Position p(x,y);
		addSimple(p);
	}

	void Game::addSimple(unsigned x, unsigned y, double energy)
	{
		Position p(x,y);
		addSimple(p, energy);
	}

	void Game::addStrategic(const Position &position, Strategy *s)
	{
		if(position.y < 0 || position.y >= __width || position.x < 0 || position.x >= __height)
			throw OutOfBoundsEx(__width, __height, position.y, position.x);
		if(__grid[grid_converter(*this, position)] == nullptr)
		{
			if (s != nullptr)
				__grid[grid_converter(*this, position)] = new Strategic(*this, position, Game::STARTING_AGENT_ENERGY,
				                                                        s);
			else
				__grid[grid_converter(*this, position)] = new Strategic(*this, position, Game::STARTING_AGENT_ENERGY);
			++__numInitAgents;
		}

	}

	void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
	{
		Position p(x,y);
		if(s != nullptr)
			addStrategic(p, s);
		else
			addStrategic(p);
	}

	void Game::addFood(const Position &position)
	{
		if(position.y < 0 || position.y >= __width || position.x < 0 || position.x >= __height)
			throw OutOfBoundsEx(__width, __height, position.y, position.x);
		if(__grid[grid_converter(*this, position)] == nullptr)
		{
			__grid[grid_converter(*this, position)] = new Food(*this, position, Game::STARTING_RESOURCE_CAPACITY);
			++__numInitResources;
		}
	}

	void Game::addFood(unsigned x, unsigned y)
	{
		Position p(x,y);
		addFood(p);
	}

	void Game::addAdvantage(const Position &position)
	{
		if(position.y < 0 || position.y >= __width || position.x < 0 || position.x >= __height)
			throw OutOfBoundsEx(__width, __height, position.y, position.x);
		if(__grid[grid_converter(*this, position)] == nullptr)
		{
			__grid[grid_converter(*this, position)] = new Advantage(*this, position, Game::STARTING_RESOURCE_CAPACITY);
			++__numInitResources;
		}
	}

	void Game::addAdvantage(unsigned x, unsigned y)
	{
		Position p(x,y);
		addAdvantage(p);
	}

	const Surroundings Game::getSurroundings(const Position &pos) const
	{
		Gaming::Surroundings surroundings;
		bool pieceFound = false;
		if(pos.y < 0 || pos.y >= __width || pos.x < 0 || pos.x >= __height)
			throw OutOfBoundsEx(__width, __height, pos.y, pos.x);
		for(auto it = __grid.begin(); it != __grid.end(); ++it)
		{
			if(*it != nullptr)
			{
				Position ofPiece = (*it)->getPosition();
				if(pos.x == ofPiece.x && pos.y == ofPiece.y)
					pieceFound = true;
			}
		}
		if(pieceFound)
		{
			int k = 0;
			std::array<Position, 9> posArray;
			for (int i = -1; i < 2; ++i)
			{
				for (int j = -1; j < 2; ++j)
				{
					posArray[k] = Position(pos.x + i, pos.y + j);
					k++;
				}
			}
			for(int i = 0; i < posArray.size(); ++i)
			{
				if(posArray[i].y < 0 || posArray[i].y >= __width || posArray[i].x < 0 || posArray[i].x >= __height)
					surroundings.array[i] = Gaming::INACCESSIBLE;
				else
				{
					for (auto it = __grid.begin(); it != __grid.end(); ++it)
					{
						if (*it != nullptr &&
						    (*it)->getPosition().x == posArray[i].x && (*it)->getPosition().y == posArray[i].y)
							surroundings.array[i] = (*it)->getType();
					}
				}
			}
			surroundings.array[4] = Gaming::SELF;
			int i = 0;
			do
			{
				if(surroundings.array[i] < 0 || surroundings.array[i] > 5)
					surroundings.array[i] = Gaming::EMPTY;
				++i;
			}while(!(surroundings.array.end()));
			if(surroundings.array[i] < 0 || surroundings.array[i] > 5)
				surroundings.array[i] = Gaming::EMPTY;
		}

		return surroundings;
	}

	const ActionType Game::reachSurroundings(const Position &from, const Position &to)
	{
		ActionType ac;

		unsigned int x = to.x - from.x;
		unsigned int y = to.y - from.y;

		if(x == -1 && y == -1)
		{ ac = NW;}
		else if(x == -1 && y == 0)
		{ ac = N;}
		else if(x == -1 && y == 1)
		{ ac = NE;}
		else if(x == 0 && y == -1)
		{ ac = W;}
		else if(x == 0 && y == 1)
		{ ac = E;}
		else if(x == 1 && y == -1)
		{ ac = SW;}
		else if(x == 1 && y == 0)
		{ ac = S;}
		else if(x == 1 && y == 1)
		{ ac = SE;}
		else
			ac = STAY;

		return ac;
	}

	bool Game::isLegal(const ActionType &ac, const Position &pos) const
	{
		bool __isLegal = false;
		Position newPos(pos);
		switch(ac)
		{
			case N: newPos.x -= 1;
				if(!(newPos.x < 0)){__isLegal = true;}
				break;
			case NE: newPos.x -= 1;
				newPos.y += 1;
				if((!(newPos.x < 0)) && (!(newPos.y >= __width))){__isLegal = true;}
				break;
			case NW: newPos.x -= 1;
				newPos.y-= 1;
				if((!(newPos.x < 0)) && (!(newPos.y < 0))){__isLegal = true;}
				break;
			case E: newPos.y += 1;
				if(!(newPos.y >= __width)){__isLegal = true;}
				break;
			case W: newPos.y -= 1;
				if(!(newPos.y < 0)){__isLegal = true;}
				break;
			case SE:newPos.x += 1;
				newPos.y += 1;
				if((!(newPos.x >= __height)) && (!(newPos.y >= __width))){__isLegal = true;}
				break;
			case SW: newPos.x += 1;
				newPos.y -= 1;
				if((!(newPos.x >= __height)) && (!(newPos.y < 0))){__isLegal = true;}
				break;
			case S: newPos.x += 1;
				if(!(newPos.x >= __height)){__isLegal = true;}
				break;
			case STAY: __isLegal = true;
				break;
			default: std::cout << "something wrong happened here" << std::endl;
		}
		return __isLegal;
	}

	const Position Game::move(const Position &pos, const ActionType &ac) const
	{
		Position other(pos.x, pos.y);
		switch(ac)
		{
			case N: other.x -= 1;
				break;
			case NE: other.x -= 1;
				other.y += 1;
				break;
			case NW: other.x -= 1;
				other.y -= 1;
				break;
			case E: other.y += 1;
				break;
			case W: other.y -= 1;
				break;
			case SE: other.x += 1;
				other.y += 1;
				break;
			case SW: other.x += 1;
				other.y -= 1;
				break;
			case S: other.x += 1;
				break;
			default: std::cout << "no other options available" << std::endl;
		}
		return other;
	}

	void Game::round()
	{
	//  1. Go through all the Piece-s that are still viable and on the grid (this doesn't change between rounds) and:
	//      i. If a Piece has not taken a turn, give it a turn. Use Piece::getTurned().
	//      ii. Call the Piece::setTurned(true) to avoid giving a moving Piece more than one turn per round
	//          (e.g. if it happens to move to a grid position you haven't covered in the current round).
	//      iii. Perform the turn and implement all the consequences of the turn (e.g. interaction with another Piece).
	//      iv. Delete any Piece(s) which became unviable.
	//  2. Go through all the Piece-s that are still viable and on the grid after the current round, and:
	//      i. Call the polymorphic Piece::age().
	//      ii. Call the Piece::setTurned(false) to reset the turn for the next round.
	//      iii. Delete any Piece(s) which have aged to zero.
	//TODO
	//				Some guidelines for implementing Game::round():
	//  1. Do it incrementally and use the tests to build in more and more detail.
	//  2. It might be intuitive that you cycle through the Game grid for each round, but it is much better to
	//      cycle through a std::set of all the currently occupied positions of the grid. Cycling through the grid won't be able
	//      to handle some of the finer cases. Cycling through the set, if done correctly, will ensure a fair turn for all the
	//      Pieces.
	//  3. Notice that a Piece might become unviable before its turn comes (e.g. a Resource gets consumed by an Agent that
	//      moves ahead of it, or an Agent gets challenged by another, and one or both die as a result).
	//  4. Notice that a Piece might move to a new position, either through an interaction or through a move to a
	//      previously free position. In the first case, the position was already occupied, so it is still in the set. In the
	//      second case, the position was empty, so needs to be added to the set.

		std::set<int> piece_positions;
		for(auto it = __grid.begin(); it != __grid.end(); ++it)
		{
			if(*it != nullptr && (*it)->isViable())
				piece_positions.insert(grid_converter(*this, (*it)->getPosition()));
		}
		for(auto it =  piece_positions.begin(); it != piece_positions.end(); ++it)
		{
			if(!(__grid[(*it)]->getTurned()))
			{
				__grid[(*it)]->setTurned(true);
				Position p = __grid[(*it)]->getPosition();
				Surroundings s = getSurroundings(p);
				ActionType ac = __grid[(*it)]->takeTurn(s);
				if(isLegal(ac,p))
				{
					__grid[(*it)]->setPosition(move(p,ac));
				}
			}
		}
	}

	void Game::play(bool verbose)
	{
		//if(verbose)   //print the game board
	}

	std::ostream &operator<<(std::ostream &os, const Game &game)
	{

		os << "Round " << game.getRound() << "/n";
		for(int i = 0; i < game.__height; ++i)
		{
			for (int j = 0; j < game.__width; ++j)
			{
				for(auto it = game.__grid.begin(); it != game.__grid.end(); ++it)
				{
					if(*it != nullptr)
					{
						Position pos(i, j);
						Position piecePos = (*it)->getPosition();

						if (piecePos.x == pos.x && piecePos.y == pos.y)
						{
							os << "[" << std::setw(6) << (*it) << "]";
						}
					}
					else
						os << "[" << std::string(6, ' ') << "]";
				}
			}

			os << "/n";
		}

		switch(game.getStatus())
		{
			case Game::NOT_STARTED: os << "Not Started./n";
				break;
			case Game::PLAYING: os << "Playing.../n";
				break;
			case Game::OVER: os << "Over!/n";
				break;
			default: os << "Something really wrong must've happened to get here.../n";
		}

		return os;
	}
}