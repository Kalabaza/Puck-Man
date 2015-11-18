#include <fstream>
#include "Scenario.h"
#include "../Graphics/Graphics.h"

#define Clip(type)      _data.Clippings((graphicscore::Clipping)type)
#define Graphics        graphicscore::CGraphics::Instance()
#define Grid            CGridWithWeights::Instance()

namespace gamecore
{
	bool CScenario::AddSpritesFile(std::string filename, Elements element)
	{
		// add walls to the scenario
		std::ifstream input{ filename, std::ifstream::in };

		// check if the file was opened correctly
		if (!input.is_open())
		{
#if DEBUG_CONSOLE
			std::cout << "ERROR: Unable to load input sprite file: " << filename << std::endl;
#endif
			return false;
		}

		GLfloat x, y;
		int c;
		while (!input.eof())
		{
			input >> x;  input.ignore(1, ',');
			input >> y;  input.ignore(1, ',');
			input >> c;  input.ignore(1, ',');
			// add the wall to the sprites map of the scenario
			_spritesMap[std::make_pair(x, y)] = Clip(c);
			// add the position to the elements matrix
			_elementsMatrix[x][y] = element;
		}
		input.close();

		return true;
	}

	bool CScenario::Init()
	{
		using graphicscore::SIZE_OF_TILE;

		// add wall locations to the scenario map
		AddSpritesFile("Scenarios/Scenario.txt", Wall);

		// add pill locations to the scenario map
		AddSpritesFile("Scenarios/Pills.txt", Pill);

		// fill the grid with the data about the scenario
		Grid.Instance().PopulateGrid();

		return true;
	}

	void CScenario::RenderMap()
	{
		// add walls and pills
		using graphicscore::SIZE_OF_TILE;
		for (auto sprite : _spritesMap)
		{
			if (_elementsMatrix[sprite.first.first][sprite.first.second] != Empty)
				_data.RenderTexture(sprite.first.first * SIZE_OF_TILE - 1, sprite.first.second * SIZE_OF_TILE, &sprite.second);
		}

		// add special zones to the scenario
		for (auto zone : Grid.Ground())
			Graphics.PaintSquare(zone, Color::ORANGE);
		for (auto zone : Grid.Grass())
			Graphics.PaintSquare(zone, Color::GREEN);
		for (auto zone : Grid.Water())
			Graphics.PaintSquare(zone, Color::BLUE);
	}

	CGridWithWeights& CGridWithWeights::Instance()
	{
		static CGridWithWeights grid{ SCREEN_WIDTH_TILES, SCREEN_HEIGHT_TILES };
		return grid;
	}

	void CGridWithWeights::AddRect(CSquareGrid& grid, int x1, int y1, int x2, int y2) {
		for (int x = x1; x <= x2; ++x) {
			for (int y = y1; y <= y2; ++y) {
				grid._walls.insert(CSquareGrid::Location{ x, y });
			}
		}
	}

	void CGridWithWeights::PopulateGrid() {
		// add walls to the grid
		AddRect(Grid, 0, 3, 27, 3);
		AddRect(Grid, 0, 4, 0, 11);
		AddRect(Grid, 2, 5, 5, 7); AddRect(Grid, 7, 5, 11, 7); AddRect(Grid, 13, 4, 14, 7); AddRect(Grid, 16, 5, 20, 7); AddRect(Grid, 22, 5, 25, 7);
		AddRect(Grid, 27, 4, 27, 11);
		AddRect(Grid, 2, 9, 5, 10);
		AddRect(Grid, 7, 9, 8, 16);
		AddRect(Grid, 10, 9, 17, 10);
		AddRect(Grid, 19, 9, 20, 16);
		AddRect(Grid, 22, 9, 25, 10);
		AddRect(Grid, 0, 12, 5, 16);
		AddRect(Grid, 9, 12, 11, 13); AddRect(Grid, 13, 11, 14, 13); AddRect(Grid, 16, 12, 18, 13);
		AddRect(Grid, 22, 12, 27, 16);
		AddRect(Grid, 0, 18, 5, 22); AddRect(Grid, 7, 18, 8, 22); AddRect(Grid, 10, 21, 17, 22); AddRect(Grid, 19, 18, 20, 22); AddRect(Grid, 22, 18, 27, 22);
		AddRect(Grid, 0, 23, 0, 32);
		AddRect(Grid, 2, 24, 5, 25); AddRect(Grid, 7, 24, 11, 25); AddRect(Grid, 13, 23, 14, 25); AddRect(Grid, 16, 24, 20, 25); AddRect(Grid, 22, 24, 25, 25);
		AddRect(Grid, 27, 23, 27, 32);
		AddRect(Grid, 4, 26, 5, 28); AddRect(Grid, 22, 26, 23, 28);
		AddRect(Grid, 1, 27, 2, 28);
		AddRect(Grid, 7, 27, 8, 29);
		AddRect(Grid, 10, 27, 17, 28);
		AddRect(Grid, 19, 27, 20, 29);
		AddRect(Grid, 25, 27, 26, 28);
		AddRect(Grid, 2, 30, 11, 31); AddRect(Grid, 13, 29, 14, 31); AddRect(Grid, 16, 30, 25, 31);
		AddRect(Grid, 0, 33, 27, 33);
		// ghosts home square zone
		AddRect(Grid, 10, 15, 12, 16); AddRect(Grid, 15, 15, 17, 16); AddRect(Grid, 10, 17, 10, 17); AddRect(Grid, 17, 17, 17, 17); AddRect(Grid, 10, 18, 17, 19);

		typedef CSquareGrid::Location L;

		/* special zones on the grid */
		// add ground zones to the grid
		Grid.Ground(std::unordered_set<CSquareGrid::Location>
		{
			L{ 6, 12 }, L{ 6, 13 }, L{ 6, 14 }, L{ 6, 15 }, L{ 6, 16 }, L{ 6, 17 }, L{ 6, 18 }, L{ 6, 19 }, L{ 6, 20 }, L{ 6, 21 }, L{ 6, 22 },
				L{ 21, 12 }, L{ 21, 13 }, L{ 21, 14 }, L{ 21, 15 }, L{ 21, 16 }, L{ 21, 17 }, L{ 21, 18 }, L{ 21, 19 }, L{ 21, 20 }, L{ 21, 21 }, L{ 21, 22 }
		});
		// add grass zones to the grid
		Grid.Grass(std::unordered_set<CSquareGrid::Location>
		{
			L{ 9, 14 }, L{ 9, 15 }, L{ 10, 14 }, L{ 17, 14 }, L{ 18, 14 }, L{ 18, 15 },
				L{ 9, 19 }, L{ 9, 20 }, L{ 10, 20 }, L{ 11, 20 }, L{ 12, 20 }, L{ 13, 20 }, L{ 14, 20 }, L{ 15, 20 }, L{ 16, 20 }, L{ 17, 20 }, L{ 18, 20 }, L{ 18, 19 }
		});
		// add water zones to the grid
		Grid.Water(std::unordered_set < CSquareGrid::Location >
		{
			L{ 9, 27 }, L{ 9, 28 }, L{ 9, 29 }, L{ 10, 29 }, L{ 11, 29 }, L{ 12, 29 }, L{ 12, 30 }, L{ 12, 31 }, L{ 15, 31 }, L{ 15, 30 }, L{ 15, 29 }, L{ 16, 29 }, L{ 17, 29 }, L{ 18, 29 }, L{ 18, 28 }, L{ 18, 27 }
		});
	}

	std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>
		CGridWithWeights::BreadthFirstSearch(CGridWithWeights graph, CGridWithWeights::Location start, CGridWithWeights::Location goal)
	{
		typedef CSquareGrid::Location Location;
		std::queue<Location> frontier;
		frontier.push(start);

		std::unordered_map<Location, Location> came_from;
		came_from[start] = start;

		while (!frontier.empty()) {
			auto current = frontier.front();
			frontier.pop();

			if (current == goal) {
				break;
			}

			for (auto next : graph.Neighbors(current)) {
				if (!came_from.count(next)) {
					frontier.push(next);
					came_from[next] = current;
				}
			}
		}
		return came_from;
	}

	void CGridWithWeights::DijkstraSearch(CGridWithWeights graph, CGridWithWeights::Location start, CGridWithWeights::Location goal,
		std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>& came_from, std::unordered_map<CGridWithWeights::Location, int>& cost_so_far)
	{
		typedef CGridWithWeights::Location Location;
		PriorityQueue<Location> frontier;
		frontier.Put(start, 0);

		came_from[start] = start;
		cost_so_far[start] = 0;

		while (!frontier.Empty()) {
			auto current = frontier.Get();

			if (current == goal) {
				break;
			}

			for (auto next : graph.Neighbors(current)) {
				int new_cost = cost_so_far[current] + graph.Cost(next);
				if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
					cost_so_far[next] = new_cost;
					came_from[next] = current;
					frontier.Put(next, new_cost);
				}
			}
		}
	}

	void CGridWithWeights::AStarSearch(CGridWithWeights graph, CGridWithWeights::Location start, CGridWithWeights::Location goal,
		std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>& came_from, std::unordered_map<CGridWithWeights::Location, int>& cost_so_far)
	{
		typedef CGridWithWeights::Location Location;
		PriorityQueue<Location> frontier;
		frontier.Put(start, 0);

		came_from[start] = start;
		cost_so_far[start] = 0;

		while (!frontier.Empty()) {
			auto current = frontier.Get();

			if (current == goal) {
				break;
			}

			for (auto next : graph.Neighbors(current)) {
				int new_cost = cost_so_far[current] + graph.Cost(next);
				if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
					cost_so_far[next] = new_cost;
					int priority = new_cost + Heuristic(next, goal);
					frontier.Put(next, priority);
					came_from[next] = current;
				}
			}
		}
	}

	std::vector<CGridWithWeights::Location> CGridWithWeights::ReconstructPath(CGridWithWeights::Location start, CGridWithWeights::Location goal,
		std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>& came_from)
	{
		std::vector<CGridWithWeights::Location> path;
		CGridWithWeights::Location current = goal;
		path.push_back(current);
		while (current != start) {
			current = came_from[current];
			path.push_back(current);
		}
		if (path.size() >= 2)
		{
			path.erase(path.begin());
			path.pop_back();
		}
		return path;
	}
}
