



#include "main.h"
#include "input.h"
#include "titolelogo.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureTitlelLogo = NULL; //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;//���_�o�b�t�@�̃|�C���^

//====================================================================
//�^�C�g���w�i�̏���������
//====================================================================

void InitTitleLogo(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title.png",
		&g_apTextureTitlelLogo);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(425.0f,240.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(855.0f,240.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(425.0f,480.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(855.0f,480.0f,0.0f);

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

	g_pVtxBuffTitleLogo->Unlock();
}

//====================================================================
//�^�C�g���w�i�̏I������
//====================================================================

void UninitTitleLogo(void)
{
	//�e�N�X�`���̔j��


	if (g_apTextureTitlelLogo != NULL)
	{
		g_apTextureTitlelLogo->Release();
		g_apTextureTitlelLogo = NULL;
	}


	//�o�b�t�@�̔j��
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}
//====================================================================
//�^�C�g���w�i�̍X�V����
//====================================================================

void UpdateTitleLogo(void)
{
	//VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	//							//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	//g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	////�e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//g_pVtxBuffTitleLogo->Unlock();

	
}

//====================================================================
//�^�C�g���w�i�̕`�揈��
//====================================================================

void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureTitlelLogo);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u�̎��		

}