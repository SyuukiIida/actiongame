#ifndef _SHADOW_H_
#define _SHADOW_H_

//影の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	bool bUse;
}Shadow;
					
//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetEndShadow(int nShadow);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

#endif 
