#include "scene_management.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "enemy.h"
#include <string>


void UpdateResultScreen()
{
	// if button is pressed go to game screen
	if (Keyboard_IsRelease(DIK_SPACE))
	{
		currentScene = MainMenuScreen;
		return;
	}

	// retry if right mouse button
	if ((GetKeyState(VK_RBUTTON) & 0x100) != 0)
	{
		currentScene = GameScreen;
		return;
	}
}

void DrawResultScreen()
{
	// draw bg
	TextureIndex tex = TEXTURE_INDEX_RESULTBG;

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(tex, 0, 0, 0, 0, Texture_GetWidth(tex), Texture_GetHeight(tex));

	// draw score
	RECT rect;
	int offset = score < 10 ? 0 : -75;
	SetRect(&rect, 600 + offset, 300, 30, 30);

	auto str = std::to_string(score);
	g_pBigFont->DrawTextA(NULL, str.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

}
