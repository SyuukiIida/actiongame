

#include "main.h"
#include "game.h"
#include "gauge.h"
#include "player.h"
#include "input.h"



//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureGauge = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Gauge g_aGauge;
int g_nGaugeCounter;
int g_nCoolTime;

D3DXMATRIX mtxWorldGauge;

//===============================
//�X�R�A�̏������̏��� 
//===============================
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

								//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\numbr01.png",
		&g_pTextureGauge);

	g_aGauge.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nGaugeCounter = 0;
	g_nCoolTime = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//nor(�@��)�̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//===============================
//�X�R�A�̏I������
//===============================
void UninitGauge(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureGauge != NULL)
	{
		g_pTextureGauge->Release();
		g_pTextureGauge = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//===============================
//�X�R�A�̍X�V����
//===============================
void UpdateGauge(void)
{
	Player model = GetPlayer();
	float nGauge;
	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	
	g_aGauge.pos = model.pos + (D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		nGauge = model.gauge / MAX_GAUGE;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-15.0f, MAX_GAUGE * nGauge, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(-5.0f, MAX_GAUGE * nGauge, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);

		if(nGauge>=0.9f)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
			pVtx[1].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
			pVtx[2].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
			pVtx[3].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
		}

		else
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);
			pVtx[1].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);
			pVtx[2].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);
			pVtx[3].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);

		}

	}

	if (GetKeyboardRelease(DIK_SPACE) == true)
	{//SPACE�L�[�������ꂽ
		g_nCoolTime = COOLTIME;
	}

	g_nGaugeCounter++;

	if ((g_nGaugeCounter % 60) == 0)
	{//��莞�Ԍo��
		g_nGaugeCounter = 0;		//�J�E���^�[�������l�ɖ߂�

		g_nCoolTime--;

		if (g_nCoolTime == 0)
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffGauge->Unlock();

}

//===============================
//�X�R�A�̕`�揈��
//===============================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

											//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorldGauge);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);


	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&mtxWorldGauge, NULL, &mtxView);//�t�s������߂�														
	mtxWorldGauge._41 = 0.0f;
	mtxWorldGauge._42 = 0.0f;
	mtxWorldGauge._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aGauge.pos.x, g_aGauge.pos.y, g_aGauge.pos.z);

	D3DXMatrixMultiply(&mtxWorldGauge, &mtxWorldGauge, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldGauge);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGauge);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
}
