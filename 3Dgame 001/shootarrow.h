#ifndef _SHOOTARROW_H_
#define _SHOOTARROW_H_

//�e�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	bool bUse;
}ShootArrow;

//�v���g�^�C�v�錾
void InitShootArrow(void);
void UninitShootArrow(void);
void UpdateShootArrow(void);
void DrawShootArrow(void);
void SetPositionShootArrow(D3DXVECTOR3 pos);

#endif 

