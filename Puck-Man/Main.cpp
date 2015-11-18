#include <iostream>
#include "Graphics/Graphics.h"

using graphicscore::CGraphics;

// delete the console on the executable
#if !DEBUG_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

int main(int argc, char **argv)
{
	// initialize the game
	if (!CGraphics::Instance().Init())
	{
		return EXIT_FAILURE;
	}

	// give control to OpenGL
	CGraphics::Instance().MainLoop();

	return 0;
}
