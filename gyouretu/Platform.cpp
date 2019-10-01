#include "Platform.h"
#include "texture.h"


// constructor
Platform::Platform(int posX, int posY, int width)
{
	// init values
	this->width = width;
	setX(posX);
	setY(posY);

	// init uv array
	uvPos = (std::pair<int, int>*)calloc(width, sizeof(std::pair<int, int>));
	initUV();
}

// destructor
Platform::~Platform()
{
	// free memory
	free(uvPos);
}

// init uv array depending on length
void Platform::initUV()
{
	// init tile width and height
	int texWidth = Texture_GetWidth(TEXTURE_INDEX_SPRITESHEET_GROUND);
	int texHeight = Texture_GetHeight(TEXTURE_INDEX_SPRITESHEET_GROUND);

	// init width and height
	widthPixel = texWidth / 8;
	heightPixel = texHeight / 16;

	// if width is 1, set and return
	if (width == 1)
	{
		uvPos[0].first = texWidth / 8 * 1;
		uvPos[0].second = texHeight / 16 * 1;
		return;
	}

	// left side
	uvPos[0].first = 0;
	uvPos[0].second = texHeight / 16 * 2;

	// mid
	for (int i = 1; i < width - 1; i++)
	{
		uvPos[i].first = 0;
		uvPos[i].second = texHeight / 16 * 1;
	}

	// right side
	uvPos[width - 1].first = 0;
	uvPos[width - 1].second = 0;
}
