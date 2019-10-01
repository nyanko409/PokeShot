#include <d3d9.h>
#include <d3dx9.h>
#include "texture.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "common.h"

#define TEXTURE_FILENAME_MAX (64)

typedef struct TextureFile_tag
{
	char filenmae[TEXTURE_FILENAME_MAX];
	int width;
	int height;
}TextureFile;

//テクスチャーの情報
static const TextureFile g_TextureFiles[] =
{
	//必要なテクスチャーのファイル名とサイズを登録
	{"asset/texture/Ball.png",80, 84},
	{"asset/texture/MetalTile.png",70, 70},
	{"asset/texture/Bg.png",SCREEN_WIDTH, SCREEN_HEIGHT},
	{"asset/texture/Enemy.png",80, 80},
	{"asset/texture/Cursor.png",20, 20},
	{"asset/texture/MainScreen.png",SCREEN_WIDTH, SCREEN_HEIGHT},
	{"asset/texture/ResultScreen.png",SCREEN_WIDTH, SCREEN_HEIGHT}
};

//登録テクスチャー数(構造体の個数)
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);

//上の数とenumに作った数が違っていたらエラーとする。
static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTUREの数がおかしいです");

//テクスチャーインターフェース保存
static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_FILE_COUNT] = {};//中身空っぽ

//テクスチャーロード
int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 pDivce = MyDirect3D_GetDevice();
	if (!pDivce)
	{
		return TEXTURE_FILE_COUNT;
	}

	int faild_count = 0;
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	{
		if (FAILED(D3DXCreateTextureFromFile(
			pDivce,
			g_TextureFiles[i].filenmae,
			&g_pTextures[i])))
		{
			//メッセージBoxやデバック表示などのエラー出力
			faild_count++;//失敗した数
		}
	}
	return faild_count;
}

void TextureRelease(void)
{
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	{
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex idx)
{
	if (idx < 0 || idx >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_pTextures[idx];
}

int Texture_GetWidth(TextureIndex idx)
{
	if (idx < 0 || idx >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_TextureFiles[idx].width;
}


int Texture_GetHeight(TextureIndex idx)
{
	if (idx < 0 || idx >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_TextureFiles[idx].height;
}