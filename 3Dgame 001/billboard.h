#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
}Billboard;

//プロトタイプ宣言
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);

#endif 

