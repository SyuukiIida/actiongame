#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//マクロ定義
#define OBSTACLE_SIZE		(20)		//障害物の当たる範囲

//判定する座標
typedef enum
{
	COLLISIONBLOCK_Y = 0,		//Yの当たり判定をする
	COLLISIONBLOCK_X,			//Xの当たり判定をする
	COLLISIONBLOCK_MAX
}COLLISIONOBSTACLE;

//モデル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 rot;							//向き
	D3DXVECTOR3 move;						//移動量
	int nLife;								//体力
	bool bUse;								//使っているか
}Obstacle;

//プロトタイプ宣言
void InitObstacle(void);
void UninitObstacle(void);
void UpdateObstacle(void);
void DrawObstacle(void);
void HitObstacle(int nCntObstacle, int nDamage);
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif 
