//
// Created by Nikki on 4/2/2016.
//

#include <iomanip>

#include "Game.h"
#include "Exceptions.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"
#include "Gaming.h"

namespace Gaming
{
	//private consts
	const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
	const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;

	//public consts
	const unsigned Game::MIN_WIDTH = 3, Game::MIN_HEIGHT = 3;
	const double Game::STARTING_AGENT_ENERGY = 20;
	const double Game::STARTING_RESOURCE_CAPACITY = 10;

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
	Game::Game():__numInitAgents(NUM_INIT_AGENT_FACTOR),__numInitResources(NUM_INIT_RESOURCE_FACTOR),
	             __width(MIN_WIDTH), __height(MIN_HEIGHT), __grid(__width * __height, nullptr), __round(0),
	             __status(NOT_STARTED), __verbose(false)
	{}

	Game::Game(unsigned int width, unsigned int height, bool manual):
			__numInitAgents(NUM_INIT_AGENT_FACTOR), __numInitResources(NUM_INIT_RESOURCE_FACTOR),
			__round(0),__status(NOT_STARTED), __verbose(false)
	{
		if(width < MIN_WIDTH || height < MIN_HEIGHT)
			throw(InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height));
		else
		{
			__width = width;
			__height = height;
			__grid.resize(__width * __height, nullptr);
		}

		if(!(manual)){populate();}
	}

	//copy constructor
	Game::Game(const Game &another)
	{}

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
		unsigned int numPieces = getNumResources() + getNumAgents();

		return numPieces;
	}

	unsigned int Game::getNumAgents() const
	{
		unsigned int numAgents = getNumSimple() + getNumStrategic();

		return numAgents;
	}

	unsigned int Game::getNumSimple() const
	{
		unsigned int numAgents = 0;

		for (auto it = this->__grid.begin(); it != this->__grid.end(); ++it) {
			Agent *agent = dynamic_cast<Simple*>(*it);
			if (agent) numAgents ++;
		}

		return numAgents;
	}

	unsigned int Game::getNumStrategic() const
	{
		unsigned int numAgents = 0;

		for (auto it = this->__grid.begin(); it != this->__grid.end(); ++it) {
			Agent *agent = dynamic_cast<Strategic*>(*it);
			if (agent) numAgents ++;
		}

		return numAgents;
	}

	unsigned int Game::getNumResources() const
	{
		unsigned int numResources = 0;

		for (auto it = this->__grid.begin(); it != this->__grid.end(); ++it) {
			Resource *resource = dynamic_cast<Resource*>(*it);
			if (resource) numResources ++;
		}

		return numResources;
	}

	const Piece* Game::getPiece(unsigned int x, unsigned int y) const
	{
		Piece* p = nullptr;
		//TODO: goto x,y and find out if there is a Piece there
		//if yes, return Piece* with & of that <Piece>
		//if no, return nullptr (which is already done)

		return p;
	}

	//grid population mutators
	void Game::addSimple(const Position &position)
	{
		bool pieced = false;
		if(position.y < 0 || position.y >= this->__width || position.x < 0 || position.x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, position.y, position.x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (position.x == occupied.x && position.y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				this->__grid.push_back(new Simple(*this, position, Game::STARTING_AGENT_ENERGY));
				pieced = true;
			}
		}
	}

	void Game::addSimple(const Position &position, double energy)
	{
		addSimple(position);
		Simple *s = dynamic_cast<Simple*>(getPiece(position.x, position.y));
		s->addEnergy(energy);
	}

	void Game::addSimple(unsigned x, unsigned y)
	{
		bool pieced = false;
		if(y < 0 || y >= this->__width || x < 0 || x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, y, x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (x == occupied.x && y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				Position position(x,y);
				this->__grid.push_back(new Simple(*this, position, Game::STARTING_AGENT_ENERGY));
				pieced = true;
			}
		}
	}

	void Game::addSimple(unsigned x, unsigned y, double energy)
	{
		//TODO: addSimple ADD SIMPLE AGENT TO x,y WITH energy
	}

	void Game::addStrategic(const Position &position, Strategy *s)
	{
		bool pieced = false;
		if(position.y < 0 || position.y >= this->__width || position.x < 0 || position.x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, position.y, position.x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (position.x == occupied.x && position.y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				this->__grid.push_back(new Strategic(*this, position, Game::STARTING_AGENT_ENERGY));
				pieced = true;
			}
		}
	}

	void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
	{
		bool pieced = false;
		if(y < 0 || y >= this->__width || x < 0 || x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, y, x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (x == occupied.x && y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				Position position(x,y);
				this->__grid.push_back(new Strategic(*this, position, Game::STARTING_AGENT_ENERGY));
				pieced = true;
			}
		}
	}

	void Game::addFood(const Position &position)
	{
		bool pieced = false;
		if(position.y < 0 || position.y >= this->__width || position.x < 0 || position.x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, position.y, position.x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (position.x == occupied.x && position.y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				this->__grid.push_back(new Food(*this, position, Game::STARTING_RESOURCE_CAPACITY));
				pieced = true;
			}
		}
	}

	void Game::addFood(unsigned x, unsigned y)
	{
		bool pieced = false;
		if(y < 0 || y >= this->__width || x < 0 || x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, y, x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (x == occupied.x && y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				Position position(x,y);
				this->__grid.push_back(new Food(*this, position, Game::STARTING_RESOURCE_CAPACITY));
				pieced = true;
			}
		}
	}

	void Game::addAdvantage(const Position &position)
	{
		bool pieced = false;
		if(position.y < 0 || position.y >= this->__width || position.x < 0 || position.x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, position.y, position.x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (position.x == occupied.x && position.y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				this->__grid.push_back(new Advantage(*this, position, Game::STARTING_RESOURCE_CAPACITY));
				pieced = true;
			}
		}
	}

	void Game::addAdvantage(unsigned x, unsigned y)
	{
		bool pieced = false;
		if(y < 0 || y >= this->__width || x < 0 || x >= this->__height)
		{
			throw (OutOfBoundsEx(this->__width, this->__height, y, x));
		}
		else
		{
			bool taken = false;
			for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
			{
				if(*it != nullptr)
				{
					Position occupied = (*it)->getPosition();
					if (x == occupied.x && y == occupied.y)
					{ taken = true;}
				}
			}
			if(!(taken))
			{
				Position position(x,y);
				this->__grid.push_back(new Advantage(*this, position, Game::STARTING_RESOURCE_CAPACITY));
				pieced = true;
			}
		}
	}

	const Surroundings Game::getSurroundings(const Position &pos) const
	{
		Gaming::Surroundings surroundings;
		bool pieceFound = false;

		//make sure piece position is valid first
		for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
		{
			if(*it != nullptr)
			{
				Position ofPiece = (*it)->getPosition();
				if(pos.x == ofPiece.x && pos.y == ofPiece.y)
					pieceFound = true;
			}
		}

		//then construct surroundings
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

			for(int i = 0; i < surroundings.array.size(); ++i)
			{
				if(i == 4)
					surroundings.array[4] = Gaming::SELF;
				else if(posArray[i].y < 0 || posArray[i].y >= this->__width
				        || posArray[i].x < 0 || posArray[i].x >= this->__height)
				{
					surroundings.array[i] = Gaming::INACCESSIBLE;
				}
				else
				{
					for(auto it = this->__grid.begin(); it != this->__grid.end(); ++it)
					{
						if(*it != nullptr &&
						   (posArray[i].x == (*it)->getPosition().x && posArray[i].y == (*it)->getPosition().y))
						{
							surroundings.array[i] = (*it)->getType();
							break;
						}
						else
							surroundings.array[i] = Gaming::EMPTY;
					}
				}
			}
		}

		return surroundings;
	}

	//game play methods
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
	{}

	void Game::play(bool verbose)
	{}

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
							os << "[" << std::setw(6) << *(*it) << "]";
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