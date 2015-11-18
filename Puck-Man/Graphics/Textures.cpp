#include "Textures.h"

namespace graphicscore
{
	bool CTexture::LoadTextureFromFile(std::string filename)
	{
		// texture loading result
		bool textureLoaded = false;

		// generate and set current image ID on DevIL
		ILuint imgID = 0;
		ilGenImages(1, &imgID);
		ilBindImage(imgID);

		// load the texture file
		ILboolean status = ilLoadImage(filename.c_str());

		// check if the image was loaded successfully
		if (status == IL_TRUE)
		{
			// convert image to RGBA
			status = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			if (status == IL_TRUE)
			{
				// initialize the dimensions of the image
				_imageWidth = static_cast<GLuint>(ilGetInteger(IL_IMAGE_WIDTH));
				_imageHeight = static_cast<GLuint>(ilGetInteger(IL_IMAGE_HEIGHT));

				// calculate required texture dimensions
				_textureWidth = PowerOfTwo(_imageWidth);
				_textureHeight = PowerOfTwo(_imageHeight);

				// texture is of different size that the actual image
				if (_imageWidth != _textureWidth || _imageHeight != _textureHeight)
				{
					// place the image at the upper left
					iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

					// resize the image
					iluEnlargeCanvas(static_cast<int>(_textureWidth), static_cast<int>(_textureHeight), 1);
				}

				// create texture form file pixels
				textureLoaded = LoadTextureFromPixels32(reinterpret_cast<GLuint*>(ilGetData()));
			}

			// delete file from memory
			ilDeleteImages(1, &imgID);
		}

#if DEBUG_CONSOLE
		if (!textureLoaded)
			std::cout << "ERROR: Unable to load texture: " << filename << std::endl;
#endif

		return textureLoaded;
	}

	bool CTexture::LoadTextureFromPixels32(GLuint* pixels)
	{
		// generate texture ID
		glGenTextures(1, &_ID);
		// bind the texture ID
		glBindTexture(GL_TEXTURE_2D, _ID);

		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textureWidth, _textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// check for any error
		GLenum glError = glGetError();
		if (glError != GL_NO_ERROR)
		{
#if DEBUG_CONSOLE
			std::cout << "ERROR: Unable to load texture from pixels: " << pixels << ", " << gluErrorString(glError) << std::endl;
#endif
			return false;
		}
		return true;
	}

	void CTexture::FreeTexture()
	{
		// delete texture
		if (_ID != 0)
		{
			glDeleteTextures(1, &_ID);
			_ID = _imageWidth = _imageHeight = _textureWidth = _textureHeight = 0;
		}
	}

	GLuint CTexture::PowerOfTwo(GLuint num)
	{
		if (num != 0)
		{
			num--;
			num |= (num >> 1);  // OR with the first two bits
			num |= (num >> 2);  // OR with the next two bits
			num |= (num >> 4);  // OR with the next 4 bits
			num |= (num >> 8);  // OR with the next 8 bits
			num |= (num >> 16); // OR with the next 16 bits
			num++;
		}
		return num;
	}

	bool CData::Init()
	{
		// load the main texture with the sprites
		if (!_texture.LoadTextureFromFile("Textures/Pacman.png"))
		{
#if DEBUG_CONSOLE
			std::cout << "ERROR: Unable to load non-power-of-two texture" << std::endl;
#endif
			return false;
		}

		// set the clipping information of the game sprites
		// dual line corners
		_clippings[LUC_DL] = { 312, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 0
		_clippings[LLC_DL] = { 384, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 1
		_clippings[RUC_DL] = { 294, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 2
		_clippings[RLC_DL] = { 366, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 3
																	   // special dual line corners
		_clippings[LUCS_DL] = { 474, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 4
		_clippings[LLCS_DL] = { 420, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 5
		_clippings[RUCS_DL] = { 492, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 6
		_clippings[RLCS_DL] = { 402, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 7
																		// single line corners
		_clippings[LUC_SL] = { 420, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 8
		_clippings[LLC_SL] = { 456, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 9
		_clippings[RUC_SL] = { 402, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 10
		_clippings[RLC_SL] = { 438, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 11
																	   // dual line
		_clippings[VL_DL] = { 348, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 12
		_clippings[VR_DL] = { 330, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 13
		_clippings[HU_DL] = { 474, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 14
		_clippings[HL_DL] = { 510, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 15
																	  // single line
		_clippings[VL_SL] = { 168, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 16
		_clippings[VR_SL] = { 150, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 17
		_clippings[HU_SL] = { 78, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 18
		_clippings[HL_SL] = { 546, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 19
																	  // square dual line corners
		_clippings[LUSC_DL] = { 240, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 20
		_clippings[LLSC_DL] = { 276, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 21
		_clippings[RUSC_DL] = { 222, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 22
		_clippings[RLSC_DL] = { 258, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 23
																		// special dual line
		_clippings[HLSL_DL] = { 312, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 24
		_clippings[HLSR_DL] = { 294, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 25
																		// special single line
		_clippings[HLP_DL] = { 258, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 26
																	   // special single line corners
		_clippings[LUSC_SL] = { 330, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 27
		_clippings[LLSC_SL] = { 366, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 28
		_clippings[RUSC_SL] = { 348, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 29
		_clippings[RLSC_SL] = { 384, 133, SIZE_OF_TILE, SIZE_OF_TILE }; // 30
																		// special dual line corners 2
		_clippings[LUCS2_DL] = { 456, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 31
		_clippings[RUCS2_DL] = { 438, 115, SIZE_OF_TILE, SIZE_OF_TILE }; // 32
																		 // pills
		_clippings[DOT_PILL] = { 294, 7, SIZE_OF_TILE, SIZE_OF_TILE }; // 33
		_clippings[ENER_PILL] = { 366, 7, SIZE_OF_TILE, SIZE_OF_TILE }; // 34
																		/* game characters */
																		// Pacman
		_clippings[L_PACMAN] = { 6,   403, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 35
		_clippings[R_PACMAN] = { 438, 259, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 36
		_clippings[U_PACMAN] = { 42,  403, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 37
		_clippings[D_PACMAN] = { 474, 259, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 38
																				   // red ghost
		_clippings[LR_GHOST] = { 150, 259, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 40
																				   // pink ghost
		_clippings[LP_GHOST] = { 150, 331, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 4
																				   // blue ghost
		_clippings[LB_GHOST] = { 438, 331, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 45
																				   // yellow ghost
		_clippings[LO_GHOST] = { 150, 367, SIZE_OF_CHARACTER, SIZE_OF_CHARACTER }; // 46

		return true;
	}

	void CData::RenderTexture(GLfloat x, GLfloat y, LFRect* clip)
	{
		if (_texture.ID() != 0)
		{
			// remove any previous transformations
			glLoadIdentity();

			// calculate texture coordinates
			GLfloat texTop = 0.f;
			GLfloat texBottom = 1.f;
			GLfloat texLeft = 0.f;
			GLfloat texRight = 1.f;

			// vertex coordinates
			GLfloat quadWidth = _texture.TextureWidth();
			GLfloat quadHeight = _texture.TextureHeight();

			// handle clipping
			if (clip != nullptr)
			{
				// texture coordinates
				texLeft = clip->x / _texture.TextureWidth();
				texRight = (clip->x + clip->w) / _texture.TextureWidth();
				texTop = clip->y / _texture.TextureHeight();
				texBottom = (clip->y + clip->h) / _texture.TextureHeight();

				// vertex coordinates
				quadWidth = clip->w;
				quadHeight = clip->h;
			}

			// move to rendering spot
			glTranslatef(x, y, 0.f);

			// enable 2D textures
			glEnable(GL_TEXTURE_2D);

			// set texture ID
			glBindTexture(GL_TEXTURE_2D, _texture.ID());

			// render textured quad
			glBegin(GL_QUADS);
			glTexCoord2f(texLeft, texTop);     glVertex2f(0.f, 0.f);
			glTexCoord2f(texRight, texTop);     glVertex2f(quadWidth, 0.f);
			glTexCoord2f(texRight, texBottom);  glVertex2f(quadWidth, quadHeight);
			glTexCoord2f(texLeft, texBottom);  glVertex2f(0.f, quadHeight);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}
}
