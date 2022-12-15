


#include "main.h"
#include "game.h"
#include "input.h"
#include "polygon.h"
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "wall.h"
#include "floor.h"
#include "bullet.h"
#include "meshfield.h"
#include "obstacle.h"
#include "camera.h"
#include "light.h"
#include "fade.h"



GAMESTATE g_gameState = GAMESTATE_NONE;	//�Q�[���̏��
int g_nCounterGameState=0;
RESULT g_gameresult;
bool g_bPause = false;

//====================================================================
//�Q�[����ʂ̏���������
//====================================================================
void InitGame(void)
{
	//�t�@�C���ǂݍ���
	LoadObstacle();

	//�ǂ̏���������
	InitWall();

	//���̏���������
	InitFloor();
	//InitMeshfield();

	//�e�̏���������
	InitShadow();

	//�r���{�[�h����������
	InitBillboard();

	//���f���̏���������
	InitModel();
	InitObstacle();

	//�e�̏���������
	InitBullet();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//��Ԃ̏�����
	g_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃɐݒ�

	g_nCounterGameState = 0;

	g_bPause = false;

	//�T�E���h�ݒ�
	//PlaySound(SOUND_LABEL_BGM000);

}

//====================================================================
//�Q�[����ʂ̏I������
//====================================================================
void UninitGame(void)
{
	//���̏I������
	UninitFloor();
	//UninitMeshfield();

	//�ǂ̏I������
	UninitWall();

	//�r���{�[�h�I������
	UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�e�̏I������
	UninitShadow();

	//���f���̏I������
	UninitModel();
	UninitObstacle();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();
}

//====================================================================
//�Q�[����ʂ̍X�V����
//====================================================================
void UpdateGame(void)
{
	//���̍X�V����
	UpdateFloor();
	//UpdateMeshfield();

	//�ǂ̍X�V����
	UpdateWall();

	//�r���{�[�h�X�V����
	UpdateBillboard();

	//���f���̍X�V����
	UpdateModel();
	UpdateObstacle();

	//�e�̍X�V����
	UpdateBullet();

	//�e�̍X�V����
	UpdateShadow();

	//�J�����̍X�V����
	UpdateCamera();

	//���C�g�̍X�V����
	UpdateLight();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//����L�[�iENTER�j�������ꂽ
	 //�Q�[���̏�Ԃ�ݒ�
		SetGameState(GAMESTATE_END, 30, RESULT_CREAR);
		//���[�h�ݒ�i�Q�[����ʂɈڍs�j
		//SetMode(MODE_RESULT);
	}
#endif
	

	switch(g_gameState)
	{

	case GAMESTATE_NORMAL:		//�ʏ���
		break;
	case GAMESTATE_END:			//�I��
		g_nCounterGameState--;

		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;	//�������Ă��Ȃ���Ԃɂ���
			//���[�h�ݒ�i���U���g��ʂɈڍs�j
			SetFade(MODE_RESULT);
		}

		break;
	}

	//if (�G�S�� || �v���C���[��)
	//{
	//	//���[�h�ݒ�i���U���g��ʂɈڍs�j
	//	SetMode(MODE_RESULT);
	//}
}

//====================================================================
//�Q�[����ʂ̕`�揈��
//====================================================================
void DrawGame(void)
{
	
	//���̍X�V����
	DrawFloor();
	//DrawMeshfield();

	//�ǂ̕`�揈��
	DrawWall();



	//���f���̕`�揈��
	DrawModel();
	DrawObstacle();

	//�e�̕`�揈��
	DrawShadow();

	//�r���{�[�h�`�揈��
	DrawBillboard();

	//�e�̕`�揈��
	DrawBullet();

}
//====================================================================
//�Q�[���̏�Ԃ̐ݒ�
//====================================================================
void SetGameState(GAMESTATE state, int nCounter, RESULT Result)
{
	g_gameState = state;
	g_gameresult = Result;
	g_nCounterGameState = nCounter;
}

//====================================================================
//�Q�[���̏�Ԃ̎擾
//====================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

RESULT GetResult(void)
{
	return g_gameresult;
}
