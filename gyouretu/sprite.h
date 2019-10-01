#pragma once
#ifndef SPRITE_H_
#define SPRITE_H_

#include <d3d9.h>
#include "texture.h"

void Sprite_Initialize(void);
void Sprite_Finalize(void);
void Sprite_SetColor(D3DCOLOR color);
void Sprite_Draw
	(TextureIndex texture_index, //�e�N�X�`���[NO
	float dx, float dy,			//�X�v���C�g�\�����W
		int tx, int ty,				//�e�N�X�`���[����W�i�s�N�Z���j
		int tw, int th);			//�e�N�X�`���[�\��t���T�C�Y


#endif // SPRITE_H_
