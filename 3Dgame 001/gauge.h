#ifndef _GAUGE_H_
#define _GAUGE_H_

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
}Gauge;

//�v���g�^�C�v�錾
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

#endif 
