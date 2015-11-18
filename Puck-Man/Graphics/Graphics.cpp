#include "Graphics.h"

#define Game        CGame::Instance()
#define Scenario    CGame::Instance().Scenario()

namespace graphicscore
{
	using gamecore::CGame;

	CGraphics& CGraphics::Instance()
	{
		static CGraphics graphics;
		return graphics;
	}

	/*
	*  initialize the graphics engine
	*/
	bool CGraphics::Init()
	{
		// simulate the argc and argv variables of a normal executable
		int argc = 1;
		char *buffer = new char[MAX_PATH];
		char **argv = &buffer;
		// get name of current application
		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		// GLUT initialization
		glutInit(&argc, argv);
		delete[] buffer;
		// use double buffer and RGBA color schema
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		// center the game window on the screen
		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) >> 1, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) >> 1);
		// set size of the window
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		// set title of the window
		glutCreateWindow("Puck-Man");

		// assign callback to paint the window
		glutDisplayFunc(CGraphics::paint);
		// assign callback to the loop method to achieve 60 FPS
		glutTimerFunc(1000 / SCREEN_FPS, CGraphics::loop, 0);

		// assign callback to get the key been pressed
		glutKeyboardFunc(CGraphics::keys);
		// assign callback to get the special key been pressed
		glutSpecialFunc(CGraphics::special);

		// initialize the game
		return Game.Init();
	}

	/*
	*  paint the game scene
	*/
	void CGraphics::PaintScene()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// render scenario map on screen
		Scenario.RenderMap();

		// TODO: hack to get the normal color on screen, will solve this later
		SetColor(WHITE);

		// render Pacman on the screen
		Game.Pacman().Render();

		// render the Ghosts on the screen
		for (auto& ghost : Game.Ghosts())
			ghost.Render();

		// check if the game is paused or not
		if (Game.State() == GamePaused)
		{
			// show the pause message
			PaintText(WINDOW_WIDTH >> 1, (WINDOW_HEIGHT >> 1) + 60, "Pause", SMALL);
			// show the cursor on screen
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		else
			// hide the cursor on screen
			glutSetCursor(GLUT_CURSOR_NONE);

		// paint objects on screen by swapping buffers
		glutSwapBuffers();
	}

	/*
	*  give the timing to the game
	*/
	void CGraphics::LoopGame(int value)
	{
		// change the game state is it's not paused
		if (Game.State() != GamePaused)
		{
			// move Pacman on the scenario
			Game.Pacman().Move();

			// move the Ghosts on the scenario
			for (auto& ghost : Game.Ghosts())
				ghost.Move();
		}
		// trigger the update of the screen
		glutPostRedisplay();
		// set the new wait interval to achieve 60 FPS
		glutTimerFunc(1000 / SCREEN_FPS, CGraphics::loop, value);
	}

	/*
	*  process the normal pressed keys
	*/
	void CGraphics::KeyPressed(unsigned char key, int x, int y)
	{
		// check what key was pressed
		switch (static_cast<ControlKeys>(key))
		{
		case EnterKey:
#if DEBUG_CONSOLE
			std::cout << "Pause game" << std::endl;
#endif
			if (Game.State() == GameRunning)
				Game.State(GamePaused);
			else if (Game.State() == GamePaused)
				Game.State(GameRunning);
			break;
		case 'r' | 'R':
			for (auto& ghost : Game.Ghosts())
				ghost.Reset();
			break;
		case EscapeKey:
			exit(0);
			break;
		}
	}

	/*
	*  process the special pressed keys
	*/
	void CGraphics::SpecialKeyPressed(int key, int x, int y)
	{
		// check what key was pressed
		switch (static_cast<ControlKeys>(key))
		{
			// send the key event to the Pacman object
		case LeftKey:
			Game.Pacman().ChangeDirection(gamecore::Left);
			break;
		case RightKey:
			Game.Pacman().ChangeDirection(gamecore::Right);
			break;
		case UpKey:
			Game.Pacman().ChangeDirection(gamecore::Up);
			break;
		case DownKey:
			Game.Pacman().ChangeDirection(gamecore::Down);
			break;
		default:
			break;
		}
	}

	void CGraphics::PaintSquare(std::tuple<int, int> location, Color color)
	{
		// remove any previous transformations
		glLoadIdentity();
		SetColor(color, 0.60f);
		int x = (std::get<0>(location) * SIZE_OF_TILE) - 1;
		int y = std::get<1>(location) * SIZE_OF_TILE;
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + SIZE_OF_TILE, y);
		glVertex2f(x + SIZE_OF_TILE, y + SIZE_OF_TILE);
		glVertex2f(x, y + SIZE_OF_TILE);
		glEnd();
	}

	/*
	*  paint text on screen
	*/
	void CGraphics::PaintText(int x, int y, char *text, TextSize size)
	{
		// remove any previous transformations
		glLoadIdentity();
		// set text color to white
		SetColor(WHITE);
		// translate to the specified coordinates
		glTranslatef(x - TextOffset(size, text), y, 0.f);
		// scale the text to the selected size
		glScalef(TextScale(size), -(TextScale(size)), TextScale(size));
		// set width of the line to draw the font
		glLineWidth(2);
		// paint each character on the screen
		for (auto ch = text; *ch != '\0'; ch++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *ch);
	}
}
