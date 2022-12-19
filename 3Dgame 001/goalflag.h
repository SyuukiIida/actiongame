#ifndef _GOALFLAG_H_
#define _GOALFLAG_H_

//��Q���̏��
typedef enum
{
	GOALSTATE_NORMAL = 0,		//�ʏ���
	GOALSTATE_GOAL,			//�_���[�W���
	GOALSTATE_MAX
}GOALSTATE;

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;						//�ʒu	
	D3DXVECTOR3 rot;						//����
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3	vtxMaxModel;
	GOALSTATE state;						//���
	int nCounterState;						//��ԊǗ��J�E���^�[
}GoalFlag;

//�v���g�^�C�v�錾
void InitGoalFlag(void);
void UninitGoalFlag(void);
void UpdateGoalFlag(void);
void DrawGoalFlag(void);
bool CollisionGoalFlagPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold);

#endif