#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//�\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;								//�^�C���̈ʒu


}TIME;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);


#endif
