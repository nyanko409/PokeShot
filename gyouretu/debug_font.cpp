#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "common.h"
#include "mydirect3d.h"

//グローバル変数
#define DEBUG_PRINTF_BUFFER_MAX (100)

#if defined(_DEBUG) || defined(DEBUG)

static LPD3DXFONT g_pD3DXFont = NULL;

#endif _DEBUG || DEBUG	//デバッグビルドのみ有効

void DebugFont_Initialize()
{
#if defined(_DEBUG) || defined(DEBUG)

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXCreateFont(pDevice, 24, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"HGP創英角ﾎﾟｯﾌﾟ体", &g_pD3DXFont);

#endif
}

void DebugFont_Finalize()
{
#if defined(_DEBUG) || defined(DEBUG)

	if (g_pD3DXFont)
	{
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}

#endif
}

void DebugFont_Draw(int x, int y, const char * pFormat, ...)
{
#if defined(_DEBUG) || defined(DEBUG)

	RECT rect = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT };
	va_list argp;
	va_start(argp, pFormat);
	char buf[DEBUG_PRINTF_BUFFER_MAX];
	vsnprintf(buf, DEBUG_PRINTF_BUFFER_MAX , pFormat, argp);
	va_end(argp);
	g_pD3DXFont->DrawText(NULL, buf, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 255, 0, 255));

#else

	UNREFERENCED_PARAMETER(pFormat);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);

#endif
}