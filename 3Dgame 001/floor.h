#ifndef _FLOOR_H_
#define _FLOOR_H_

#define FLOOR_SIZE			(300)			//地面の大きさ

//床の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
}Floor;

//プロトタイプ宣言
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);

#endif 

