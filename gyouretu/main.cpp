/*=======================================================================

  windows�̕\��[main.cpp]
			 Author : Ato
			 Date   : 2018/4/22
-------------------------------------------------------------------------

========================================================================*/
#include<Windows.h>
#include <chrono>
#include <thread>
#include "common.h"
#include "debug_font.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"
#include "input.h"
#include "bg.h"
#include "sound.h"
#include "camera.h"
#include "ball.h"
#include "enemy.h"
#include "time.h"
#include "cursor.h"
#include "scene_management.h"


//���C�u�����t�@�C���̃����N�iexe�t�@�C���Ɋ܂߂�j
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")


/*-----------------------------------------------------------------------
   �v���g�^�C�v�錾
------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�Q�[���֌W�������֐�
static bool Initialize(void);
static void InitGame();

//�Q�[���X�V�֐�
static void Update(void);
static void MainScreenUpdate();
static void ResultScreenUpdate();

// collision physics
static void UpdatePhysics();

//�Q�[���`��֐�
static void Draw(void);
static void MainScreenDraw();
static void ResultScreenDraw();

//�Q�[���I���̊֐�
static void Finalize(void);

void DrawLine_BallPos(LPDIRECT3DDEVICE9& g_pD3DDevice);
void DrawLine_ClickPos(LPDIRECT3DDEVICE9& g_pD3DDevice);
HRESULT InitFont(HWND hWnd);

// global variables
constexpr unsigned int FPS = 30;
std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

bool gameInitialized;
static HWND g_hWnd;
LPD3DXFONT g_pFont = NULL;
LPD3DXFONT g_pBigFont = NULL;





/*-----------------------------------------------------------------------
   ���C��
------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);
	//�E�B���h�E�X�^�C���i�����ځj�̐ݒ�
	DWORD window_style = WS_OVERLAPPEDWINDOW;// &~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	//��{��`���W
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�w�肵���N���C�A���g�̈�ɍ��킹�ċ�`�𒲐�
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//�E�B���h�E�̋�`���W���畝�ƍ������v�Z
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//�f�B�X�v���C�̉𑜓x�̎擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//��ʂ̒��ɃE�B���h�E��\������悤���W�v�Z
	//��������ʂ��傫���E�B���h�E�͍���ɍ��킹�ĕ\��
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);
	//��max(A,B)��A��B�ő傫����Ԃ��֐�

	g_hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//�E�B���h�E���쐬�ł��Ȃ�������I��
	if (g_hWnd == NULL)
	{
		return 0;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// init
	if (!Initialize())
	{
		//���������s
		return -1;
	}
	Keyboard_Initialize(hInstance,g_hWnd);
	PlaySound(SOUND_LABEL_BGM_MAINMENU);

	MSG msg = {};

	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Maintain designated frequency of 5 Hz (200 ms per frame)
			a = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> work_time = a - b;

			if (work_time.count() < 1.0F/FPS * 1000)
			{
				std::chrono::duration<double, std::milli> delta_ms(1.0F / FPS * 1000 - work_time.count());
				auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
				std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
			}

			b = std::chrono::system_clock::now();

			// main screen
			if (currentScene == MainMenuScreen)
			{
				MainScreenUpdate();
				MainScreenDraw();
			}

			// game screen
			if(currentScene == GameScreen)
			{ 
				if (!gameInitialized)
				{
					InitGame();
					gameInitialized = true;
				}

				Update();
				Draw();
			}

			// result screen
			if (currentScene == ResultScreen)
			{
				gameInitialized = false;
				ResultScreenUpdate();
				ResultScreenDraw();
			}
		}
	}

	//�Q�[���̏I������
	Finalize();
	DebugFont_Finalize();
	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		ShowCursor(true);
		if (MessageBox(g_hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(g_hWnd);
		}
		ShowCursor(false);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(g_hWnd, uMsg, wParam, lParam);
}

/*-----------------------------------------------------------------------
   �Q�[���̏�����
------------------------------------------------------------------------*/
bool Initialize(void)
{
	//mydirect 68�s�ځH
	//Direct3D�C���^�[�t�F�C�X�쐬
	if (!MyDirect3D_Initialize(g_hWnd))
	{
		return false;
	}

	DebugFont_Initialize();
	Sprite_Initialize();
	Texture_Load();
	InitSound(g_hWnd);
	InitFont(g_hWnd);
	
	InitCursor();
	InitBg();

	return true;
}

void InitGame()
{
	InitBall();
	InitEnemy();
	InitTime();
}

//�Q�[���X�V�֐�
void Update(void)
{
	Keyboard_Update();

	UpdateBall();
	UpdateEnemy();
	UpdateBg();
	UpdateTime();
	UpdateCursor();

	UpdatePhysics();
}

// update collision
void UpdatePhysics()
{
	
}

//�Q�[���̕`�揈��
void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	
	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0F, 0);

	//�G�揈���J�n
	pDevice->BeginScene();

	DrawBg();
	DrawEnemy();
	DrawBall();
	DrawTime();
	DrawLine_ClickPos(pDevice);
	DrawCursor();

	pDevice->EndScene();

	//�o�b�N�o�b�t�@����ʂɕ\������
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//�I������
void Finalize(void)
{
	UninitBall();
	UninitBg();
	UninitEnemy();

	UninitSound();
	Keyboard_Finalize();
	Sprite_Finalize();
	TextureRelease();
	
	MyDirect3D_Finalize();//DirectX�̏I��
}

void DrawLine_BallPos(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	// return if player is not aiming
	if (!isDragging) return;

	// get start and current pos to draw the line
	Ball* ball = GetBall();
	v2i ballCenter = { ball->getPosition().x + Texture_GetWidth(TEXTURE_INDEX_BALL) / 2, 
					   ball->getPosition().y + Texture_GetHeight(TEXTURE_INDEX_BALL) / 2 };

	v2i diff = currentPos - startPos;
	int mag = diff.getMagnitude() / 3.5F;
	if (mag > 255) mag = 255;

	// draw the line
	Vertex2D v[] = { D3DXVECTOR4(ballCenter.x,ballCenter.y, 0.0F, 1.0F), D3DCOLOR(D3DCOLOR_RGBA(mag, 255 - mag, 0, 255)),
					  D3DXVECTOR4(ballCenter.x + diff.x, ballCenter.y + diff.y, 0.0F, 1.0F), D3DCOLOR(D3DCOLOR_RGBA(mag, 255 - mag, 0, 255))};

	
	g_pD3DDevice->SetFVF(FVF_VERTEX2D);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, v, sizeof(Vertex2D));
}

void DrawLine_ClickPos(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	// return if player is not aiming
	if (!isDragging) return;

	// get start and current pos to draw the line
	Ball* ball = GetBall();

	v2i diff = currentPos - startPos;
	int mag = diff.getMagnitude() / 3.5F;
	if (mag > 255) mag = 255;

	// offset startpos to match position with cursor
	v2i finalPos = startPos - v2i{40, 70};

	// draw the line
	Vertex2D v[] = { D3DXVECTOR4(finalPos.x, finalPos.y, 0.0F, 1.0F), D3DCOLOR(D3DCOLOR_RGBA(mag, 255 - mag, 0, 255)),
					  D3DXVECTOR4(finalPos.x + diff.x, finalPos.y + diff.y, 0.0F, 1.0F), D3DCOLOR(D3DCOLOR_RGBA(mag, 255 - mag, 0, 255)) };

	

	g_pD3DDevice->SetFVF(FVF_VERTEX2D);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, v, sizeof(Vertex2D));
}

//�t�H���g�̏�����
HRESULT InitFont(HWND hWnd)
{
	//�t�H���g�̐��� MS�S�V�b�N
	//�f�o�C�X,��������,������,�t�H���g�X�^�C��,�~�b�v�}�b�v�̃��x��,�Α̂ɂ��邩�ǂ���,�����Z�b�g,�o�͐��x,�o�͕i��,�t�H���g�s�b�`�ƃt�@�~��,�t�H���g��,�t�H���g�|�C���^
	HRESULT hr = D3DXCreateFont(MyDirect3D_GetDevice(), 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "�l�r �S�V�b�N", &g_pFont);
	if FAILED(hr) { return(E_FAIL); }

	hr = D3DXCreateFont(MyDirect3D_GetDevice(), 300, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "�l�r �S�V�b�N", &g_pBigFont);
	if FAILED(hr) { return(E_FAIL); }

	return S_OK;
}

void MainScreenDraw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0F, 0);

	//�G�揈���J�n
	pDevice->BeginScene();

	DrawMainScreen();

	pDevice->EndScene();

	//�o�b�N�o�b�t�@����ʂɕ\������
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void MainScreenUpdate()
{
	Keyboard_Update();
	UpdateMainScreen();
}

void ResultScreenDraw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0F, 0);

	//�G�揈���J�n
	pDevice->BeginScene();

	DrawResultScreen();

	pDevice->EndScene();

	//�o�b�N�o�b�t�@����ʂɕ\������
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void ResultScreenUpdate()
{
	Keyboard_Update();
	UpdateResultScreen();
}
