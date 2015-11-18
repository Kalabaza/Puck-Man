#pragma once

#ifndef PUCKMAN_GAME_CHARACTER_H_
#define PUCKMAN_GAME_CHARACTER_H_

#include "../Graphics/Textures.h"
#include "../Common/Utils.h"

namespace gamecore
{
	const enum Direction
	{
		Left = 0,
		Right,
		Up,
		Down,
	};

	const enum Ghost
	{
		Red = 0,
		Pink,
		Blue,
		Orange,
		// element to obtain size of the enumeration
		Ghost_Size
	};

	class CCharacter
	{
	public:
		CCharacter(graphicscore::Clipping c) : _c{ c } {};
		~CCharacter() = default;

		// set position of character
		void Position(GLfloat x, GLfloat y) { _x = x; _y = y; }
		void Position(std::tuple<int, int> position) { _x = std::get<0>(position); _y = std::get<1>(position); }

		// get position of character
		std::tuple<int, int> Position() { return std::make_tuple(static_cast<int>(_x), static_cast<int>(_y)); }

		// set clipping information
		void Clipping(graphicscore::Clipping c) { _c = c; }

		// init the character
		virtual bool Init() = 0;

		// render the character on screen
		virtual void Render();

		// move the character depending on the key pressed
		virtual void Move() = 0;

	protected:
		// center position of the character
		GLfloat _x, _y;
		// clipping information to draw sprite
		graphicscore::Clipping _c;
	};

	/*
	*  Pacman class
	*/
	class CPacman : public CCharacter
	{
	public:
		// constructor of Pacman that sets its initial position
		CPacman() : CCharacter{ graphicscore::L_PACMAN } {}
		~CPacman() = default;

		/* virtual methods */
		// init Pacman location
		bool Init() { Position(13.5, 26); return true; }

		// move Pacman in the current facing direction
		void Move();

		// change the direction at which Pacman is facing
		void ChangeDirection(Direction dir) { d = dir; }

	private:
		// current facing direction
		Direction d;
	};

	/*
	*  Ghost class
	*/
	class CGhost : public CCharacter
	{
	public:
		// constructor of the Ghosts that sets its initial position
		CGhost() : CCharacter{ graphicscore::GHOST } {}
		CGhost(Ghost g) : CCharacter{ graphicscore::GHOST } { _g = g; }
		~CGhost() = default;

		bool Init();

		// move a Ghost to chase Pacman
		void Move();

		void Reset();

	private:
		Ghost _g;
	};
}

#endif // PUCKMAN_GAME_CHARACTER_H_
