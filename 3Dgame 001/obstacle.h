#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//マクロ定義

//判定する座標
typedef enum
{
	COLLISIONOBSTACLE_Z = 0,		//Yの当たり判定をする
	COLLISIONOBSTACLE_X,			//Xの当たり判定をする
	COLLISIONOBSTACLE_MAX
}COLLISIONOBSTACLE;

//障害物の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常状態
	ENEMYSTATE_DAMAGE,			//ダメージ状態
	ENEMYSTATE_MAX
}OBSTACLESTATE;

//モデル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 vtxMinModel;				//頂点情報最小値
	D3DXVECTOR3 vtxMaxModel;				//頂点情報最大値
	D3DXVECTOR3 rot;						//向き
	D3DXVECTOR3 move;						//移動量
	OBSTACLESTATE state;					//状態
	int nCounterState;						//状態管理カウンター
	int nLife;								//体力
	int type = -1;							//種類
	bool bUse;								//使っているか
}Obstacle;

//プロトタイプ宣言
void InitObstacle(void);
void UninitObstacle(void);
void UpdateObstacle(void);
void DrawObstacle(void);
void HitObstacle(int nCntObstacle, int nDamage);
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	,D3DXVECTOR3 *vtxMaxModel,D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold
	, COLLISIONOBSTACLE collisionObstacle);
void LoadObstacle(void);

#endif 
