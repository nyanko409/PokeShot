#include "cursor.h"
#include "ball.h"
#include "texture.h"
#include "sprite.h"


static v2i cursorPos;

void InitCursor()
{
	ShowCursor(false);

	// get cursor pos at location
	POINT p = POINT();
	if (GetCursorPos(&p))
	{
		cursorPos.x = p.x;
		cursorPos.y = p.y;
	}
}

void UpdateCursor()
{
	// set cursor pos at location
	POINT p = POINT();
	if (GetCursorPos(&p))
	{
		cursorPos.x = p.x;
		cursorPos.y = p.y;
		cursorPos = cursorPos - v2i{ 50, 80 };
	}
}

void DrawCursor()
{
	TextureIndex tex = TEXTURE_INDEX_CURSOR;
	
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(tex, cursorPos.x, cursorPos.y, 0, 0, Texture_GetWidth(tex), Texture_GetHeight(tex));
}
