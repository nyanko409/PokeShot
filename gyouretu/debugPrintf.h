#ifndef _DEBUG_PRINTF_H
#define _DEBUG_PRINTF_H

#include <Windows.h>
#include <stdio.h>

//デバッグ用printf
inline void DebugPrintf(const char* pFormat)
{
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);	//バッファ量と第2引数を合わせること
	va_end(argp);
	OutputDebugStringA(buf);

#endif _DEBUG || DEBUG
}

#endif _DEBUG_PRINTF_H
