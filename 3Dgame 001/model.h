#ifndef _MODEL_H_
#define _MODEL_H_

//���f���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;							//���݂̈ʒu
	D3DXVECTOR3 posold;							//�O��̈ʒu
	D3DXVECTOR3 vtxMinModel;					//���_���ŏ��l
	D3DXVECTOR3 vtxMaxModel;					//���_���ő�l
	D3DXVECTOR3 vtxMinModelold;					//�O��̒��_���ŏ��l
	D3DXVECTOR3 vtxMaxModelold;					//�O��̒��_���ő�l
	D3DXVECTOR3 rot;							//����
	D3DXVECTOR3 move;							//�ړ���
	float gauge;								//�Q�[�W��
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
}Model;

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model GetModel(void);
float SetGauge(void);

#endif 
