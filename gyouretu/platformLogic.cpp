#include "Platform.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"


// list of platforms
std::vector<Platform*> platform = {};


void InitPlatform()
{
	// bot platform
	SetPlatform(300, 705, 20);
	//SetPlatform(900, 705, 10);
	//SetPlatform(900, 705, 10);

	// mid platform
	SetPlatform(50, 500, 12);
	SetPlatform(200, 300, 3);
}

void UninitPlatform()
{

}

void UpdatePlatform()
{
	for (int i = 0; i < platform.size(); i++)
	{
		platform[i]->setX(platform[i]->getX() - xCamDiff);
		platform[i]->setY(platform[i]->getY() - yCamDiff);
	}
}

void DrawPlatform()
{
	// set texture
	TextureIndex tex = TEXTURE_INDEX_SPRITESHEET_GROUND;
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

	// draw ground tiles
	for (int i = 0; i < platform.size(); i++)
		for (int j = 0; j < (*platform[i]).getWidth(); j++)
			Sprite_Draw(tex, (*platform[i]).getX() + j * (Texture_GetWidth(tex) / 8), (*platform[i]).getY(),
			(*platform[i]).getUVPos()[j].first, (*platform[i]).getUVPos()[j].second,
				Texture_GetWidth(tex) / 8, Texture_GetHeight(tex) / 16);
}

// create new platform at location
void SetPlatform(int posX, int posY, int width)
{
	// add platform to vector
	platform.push_back(new Platform(posX, posY, width));
}

// return pointer of list of platforms
std::vector<Platform*>* GetPlatform()
{
	return &platform;
}
