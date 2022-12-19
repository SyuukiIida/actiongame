#include "main.h"
#include "input.h"
#include "resultjudge.h"
#include "game.h"


//�}�N����`
#define NUM_JUDGE				(2)			//	�W���b�`�̐�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureResultJudge[NUM_JUDGE] = {}; //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultJudge = NULL;//���_�o�b�t�@�̃|�C���^
RESULT g_result;

//====================================================================
//�|���S���̏���������
//====================================================================

void InitResultJudge()
{
	g_result = GetResult();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\crear000.png",
		&g_apTextureResultJudge[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameover000.png",
		&g_apTextureResultJudge[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_JUDGE ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultJudge,
		NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResultJudge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntJudge = 0; nCntJudge < NUM_JUDGE; nCntJudge++,pVtx+=4)
	{
		//���_���W�̐ݒ�	
		pVtx[0].pos = D3DXVECTOR3(425.0f, 240.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(855.0f, 240.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(425.0f, 480.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(855.0f, 480.0f, 0.0f);


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
	}

	


	g_pVtxBuffResultJudge->Unlock();

}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitResultJudge(void)
{
	for (int nCntJudge = 0; nCntJudge < NUM_JUDGE; nCntJudge++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureResultJudge[nCntJudge] != NULL)
		{
			g_apTextureResultJudge[nCntJudge]->Release();
			g_apTextureResultJudge[nCntJudge] = NULL;
		}
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffResultJudge != NULL)
	{
		g_pVtxBuffResultJudge->Release();
		g_pVtxBuffResultJudge = NULL;
	}

}

//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateResultJudge(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResultJudge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += g_result * 4;

	//���_���W�̐ݒ�	
	pVtx[0].pos = D3DXVECTOR3(425.0f, 240.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(855.0f, 240.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(425.0f, 480.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(855.0f, 480.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffResultJudge->Unlock();

}

//====================================================================
//�|���S���̕`�揈��
//====================================================================

void DrawResultJudge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultJudge, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�


	/*pDevice->SetTexture(0, g_apTextureBG[1]);

	pDevice->SetTexture(0, g_apTextureBG[2]);*/

	//�|���S���̕`��
	
		pDevice->SetTexture(0, g_apTextureResultJudge[g_result]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, g_result * 4, 2);	//�v���~�e�B�u�̎��	
	
}