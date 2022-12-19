#ifndef _WALL_H_
#define _WALL_H_

//���肷����W
typedef enum
{
	COLLISIONWALL_Z = 0,		//Y�̓����蔻�������
	COLLISIONWALL_X,			//X�̓����蔻�������
	COLLISIONWALL_MAX
}COLLISIONWALL;

//���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
}Wall;

//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWallBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionWallPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold
	, COLLISIONWALL collisionWall);

#endif 

