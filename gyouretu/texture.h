#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d9.h>
#include <d3dx9.h>

//�e�N�X�`���[�ԍ�����U��
enum TextureIndex
{
	TEXTURE_INDEX_BALL,
	TEXTURE_INDEX_METALTILE,
	TEXTURE_INDEX_BG,
	TEXTURE_INDEX_ENEMY,
	TEXTURE_INDEX_CURSOR,
	TEXTURE_INDEX_MAINBG,
	TEXTURE_INDEX_RESULTBG,

	TEXTURE_INDEX_MAX
};


//�v���g�^�C�v�錾
int Texture_Load(void);
void TextureRelease(void);

LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex idx);

int Texture_GetWidth(TextureIndex idx);
int Texture_GetHeight(TextureIndex idx);

#endif