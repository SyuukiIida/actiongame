#ifndef _WALL_H_
#define _WALL_H_

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
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif 

