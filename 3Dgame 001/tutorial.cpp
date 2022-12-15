
//
//
#include "main.h"
#include "tutorial.h"
#include "input.h"

//�}�N����`
#define NUM_TUTORIAL				(1)			//	�`���[�g���A���̐�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureTUTORIAL[NUM_TUTORIAL] = {}; //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTUTORIAL = NULL;//���_�o�b�t�@�̃|�C���^
float g_aTexVt[NUM_TUTORIAL];

//====================================================================
//�|���S���̏���������
//====================================================================

void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntBG;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\water001.jpg",
		&g_apTextureTUTORIAL[0]);

	//�e�N�X�`�����W�̊J�n�ʒu�iV�l�j�̏�����
	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		g_aTexVt[nCntBG] = 0.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTUTORIAL,
		NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTUTORIAL->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG] + 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffTUTORIAL->Unlock();
}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitTutorial(void)
{

	for (int nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureTUTORIAL[nCntBG] != NULL)
		{
			g_apTextureTUTORIAL[nCntBG]->Release();
			g_apTextureTUTORIAL[nCntBG] = NULL;
		}
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffTUTORIAL != NULL)
	{
		g_pVtxBuffTUTORIAL->Release();
		g_pVtxBuffTUTORIAL = NULL;
	}
}
//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateTutorial(void)
{
	int nCntBG;

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTUTORIAL->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		//g_aTexV[nCntBG] -=0.0005f*((nCntBG+1)*3);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG] + 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffTUTORIAL->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//����L�[�iENTER�j�������ꂽ
	 //���[�h�ݒ�i�Q�[����ʂɈڍs�j
		SetMode(MODE_GAME);
	}
}

//====================================================================
//�|���S���̕`�揈��
//====================================================================

void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCntBG;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTUTORIAL, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTUTORIAL[nCntBG]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBG, 2);	//�v���~�e�B�u�̎��	
	}

}