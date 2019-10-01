#include <d3d9.h>
#include <d3dx9.h>
#include "mydirect3d.h"
#include "texture.h"

//�\����
typedef struct Vertex2D_teg
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}Vertex2D;

#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static D3DCOLOR g_Color = 0xffffffff;

void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
		return;

	// enable alpha
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�o�[�e�b�N�X�o�b�t�@���쐬����
	pDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX2D,
		D3DPOOL_DEFAULT,
		&g_pVertexBuffer, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIndexBuffer, NULL);

	//�C���f�b�N�X�o�b�t�@���_�C���f�b�N�X���Z�b�g
	LPWORD pIndex; //�C���f�b�N�X�z��̃|�C���^������|�C���^�ϐ�
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);//�C���f�b�N�X�o�b�t�@���Ǘ�����C���f�b�N�X�z��̃|�C���^���󂯎��
	pIndex[0] = 0;//
	pIndex[1] = 1;//�O�p�`��\������̂Ɏg�����_�ԍ��@�P����
	pIndex[2] = 2;//
	////////////////////////////////////////////////////
	pIndex[3] = 1;
	pIndex[4] = 3;//�O�p�`��\������̂Ɏg�����_�ԍ��@�P����
	pIndex[5] = 2;

	g_pIndexBuffer->Unlock();
}

void Sprite_Finalize(void)
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	//VertexBuffer�������悤�Ƀ����[�X
}

void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Draw(TextureIndex texture_index, float dx,float dy,int tx, int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
		return;

	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	//�e�N�X�`���[
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	Vertex2D* pV;//�o�[�e�b�N�X�o�b�t�@�̔z��̃|�C���^�̓��ꕨ

	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].position = D3DXVECTOR4(dx - 0.5F, dy - 0.5F, 0.0F, 1.0F);
	pV[1].position = D3DXVECTOR4(dx + tw - 0.5F, dy - 0.5F, 0.0F, 1.0F);
	pV[2].position = D3DXVECTOR4(dx - 0.5F, dy + th - 0.5F, 0.0F, 1.0F);
	pV[3].position = D3DXVECTOR4(dx + tw - 0.5F, dy + th - 0.5F, 0.0F, 1.0F);

	//�J���[
	for (int i = 0; i < 4; i++)
	{
		pV[i].color = g_Color;
	}

	pV[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pV[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pV[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pV[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock();//�������ݏI����DirectX�֋�����A�K��Lock��������Unllock���邱��

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0,//�\���������o�[�e�b�N�X�o�b�t�@
		sizeof(Vertex2D));//���_1�̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer);//�\���Ɏg���C���f�b�N�X�o�b�t�@

	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0,
		4, 0,//�\���Ɏg�����_�̐�
		2);	//�\������|���S���̐�

}