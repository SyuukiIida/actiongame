#ifndef _EFFECT_H_
#define _EFFECT_H_

#define MAX_EFFECT        (4096)    //エフェクトの最大数

typedef struct
{
	D3DVECTOR pos;
	D3DVECTOR rot;
	D3DXCOLOR col;
	int nLife;
	int nLifeTemp;
	bool bUse;
}Effect;

void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife);

#endif