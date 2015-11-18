#include <random>
#include "Characters.h"
#include "Scenario.h"
#include "../Game/Game.h"

#define Scenario        CGame::Instance().Scenario()
#define ScenarioData    Scenario.Data()
#define Clip(type)      &ScenarioData.Clippings(type)
#define Grid            CGridWithWeights::Instance()
#define Pacman          CGame::Instance().Pacman()

namespace gamecore
{
	// set up the directions
	std::array<CSquareGrid::Location, 4> CSquareGrid::DIRS{ { Location{ 1, 0 }, Location{ 0, -1 }, Location{ -1, 0 }, Location{ 0, 1 } } };

	void CCharacter::Render()
	{
		using graphicscore::SIZE_OF_TILE;
		using graphicscore::SIZE_OF_CHARACTER;

		// the center of a character is located in a different point that the start of the sprite
		// texture, the sprite size is 33x33 pixels and the tile size is 18x18, the coordinates to be
		// sent to the screen are different that the ones hold inside of the object, they referred
		// to a 28x36 tiles scenario
		ScenarioData.RenderTexture((_x * SIZE_OF_TILE) - 1 - (SIZE_OF_CHARACTER - SIZE_OF_TILE) / 2,
			(_y * SIZE_OF_TILE) - (SIZE_OF_CHARACTER - SIZE_OF_TILE) / 2,
			Clip(_c));
	}

	void CPacman::Move()
	{
		// if Pacman has reached either the left side or right side of the wrap zone
		// change its location to the opposite location
		if (_y == 17)
		{
			if (_x == 0)
				_x = 27;
			else if (_x == 27)
				_x = 0;
		}

		// move in the indicated position
		switch (d)
		{
		case Left:  if (Scenario.ElementsMatrix()[_x - 1][_y] != Wall) --_x; break;
		case Right: if (Scenario.ElementsMatrix()[_x + 1][_y] != Wall) ++_x; break;
		case Up:    if (Scenario.ElementsMatrix()[_x][_y - 1] != Wall) --_y; break;
		case Down:  if (Scenario.ElementsMatrix()[_x][_y + 1] != Wall) ++_y; break;
		default:    break;
		}

		// remove any decimal point from the position before moving Pacman
		_x = static_cast<int>(_x);
		_y = static_cast<int>(_y);

		// if there's a pill in the position, take it and sum the value in the score
		if (Scenario.ElementsMatrix()[_x][_y] == Pill)
		{
			// TODO: Add the pill to the score
			Scenario.ElementsMatrix(_x, _y, Empty);
		}
		// if there was an energizer, change 
		else if (Scenario.ElementsMatrix()[_x][_y] == Energizer)
		{
			// TODO: Implement the energizer behavior
			Scenario.ElementsMatrix(_x, _y, Empty);
		}
	}

	bool CGhost::Init()
	{
		// set the initial position of the ghost
		if (_g == Red)
		{
			Position(13.5, 14);
			Clipping(graphicscore::LR_GHOST);
		}
		else if (_g == Pink)
		{
			Position(11.5, 17);
			Clipping(graphicscore::LP_GHOST);
		}
		else if (_g == Blue)
		{
			Position(13.5, 17);
			Clipping(graphicscore::LB_GHOST);
		}
		else
		{
			Position(15.5, 17);
			Clipping(graphicscore::LO_GHOST);
		}

		return true;
	}

	void CGhost::Move()
	{
		/* move the ghost depending on the color */
		// the red ghost moves using the bread first search algorithm
		if (_g == Red)
		{
			auto path = Grid.ReconstructPath(Position(), Pacman.Position(), Grid.BreadthFirstSearch(Grid, Position(), Pacman.Position()));
			if (path.size() != 0)
				Position(path.back());
		}
		// the pink ghost moves using the Djikstra search algorithm
		else if (_g == Pink)
		{
			std::unordered_map<CSquareGrid::Location, CSquareGrid::Location> came_from;
			std::unordered_map<CSquareGrid::Location, int> cost_so_far;
			Grid.DijkstraSearch(Grid, Position(), Pacman.Position(), came_from, cost_so_far);
			auto path = Grid.ReconstructPath(Position(), Pacman.Position(), came_from);
			if (path.size() != 0)
				Position(path.back());
		}
		// the blue ghost moves using the A* search algorithm
		else if (_g == Blue)
		{
			std::unordered_map<CSquareGrid::Location, CSquareGrid::Location> came_from;
			std::unordered_map<CSquareGrid::Location, int> cost_so_far;
			Grid.AStarSearch(Grid, Position(), Pacman.Position(), came_from, cost_so_far);
			auto path = Grid.ReconstructPath(Position(), Pacman.Position(), came_from);
			if (path.size() != 0)
				Position(path.back());
		}
		// the orange ghost moves using random movements, just for fun
		else
		{
			if (Position() != Pacman.Position())
			{
				typedef CGridWithWeights::Location Location;
				auto die = std::bind(std::uniform_int_distribution < > {0, 3}, std::default_random_engine{ std::random_device{}() });
				int direction = die();
				auto position = Position();
				Location next;
				switch (static_cast<Direction>(direction))
				{
				case Left:
					next = Location{ --std::get<0>(position), std::get<1>(position) };
					if (Grid.Passable(next))
						Position(next);
					break;
				case Right:
					next = Location{ ++std::get<0>(position), std::get<1>(position) };
					if (Grid.Passable(next))
						Position(next);
					break;
				case Up:
					next = Location{ std::get<0>(position), --std::get<1>(position) };
					if (Grid.Passable(next))
						Position(next);
					break;
					// Orange
				default:
					next = Location{ std::get<0>(position), ++std::get<1>(position) };
					if (Grid.Passable(next))
						Position(next);
					break;
				}
			}
		}
	}

	void CGhost::Reset()
	{
		// reset the initial position of the ghosts
		if (_g == Red)
			Position(13.5, 14);
		else if (_g == Pink)
			Position(11.5, 17);
		else if (_g == Blue)
			Position(13.5, 17);
		else
			Position(15.5, 17);
	}
}
