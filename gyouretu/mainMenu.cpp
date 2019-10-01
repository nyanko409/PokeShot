#include "scene_management.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"


Scene currentScene = MainMenuScreen;

void UpdateMainScreen()
{
	// if button is pressed go to game screen
	if ((GetKeyState(VK_RBUTTON) & 0x100) != 0)
	{
		currentScene = GameScreen;
		return;
	}
}

void DrawMainScreen()
{
	TextureIndex tex = TEXTURE_INDEX_MAINBG;

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(tex, 0, 0, 0, 0, Texture_GetWidth(tex), Texture_GetHeight(tex));
}
