#pragma once

void DebugFont_Initialize(void);	//初期化
void DebugFont_Finalize(void);		//終了

//デバッグ文字表示
//pFormat : printfとほぼ同じように表示を指定する
void DebugFont_Draw(int x, int y, const char* pFormat, ...);