#ifndef _MODEL_H_
#define _MODEL_H_

//�}�N����`
#define MAX_GAUGE			(25.0f)				//�Q�[�W�ő�l
#define COOLTIME			(1)					//�V���b�g�N�[���^�C��

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
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player GetPlayer(void);

#endif 
