#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//�}�N����`

//���肷����W
typedef enum
{
	COLLISIONOBSTACLE_Z = 0,		//Y�̓����蔻�������
	COLLISIONOBSTACLE_X,			//X�̓����蔻�������
	COLLISIONOBSTACLE_MAX
}COLLISIONOBSTACLE;

//��Q���̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//�ʏ���
	ENEMYSTATE_DAMAGE,			//�_���[�W���
	ENEMYSTATE_MAX
}OBSTACLESTATE;

//���f���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;						//�ʒu
	D3DXVECTOR3 vtxMinModel;				//���_���ŏ��l
	D3DXVECTOR3 vtxMaxModel;				//���_���ő�l
	D3DXVECTOR3 rot;						//����
	D3DXVECTOR3 move;						//�ړ���
	OBSTACLESTATE state;					//���
	int nCounterState;						//��ԊǗ��J�E���^�[
	int nLife;								//�̗�
	int type = -1;							//���
	bool bUse;								//�g���Ă��邩
}Obstacle;

//�v���g�^�C�v�錾
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
