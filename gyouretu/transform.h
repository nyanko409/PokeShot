#pragma once

#include "texture.h"
#include "mydirect3d.h"

namespace Ato
{ 
	void ScaleFromCenter(bool center);		// if true, sprite will scale from center
	void SetScale(float scale);				// set the scale of the sprite
	void SetColor(D3DCOLOR col);			// set the color of the sprite

	// draw the sprite
	void DrawSprite(TextureIndex tex, int posX, int posY, int dispWidth, int dispHeight,
					float startX = 0.0F, float startY = 0.0F, float endX = 1.0F, float endY = 1.0F);
}
