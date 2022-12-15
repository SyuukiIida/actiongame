


#include "main.h"
#include "title.h"
#include "input.h"
#include "titlebg.h"
#include "titolelogo.h"
#include "fade.h"

//====================================================================
//�^�C�g����ʂ̏���������
//====================================================================
void InitTitle(void)
{
	//�^�C�g���w�i�̏���������
	InitTitleBg();

	//�^�C�g�����S�̏���������
	InitTitleLogo();
}

//====================================================================
//�^�C�g����ʂ̏I������
//====================================================================
void UninitTitle(void)
{
	//�^�C�g���w�i�̏I������
	UninitTitleBg();

	//�^�C�g�����S�̏I������
	UninitTitleLogo();

}

//====================================================================
//�^�C�g����ʂ̍X�V����
//====================================================================
void UpdateTitle(void)
{
	FADE pFade;		//�t�F�[�h�ւ̃|�C���^
	pFade = GetFade();

	//�^�C�g���w�i�̍X�V����
	UpdateTitleBg();

	//�^�C�g�����S�̍X�V����
	UpdateTitleLogo();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//����L�[�������ꂽ

		 //���[�h�ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_GAME);
		}
	}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{//����L�[�iENTER�j�������ꂽ
	// //���[�h�ݒ�i�`���[�g���A����ʂɈڍs�j
	//	SetMode(MODE_TUTORIAL);
	//}

}

//====================================================================
//�^�C�g����ʂ̕`�揈��
//====================================================================
void DrawTitle(void)
{
	//�^�C�g���w�i�̕`�揈��
	DrawTitleBg();

	//�^�C�g�����S�̕`�揈��
	DrawTitleLogo();
}


	/*D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\titlerogo.png",
		&g_apTextureTitle[1]);
*/
	