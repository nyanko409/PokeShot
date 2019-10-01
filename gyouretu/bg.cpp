#include "texture.h"
#include "sprite.h"
#include "bg.h"
#include "texture.h"
#include "common.h"
#include "ball.h"


void InitBg()
{
	
}

void UninitBg()
{
	
}

void UpdateBg()
{

}

void DrawBg()
{
	TextureIndex tex = TEXTURE_INDEX_BG;

	int col = 255;
	col -= (1 - slowmoFactor) * 60;

	Sprite_SetColor(D3DCOLOR_RGBA(col, col, col, 255));
	Sprite_Draw(tex, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}
