/*=======================================================================

  windowsの表示[main.cpp]
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


//ライブラリファイルのリンク（exeファイルに含める）
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")


/*-----------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//ゲーム関係初期化関数
static bool Initialize(void);
static void InitGame();

//ゲーム更新関数
static void Update(void);
static void MainScreenUpdate();
static void ResultScreenUpdate();

// collision physics
static void UpdatePhysics();

//ゲーム描画関数
static void Draw(void);
static void MainScreenDraw();
static void ResultScreenDraw();

//ゲーム終了の関数
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
   メイン
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
	//ウィンドウスタイル（見た目）の設定
	DWORD window_style = WS_OVERLAPPEDWINDOW;// &~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	//基本矩形座標
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//指定したクライアント領域に合わせて矩形を調整
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//ウィンドウの矩形座標から幅と高さを計算
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//ディスプレイの解像度の取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//画面の中にウィンドウを表示するよう座標計算
	//ただし画面より大きいウィンドウは左上に合わせて表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);
	//※max(A,B)はAとBで大きさを返す関数

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

	//ウィンドウが作成できなかったら終了
	if (g_hWnd == NULL)
	{
		return 0;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// init
	if (!Initialize())
	{
		//初期化失敗
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

	//ゲームの終了処理
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
		if (MessageBox(g_hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
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
   ゲームの初期化
------------------------------------------------------------------------*/
bool Initialize(void)
{
	//mydirect 68行目？
	//Direct3Dインターフェイス作成
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

//ゲーム更新関数
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

//ゲームの描画処理
void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	
	//バックバッファーのクリア 紫色は230，0，255，255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0F, 0);

	//絵画処理開始
	pDevice->BeginScene();

	DrawBg();
	DrawEnemy();
	DrawBall();
	DrawTime();
	DrawLine_ClickPos(pDevice);
	DrawCursor();

	pDevice->EndScene();

	//バックバッファを画面に表示する
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//終了処理
void Finalize(void)
{
	UninitBall();
	UninitBg();
	UninitEnemy();

	UninitSound();
	Keyboard_Finalize();
	Sprite_Finalize();
	TextureRelease();
	
	MyDirect3D_Finalize();//DirectXの終了
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

//フォントの初期化
HRESULT InitFont(HWND hWnd)
{
	//フォントの生成 MSゴシック
	//デバイス,文字高さ,文字幅,フォントスタイル,ミップマップのレベル,斜体にするかどうか,文字セット,出力精度,出力品質,フォントピッチとファミリ,フォント名,フォントポインタ
	HRESULT hr = D3DXCreateFont(MyDirect3D_GetDevice(), 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &g_pFont);
	if FAILED(hr) { return(E_FAIL); }

	hr = D3DXCreateFont(MyDirect3D_GetDevice(), 300, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &g_pBigFont);
	if FAILED(hr) { return(E_FAIL); }

	return S_OK;
}

void MainScreenDraw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//バックバッファーのクリア 紫色は230，0，255，255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0F, 0);

	//絵画処理開始
	pDevice->BeginScene();

	DrawMainScreen();

	pDevice->EndScene();

	//バックバッファを画面に表示する
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

	//バックバッファーのクリア 紫色は230，0，255，255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0F, 0);

	//絵画処理開始
	pDevice->BeginScene();

	DrawResultScreen();

	pDevice->EndScene();

	//バックバッファを画面に表示する
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void ResultScreenUpdate()
{
	Keyboard_Update();
	UpdateResultScreen();
}
