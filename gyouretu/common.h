#pragma once

#include<d3d9.h>		//DirectXの奴を選ぶ
#include<d3dx9.h>		//この二つは最低限必要

/*-----------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------*/
#define CLASS_NAME      "GameWindow"
#define WINDOW_CAPTION  "PokeShot"

#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ２Dポリゴン頂点フォーマット

#define SCREEN_WIDTH	(1360)
#define SCREEN_HEIGHT	(768)

struct Vertex2D
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
};

struct Vertex2D_UV
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
	D3DXVECTOR2 uv;
};


extern LPD3DXFONT g_pFont;
extern LPD3DXFONT g_pBigFont;
