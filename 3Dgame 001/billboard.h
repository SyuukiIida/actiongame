#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
}Billboard;

//�v���g�^�C�v�錾
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);

#endif 

