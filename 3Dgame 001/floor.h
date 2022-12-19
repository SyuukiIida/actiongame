#ifndef _FLOOR_H_
#define _FLOOR_H_

#define FLOOR_SIZE			(300)			//�n�ʂ̑傫��

//���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
}Floor;

//�v���g�^�C�v�錾
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);

#endif 

