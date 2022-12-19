#ifndef _SHOOTARROW_H_
#define _SHOOTARROW_H_

//影の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	bool bUse;
}ShootArrow;

//プロトタイプ宣言
void InitShootArrow(void);
void UninitShootArrow(void);
void UpdateShootArrow(void);
void DrawShootArrow(void);
void SetPositionShootArrow(D3DXVECTOR3 pos);

#endif 

