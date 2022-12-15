#include "main.h"
#include "result.h"
#include "input.h"
#include "resultjudge.h"
#include "fade.h"
//#include "score.h"



//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureresult = NULL; //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffresult = NULL;//���_�o�b�t�@�̃|�C���^
RESULT g_Result;
bool Bonus = false;

//====================================================================
//�|���S���̏���������
//====================================================================

void InitResult(RESULT result)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Result = result;
		
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\water001.jpg",
		&g_apTextureresult);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffresult,
		NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffresult->Lock(0, 0, (void**)&pVtx, 0);



	//���_���W�̐ݒ�	
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;


	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	g_pVtxBuffresult->Unlock();

	//�Q�[�����ʏ���������
	InitResultJudge(g_Result);

}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitResult(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureresult != NULL)
	{			   
		g_apTextureresult->Release();
		g_apTextureresult = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffresult != NULL)
	{			  
		g_pVtxBuffresult->Release();
		g_pVtxBuffresult = NULL;
	}

	//�Q�[�����ʏI������
	UninitResultJudge();

}
//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateResult(void)
{
	FADE pFade;		//�t�F�[�h�ւ̃|�C���^
	pFade = GetFade();

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffresult->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffresult->Unlock();

	//�Q�[�����ʍX�V����
	UpdateResultJudge();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//����L�[�������ꂽ

		 //���[�h�ݒ�(�Q�[����ʂɈڍs)
			SetFade(MODE_TITLE);
		}
	}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{//����L�[�iENTER�j�������ꂽ
	// //���[�h�ݒ�i�Q�[����ʂɈڍs�j
	//	SetMode(MODE_TITLE);
	//}
}

//====================================================================
//�|���S���̕`�揈��
//====================================================================

void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffresult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	pDevice->SetTexture(0, g_apTextureresult);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u�̎��	


	//�Q�[�����ʕ`�揈��
	DrawResultJudge();

	

}

