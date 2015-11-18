#pragma once

#ifndef PUCKMAN_GAME_GAME_H_
#define PUCKMAN_GAME_GAME_H_

#pragma comment(lib,"glut32")

#include "Characters.h"
#include "Scenario.h"
#include "../GLUT/glut.h"

namespace gamecore
{
	class CPacman;

	class CGame
	{
	public:
		// method to get an instance of the CGraphics class
		static CGame& Instance();

		// method to initialize the game
		bool Init();

		// get/set the state of the game
		GameState State() { return _state; }
		void State(const GameState state) { _state = state; }

		// get the scenario for the current game
		CScenario& Scenario() { return _scenario; }
		// get the Pacman character of the current game
		CPacman& Pacman() { return _pacman; }
		// get the Ghosts from the current game
		std::array<CGhost, Ghost_Size>& Ghosts() { return _ghosts; }

	private:
		// disable the creation of objects outside of this class
		CGame::CGame() : _state(GameRunning) {}
		// use default constructor (no parameters)
		~CGame() = default;

		// object for the scenario
		CScenario _scenario;
		// Pacman object
		CPacman _pacman;
		// Ghosts array
		std::array<CGhost, Ghost_Size> _ghosts;

		/* remove these features to avoid deletion of this class' object */
		// remove the copy constructor
		CGame(const CGame& src) = delete;
		// remove the copy assign operator
		const CGame& operator=(const CGame& rhs) = delete;
		// remove the delete operator
		void operator delete(void*) = delete;

		GameState _state;
	};
}

#endif // PUCKMAN_GAME_GAME_H_
