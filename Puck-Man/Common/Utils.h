#pragma once

#ifndef PUCKMAN_COMMON_UTILS_H_
#define PUCKMAN_COMMON_UTILS_H_

#include <string>
#include "../GLUT/glut.h"

// enable the generation of debugging messages on console
#define DEBUG_CONSOLE 1

// Graphics related data structures and definitions

// window size
static const int WINDOW_WIDTH = 504;
static const int WINDOW_HEIGHT = 648;

static const int SCREEN_WIDTH_TILES = 28;
static const int SCREEN_HEIGHT_TILES = 36;

static const int SCREEN_FPS = 10;

// valid colors for the screen elements
const enum Color
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	PURPLE,
	CONCRETE,
	STEEL,
	AZURE,
	GRAY,
	WHITE,
	BLACK
};

static const struct
{
	float r, g, b;
} colors[] = { { 1.0f,  0.0f,  0.0f },     // red
{ 0.0f,  0.5f,  0.0f },     // green
{ 0.0f,  0.0f,  1.0f },     // blue
{ 1.0f,  1.0f,  0.0f },     // yellow
{ 1.0f,  0.5f,  0.0f },     // orange
{ 0.5f,  0.0f,  0.5f },     // purple
{ 0.75f, 0.75f, 0.75f },    // concrete
{ 0.18f, 0.31f, 0.31f },    // steel
{ 0.94f, 1.0f,  1.0f },     // azure
{ 0.5f,  0.5f,  0.5f },     // gray
{ 1.0f,  1.0f,  1.0f },     // white
{ 0.0f,  0.0f,  0.0f } };   // black

const enum TextSize
{
	TINY = 15,
	SMALL = 25,
	NORMAL = 35,
	BIG = 45
};

inline float TextScale(TextSize size) { return (size / 100.0f); }
inline int   TextOffset(TextSize size, char* text) { return (static_cast<int>(0.35 * std::char_traits<char>::length(text) * size)); }

struct LFRect
{
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
};

// Game related data structures and definitions

const enum GameState
{
	GameOver = 0,
	GameRunning,
	GamePaused
};

#endif // PUCKMAN_COMMON_UTILS_H_
