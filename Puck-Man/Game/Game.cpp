#include "Game.h"
#include "../Common/Utils.h"
#include "../Graphics/Textures.h"

namespace gamecore
{
	CGame& CGame::Instance()
	{
		static CGame game;
		return game;
	}

	bool CGame::Init()
	{
		// set the clear color to black
		glClearColor(0.f, 0.f, 0.f, 0.f);
		// enable alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// enable the use of relative coordinates given a specific screen size
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, 1.0, -1.0);

		glMatrixMode(GL_MODELVIEW);

		// check for any error
		GLenum glError = glGetError();
		if (glError != GL_NO_ERROR)
		{
#if DEBUG_CONSOLE
			std::cout << "ERROR: Unable to load initialize OpenGL, " << gluErrorString(glError) << std::endl;
#endif
			return false;
		}

		// initialize DevIL and DevILU to load the game textures
		ilInit();
		iluInit();

		// set the clear color for DevIL
		ilClearColour(000, 000, 000, 000);

		// check for errors on DevIL and DevILU initialization
		ILenum ilError = ilGetError();
		if (ilError != IL_NO_ERROR)
		{
#if DEBUG_CONSOLE
			std::cout << "ERROR: Unable to load initialize DevIL/DevILU, " << iluErrorString(ilError) << std::endl;
#endif
			return false;
		}

		// initialize the main texture and clipping information
		_scenario.Data().Init();

		// initialize the scenario information
		_scenario.Init();

		// initialize Pacman
		_pacman.Init();

		// create a ghost of each color
		_ghosts = { { Ghost{ Red }, Ghost{ Pink }, Ghost{ Blue }, Ghost{ Orange } } };
		// initialize the ghosts
		for (auto& ghost : _ghosts)
			ghost.Init();

		return true;
	}
}
