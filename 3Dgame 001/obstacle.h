#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//�}�N����`
#define OBSTACLE_SIZE		(20)		//��Q���̓�����͈�

//���肷����W
typedef enum
{
	COLLISIONBLOCK_Y = 0,		//Y�̓����蔻�������
	COLLISIONBLOCK_X,			//X�̓����蔻�������
	COLLISIONBLOCK_MAX
}COLLISIONOBSTACLE;

//���f���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 rot;							//����
	D3DXVECTOR3 move;						//�ړ���
	int nLife;								//�̗�
	bool bUse;								//�g���Ă��邩
}Obstacle;

//�v���g�^�C�v�錾
void InitObstacle(void);
void UninitObstacle(void);
void UpdateObstacle(void);
void DrawObstacle(void);
void HitObstacle(int nCntObstacle, int nDamage);
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif 
