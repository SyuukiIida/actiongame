#ifndef _BULLET_H_
#define _BULLET_H_

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	int nLife;
	bool bUse;
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(void);

#endif 

