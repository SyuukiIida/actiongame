#ifndef _MODEL_H_
#define _MODEL_H_

//モデル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;							//現在の位置
	D3DXVECTOR3 posold;							//前回の位置
	D3DXVECTOR3 rot;							//向き
	D3DXVECTOR3 move;						//移動量
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
}Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model GetModel(void);

#endif 
