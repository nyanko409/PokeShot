#pragma once
#ifndef SPRITE_H_
#define SPRITE_H_

#include <d3d9.h>
#include "texture.h"

void Sprite_Initialize(void);
void Sprite_Finalize(void);
void Sprite_SetColor(D3DCOLOR color);
void Sprite_Draw
	(TextureIndex texture_index, //テクスチャーNO
	float dx, float dy,			//スプライト表示座標
		int tx, int ty,				//テクスチャー上座標（ピクセル）
		int tw, int th);			//テクスチャー貼り付けサイズ


#endif // SPRITE_H_
