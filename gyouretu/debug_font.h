#pragma once

void DebugFont_Initialize(void);	//������
void DebugFont_Finalize(void);		//�I��

//�f�o�b�O�����\��
//pFormat : printf�Ƃقړ����悤�ɕ\�����w�肷��
void DebugFont_Draw(int x, int y, const char* pFormat, ...);