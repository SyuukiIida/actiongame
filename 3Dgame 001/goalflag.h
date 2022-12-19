#ifndef _GOALFLAG_H_
#define _GOALFLAG_H_

//障害物の状態
typedef enum
{
	GOALSTATE_NORMAL = 0,		//通常状態
	GOALSTATE_GOAL,			//ダメージ状態
	GOALSTATE_MAX
}GOALSTATE;

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;						//位置	
	D3DXVECTOR3 rot;						//向き
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3	vtxMaxModel;
	GOALSTATE state;						//状態
	int nCounterState;						//状態管理カウンター
}GoalFlag;

//プロトタイプ宣言
void InitGoalFlag(void);
void UninitGoalFlag(void);
void UpdateGoalFlag(void);
void DrawGoalFlag(void);
bool CollisionGoalFlagPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold);

#endif