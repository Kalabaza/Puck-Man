#pragma once

#ifndef PUCKMAN_GAME_SCENARIO_H_
#define PUCKMAN_GAME_SCENARIO_H_

#include <array>
#include <functional>
#include <map>
#include <queue>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../Graphics/Textures.h"
#include "../Common/Utils.h"

// addition of the hash method for the unordered_set
namespace std {
	template <>
	struct hash < tuple<int, int> > {
		inline size_t operator()(const tuple<int, int>& location) const {
			int x, y;
			tie(x, y) = location;
			return x * 1812433253 + y;
		}
	};
}

namespace gamecore
{
	using graphicscore::CData;
	using graphicscore::CTexture;

	// definition of the matrix type
	template<typename Type, size_t Row, size_t Col>
	using Matrix = std::array<std::array<Type, Col>, Row>;

	// definition of the sprite type
	template<typename Type1, typename Type2>
	using Sprite = std::pair<std::pair<Type1, Type1>, Type2>;

	// elements on the screen
	const enum Elements
	{
		Empty = 0,
		Pill,
		Energizer,
		Wall
	};

	// definition of a Square Grid
	class CSquareGrid {
	public:
		typedef std::tuple<int, int> Location;

		CSquareGrid(int w, int h) : _w(w), _h(h) {}
		~CSquareGrid() = default;

		inline bool InBounds(Location id) {
			int x, y;
			std::tie(x, y) = id;
			return 0 <= x && x < _w && 0 <= y && y < _h;
		}

		inline bool Passable(Location id) {
			return !_walls.count(id);
		}

		std::vector<Location> Neighbors(Location id) {
			int x, y, dx, dy;
			std::tie(x, y) = id;
			std::vector<Location> results;

			for (auto dir : DIRS) {
				std::tie(dx, dy) = dir;
				Location next(x + dx, y + dy);
				if (InBounds(next) && Passable(next)) {
					results.push_back(next);
				}
			}

			if ((x + y) % 2 == 0) {
				// aesthetic improvement on square grids
				std::reverse(results.begin(), results.end());
			}
			return results;
		}

		std::unordered_set<Location> _walls;

	protected:
		int _w, _h;
		static std::array<Location, 4> DIRS;
	};

	class CGridWithWeights : public CSquareGrid
	{
	public:
		CGridWithWeights(int w, int h) : CSquareGrid(w, h) {}
		~CGridWithWeights() = default;

		int Cost(Location l) {
			if (_ground.count(l))
				return 3;
			else if (_grass.count(l))
				return 5;
			else if (_water.count(l))
				return 10;
			return 1;
		}

		static CGridWithWeights& Instance();

		void AddRect(CSquareGrid& grid, int x1, int y1, int x2, int y2);

		void PopulateGrid();

		std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>
			BreadthFirstSearch(CGridWithWeights graph, CGridWithWeights::Location start, CGridWithWeights::Location goal);

		void CGridWithWeights::DijkstraSearch(CGridWithWeights graph, CGridWithWeights::Location start, CGridWithWeights::Location goal,
			std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>& came_from, std::unordered_map<CGridWithWeights::Location, int>& cost_so_far);

		void AStarSearch(CGridWithWeights graph, CGridWithWeights::Location start, CGridWithWeights::Location goal,
			std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>& came_from, std::unordered_map<CGridWithWeights::Location, int>& cost_so_far);

		std::vector<CGridWithWeights::Location> CGridWithWeights::ReconstructPath(CGridWithWeights::Location start, CGridWithWeights::Location goal,
			std::unordered_map<CGridWithWeights::Location, CGridWithWeights::Location>& came_from);

		std::unordered_set<Location>& Ground() { return _ground; }
		void Ground(std::unordered_set<Location> ground) { _ground = ground; }
		std::unordered_set<Location>& Grass() { return _grass; }
		void Grass(std::unordered_set<Location> grass) { _grass = grass; }
		std::unordered_set<Location>& Water() { return _water; }
		void Water(std::unordered_set<Location> water) { _water = water; }

	private:
		std::unordered_set<Location> _ground;
		std::unordered_set<Location> _grass;
		std::unordered_set<Location> _water;

		inline int Heuristic(CSquareGrid::Location a, CSquareGrid::Location b) {
			int x1, y1, x2, y2;
			std::tie(x1, y1) = a;
			std::tie(x2, y2) = b;
			return std::abs(x1 - x2) + std::abs(y1 - y2);
		}
	};

	template<typename T, typename Number = int>
	struct PriorityQueue {
		typedef std::pair<Number, T> PQElement;
		std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

		inline bool Empty() { return elements.empty(); }

		inline void Put(T item, Number priority) {
			elements.emplace(priority, item);
		}

		inline T Get() {
			T best_item = elements.top().second;
			elements.pop();
			return best_item;
		}
	};

	class CScenario
	{
	public:
		CScenario() = default;
		~CScenario() = default;

		// load the scenario information
		bool Init();

		// render the scenario map on screen
		void RenderMap();

		CData& Data() { return _data; }

		Matrix<Elements, SCREEN_WIDTH_TILES, SCREEN_HEIGHT_TILES>& ElementsMatrix() { return _elementsMatrix; }
		void ElementsMatrix(int x, int y, Elements e) { _elementsMatrix[x][y] = e; }

	private:
		// object that holds the scenario sprite data
		CData _data;

		// matrix to hold the type of elements on the screen (for collisions' detection)
		Matrix<Elements, SCREEN_WIDTH_TILES, SCREEN_HEIGHT_TILES> _elementsMatrix;

		// map to hold the sprites shown on the scenario
		std::map<std::pair<GLfloat, GLfloat>, LFRect> _spritesMap;

		// vector to hold the arrange of the sprites of the scenario
		std::vector<Sprite<GLfloat, LFRect>> _spritesVector;

		// method to add the content of a file to the sprites map
		bool CScenario::AddSpritesFile(std::string filename, Elements element);
	};
}

#endif // PUCKMAN_GAME_SCENARIO_H_

