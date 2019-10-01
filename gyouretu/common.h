#pragma once

#include<d3d9.h>		//DirectX�̓z��I��
#include<d3dx9.h>		//���̓�͍Œ���K�v

/*-----------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------*/
#define CLASS_NAME      "GameWindow"
#define WINDOW_CAPTION  "PokeShot"

#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // �QD�|���S�����_�t�H�[�}�b�g

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
