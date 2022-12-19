#ifndef _GAUGE_H_
#define _GAUGE_H_

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
}Gauge;

//プロトタイプ宣言
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

#endif 
