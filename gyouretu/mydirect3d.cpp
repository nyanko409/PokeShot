#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include "common.h"

static LPDIRECT3D9 g_pD3D = NULL;	//Direct3D �C���^�[�t�F�[�X

static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3D�f�o�C�X �C���^�[�t�F�[�X


LPDIRECT3DDEVICE9 MyDirect3D_GetDevice()
{
	return g_pD3DDevice;
}

void MyDirect3D_Finalize()
{
	//�f�o�C�X�̉��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct 3D�̉��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

bool MyDirect3D_Initialize(HWND hWnd)
{
	//Direct3D�C���^�[�t�F�[�X�쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);	//�C���^�[�t�F�[�X�̃|�C���^
	if (g_pD3D == NULL)
	{//�쐬���s
		MessageBox(hWnd, "Direct3D�C���^�[�t�F�[�X�쐬���s", "Initialize Error", MB_OK);
		return false;
	}

	//�f�o�C�X�쐬�p�\���̗̂p��
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;	//Z�o�b�t�@���쐬����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct 3D�f�o�C�X�̍쐬
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{//�f�o�C�X�쐬���s
		MessageBox(hWnd, "Direct3D�f�o�C�X�쐬���s", "Initialize Error", MB_OK);
		return false;
	}

	return true;	//����������
}