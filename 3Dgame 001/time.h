#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;								//タイムの位置


}TIME;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);


#endif
