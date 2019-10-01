#ifndef _DEBUG_PRINTF_H
#define _DEBUG_PRINTF_H

#include <Windows.h>
#include <stdio.h>

//�f�o�b�O�pprintf
inline void DebugPrintf(const char* pFormat)
{
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);	//�o�b�t�@�ʂƑ�2���������킹�邱��
	va_end(argp);
	OutputDebugStringA(buf);

#endif _DEBUG || DEBUG
}

#endif _DEBUG_PRINTF_H
