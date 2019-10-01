#include "player.h"
#include "texture.h"
#include "sprite.h"

// player data
Player* player;

Player* GetPlayer()
{
	return player;
}

void InitPlayer()
{
	// init player
	player = new Player();
}

void UninitPlayer()
{
	// free memory
	delete player;
}

void UpdatePlayer()
{
	// update uv pos
	player->UpdateUV();
}

void DrawPlayer()
{
	// init texture to display and uv offset for mirrored image
	TextureIndex tex;
	int uvOffset;

	if (player->isFacingLeft)
	{
		tex = TEXTURE_INDEX_SPRITESHEET_PLAYER_LEFT;
		uvOffset = Texture_GetWidth(tex) >> 3;
	}
	else
	{
		tex = TEXTURE_INDEX_SPRITESHEET_PLAYER_RIGHT;
		uvOffset = 0;
	}

	// draw
	if (player->isMoving)
	{
		// moving texture
		Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
		Sprite_Draw(tex, player->posX, player->posY, player->uvWalk[player->uvWalkFrame / 2].first - uvOffset, player->uvWalk[player->uvWalkFrame / 2].second, Texture_GetWidth(tex) / 8, Texture_GetHeight(tex) / 8);
	}
	else
	{
		// standing texture
		Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
		Sprite_Draw(tex, player->posX, player->posY, player->uvStand.first - uvOffset, player->uvStand.second, Texture_GetWidth(tex) / 8, Texture_GetHeight(tex) / 8);
	}
}
