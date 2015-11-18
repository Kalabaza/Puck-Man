#pragma once

#ifndef PUCKMAN_GRAPHICS_GRAPHICS_H_
#define PUCKMAN_GRAPHICS_GRAPHICS_H_

#include <iostream>
#include <windows.h>
#include "../GLUT/glut.h"
#include "../DevIL/il.h"
#include "../Game/Game.h"
#include "../Common/Utils.h"

namespace graphicscore
{
	const enum ControlKeys
	{
		EnterKey = 13,
		EscapeKey = 27,
		LeftKey = GLUT_KEY_LEFT,
		RightKey = GLUT_KEY_RIGHT,
		UpKey = GLUT_KEY_UP,
		DownKey = GLUT_KEY_DOWN
	};

	class CGraphics
	{
	public:
		// method to get an instance of the CGraphics class
		static CGraphics& Instance();

		// initialize the game engine
		bool Init();
		// execute the glut's main loop
		void MainLoop() { glutMainLoop(); }

		// paint a square on screen
		void PaintSquare(std::tuple<int, int> location, Color color);

	private:
		// disable the creation of objects outside of this class and use the default constructor
		CGraphics() = default;
		// use default constructor (no parameters)
		~CGraphics() = default;

		/* remove these features to avoid deletion of this class' objects */
		// remove the copy constructor
		CGraphics(const CGraphics& src) = delete;
		// remove the copy assign operator
		const CGraphics& operator=(const CGraphics& rhs) = delete;
		// remove the delete operator
		void operator delete(void*) = delete;

		// wrapper methods to call a class member from plain C code
		static void paint() { Instance().PaintScene(); }
		static void keys(unsigned char key, int x, int y) { Instance().KeyPressed(key, x, y); }
		static void special(int key, int x, int y) { Instance().SpecialKeyPressed(key, x, y); }
		static void loop(int value) { Instance().LoopGame(value); }

		// method to paint the scene on screen
		void PaintScene();
		// method to catch the keys being pressed during execution
		void KeyPressed(unsigned char key, int x, int y);
		// special keys pressed during execution
		void SpecialKeyPressed(int key, int x, int y);
		// method to move things on screen
		void LoopGame(int);

		// method to set color of an object on screen
		inline void CGraphics::SetColor(Color color)
		{
			glColor3f(colors[color].r, colors[color].g, colors[color].b);
		}

		// method to set color of an object on screen
		inline void SetColor(Color color, GLfloat alpha)
		{
			glColor4f(colors[color].r, colors[color].g, colors[color].b, alpha);
		}

		// method to display text on screen
		void PaintText(int x, int y, char *string, TextSize size);
	};
}

#endif // PUCKMAN_GRAPHICS_GRAPHICS_H_
