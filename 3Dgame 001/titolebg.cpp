


#include "main.h"
#include "input.h"
#include "titlebg.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureTitleBg = NULL; //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleBg = NULL;//���_�o�b�t�@�̃|�C���^

//====================================================================
//�^�C�g���w�i�̏���������
//====================================================================

void InitTitleBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\water001.jpg",
		&g_apTextureTitleBg);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleBg,
		NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);



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

	g_pVtxBuffTitleBg->Unlock();
}

//====================================================================
//�^�C�g���w�i�̏I������
//====================================================================

void UninitTitleBg(void)
{
	//�e�N�X�`���̔j��


	if (g_apTextureTitleBg != NULL)
	{
		g_apTextureTitleBg->Release();
		g_apTextureTitleBg = NULL;
	}


	//�o�b�t�@�̔j��
	if (g_pVtxBuffTitleBg != NULL)
	{
		g_pVtxBuffTitleBg->Release();
		g_pVtxBuffTitleBg = NULL;
	}
}
//====================================================================
//�^�C�g���w�i�̍X�V����
//====================================================================

void UpdateTitleBg(void)
{
	//VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	//							//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	//g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	////�e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//g_pVtxBuffTitleBg->Unlock();

}

//====================================================================
//�^�C�g���w�i�̕`�揈��
//====================================================================

void DrawTitleBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureTitleBg);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u�̎��		

}