#pragma once

#ifndef PUCKMAN_GRAPHICS_TEXTURES_H_
#define PUCKMAN_GRAPHICS_TEXTURES_H_

#pragma comment(lib,"DevIL")
#pragma comment(lib,"ILU")

#include <array>
#include <iostream>
#include <string>
#include "../DevIL/il.h"
#include "../DevIL/ilu.h"
#include "../Common/Utils.h"

namespace graphicscore
{
	// size of a tile
	static const int SIZE_OF_TILE = 18;
	// size of game character
	static const int SIZE_OF_CHARACTER = 33;

	const enum Clipping
	{
		/* Scenario elements */
		// Corners Dual-Line
		LUC_DL = 0, // Left Upper  - 0
		LLC_DL,     // Left Lower  - 1
		RUC_DL,     // Right Upper - 2
		RLC_DL,     // Right Lower - 3

					// Special Corners Dual-Line
		LUCS_DL,    // Special Left Upper  - 4
		LLCS_DL,    // Special Left Lower  - 5
		RUCS_DL,    // Special Right Upper - 6
		RLCS_DL,    // Special Right Lower - 7

					// Corners Single-Line
		LUC_SL,     // Left Upper  - 8
		LLC_SL,     // Left Lower  - 9
		RUC_SL,     // Right Upper - 10
		RLC_SL,     // Right Lower - 11

					// Dual-Line
		VL_DL,      // Vertical Left    - 12
		VR_DL,      // Vertical Right   - 13
		HU_DL,      // Horizontal Upper - 14
		HL_DL,      // Horizontal Lower - 15

					// Single-Line
		VL_SL,      // Vertical Left    - 16
		VR_SL,      // Vertical Right   - 17
		HU_SL,      // Horizontal Upper - 18
		HL_SL,      // Horizontal Lower - 19

					// Square Corners Dual-Line
		LUSC_DL,    // Left Upper Square  - 20
		LLSC_DL,    // Left Lower Square  - 21
		RUSC_DL,    // Right Upper Square - 22
		RLSC_DL,    // Right Lower Square - 23

					// Special Dual-Line
		HLSL_DL,    // Special Horizontal Lower Left  - 24
		HLSR_DL,    // Special Horizontal Lower Right - 25

					// Special Single-Line
		HLP_DL,     // Pink Horizontal Lower - 26

					// Special Corner Single-Line
		LUSC_SL,    // Special Left Upper  - 27
		LLSC_SL,    // Special Left Lower  - 28
		RUSC_SL,    // Special Right Upper - 29
		RLSC_SL,    // Special Right Lower - 30

					// Special Corner Dual-Line
		LUCS2_DL,   // Special Left Upper  - 31
		RUCS2_DL,   // Special Right Upper - 32

					// Pills
		DOT_PILL,   // Small Pill (Dot)     - 33
		ENER_PILL,  // Big Pill (Energizer) - 34

					/* Characters */
					// Pacman
		L_PACMAN,   // Left Pacman   -35
		R_PACMAN,   // Right Pacman - 36
		U_PACMAN,   // UP Pacman    - 37
		D_PACMAN,   // Down Pacman  - 38

		GHOST,      // Dummy Ghost  -- 39

					// Red Ghost
		LR_GHOST,   // Left Reg Ghost  - 40
		RR_GHOST,   // Right Red Ghost - 41
		UR_GHOST,   // Up Red Ghost    - 42
		DR_GHOST,   // Down Red Ghost  - 43

					// Pink Ghost
		LP_GHOST,   // Left Pink Ghost - 44

					// Blue Ghost
		LB_GHOST,   // Left Blue Ghost - 45

					// Orange Ghost
		LO_GHOST,   // Left Orange Ghost - 46

					// Element to obtain size of the enumeration
		Clipping_Size
	};

	class CTexture
	{
	public:
		CTexture() : _ID{ 0 }, _imageWidth{ 0 }, _imageHeight{ 0 }, _textureWidth{ 0 }, _textureHeight{ 0 } {}
		~CTexture() { FreeTexture(); }

		const GLuint ID() { return _ID; }
		const int ImageWidth() { return _imageWidth; }
		const int ImageHeight() { return _imageHeight; }
		const int TextureWidth() { return _textureWidth; }
		const int TextureHeight() { return _textureHeight; }

		// method to load texture from a file
		bool LoadTextureFromFile(std::string filename);

	private:
		// ID of the texture
		GLuint _ID;

		// size of the image
		int _imageWidth, _imageHeight;

		// size of the texture
		int _textureWidth, _textureHeight;

		// method to put a texture into memory
		bool LoadTextureFromPixels32(GLuint* pixels);

		// method to release a texture
		void FreeTexture();

		// method to get the next power of 2 size for a texture
		GLuint PowerOfTwo(GLuint num);
	};

	class CData
	{
	public:
		CData() = default;
		~CData() = default;

		CTexture& Texture() { return _texture; }
		LFRect& Clippings(int index) { return _clippings[index]; }

		// load the texture with the game sprites
		bool Init();

		// render a texture on screen
		void RenderTexture(GLfloat x, GLfloat y, LFRect* clip = nullptr);

	private:
		// member variable to hold the game texture
		CTexture _texture;

		// member variable to save the clipping information of the sprites
		std::array<LFRect, Clipping_Size> _clippings;
	};
}

#endif // PUCKMAN_GRAPHICS_TEXTURES_H_
