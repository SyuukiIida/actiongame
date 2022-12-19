


#include "input.h"
#include "main.h"
#include "shadow.h"

#define MAX_SHADOW			(256)			//�e�̍ő吔

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//�e�N�X�`���̃|�C���^
Shadow g_Shadow[MAX_SHADOW];
//D3DXVECTOR3 g_posShadow;
//D3DXVECTOR3 g_rotShadow;
D3DXMATRIX g_mtxWorldShadow;							//���[���h�}�g���b�N�X

//====================================================================
//�e�̏���������
//====================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

								//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].bUse = false;
	}

	//g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-10.0f/*+(20.0f*nCntShadow)*/, 0.0f, 10.0f);
		pVtx[1].pos = D3DXVECTOR3(10.0f /*+ (20.0f*nCntShadow)*/, 0.0f, 10.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f /*+ (20.0f*nCntShadow)*/, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(10.0f /*+ (20.0f*nCntShadow)*/, 0.0f, -10.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffShadow->Unlock();
}

//====================================================================
//�e�̏I������
//====================================================================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//====================================================================
//�e�̍X�V����
//====================================================================
void UpdateShadow(void)
{

}

//====================================================================
//�e�̕`�揈��
//====================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
											
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, 0.01f, g_Shadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);			
		}
	}

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================================================
//�e�̐ݒ菈��
//====================================================================
int SetShadow(void)
{
	int nCntShadow = -1;

	for (nCntShadow; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (nCntShadow == -1)
		{
			continue;
		}

		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].bUse = true;
			break;
		}
	}

	return nCntShadow;
}


//====================================================================
//�e�̈ʒu�ݒ菈��
//====================================================================
void SetPositionShadow(int nIdxShadow,D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxPot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	g_Shadow[nIdxShadow].pos = pos;

											//���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&g_Shadow[nIdxShadow].mtxWorldShadow);
	////�����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxPot, g_Shadow[nIdxShadow].rot.y, g_Shadow[nIdxShadow].rot.x, g_Shadow[nIdxShadow].rot.z);

	//D3DXMatrixMultiply(&g_Shadow[nIdxShadow].mtxWorldShadow, &g_Shadow[nIdxShadow].mtxWorldShadow, &mtxPot);

	////�ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, g_Shadow[nIdxShadow].pos.x, 0.0f, g_Shadow[nIdxShadow].pos.z);

	//D3DXMatrixMultiply(&g_Shadow[nIdxShadow].mtxWorldShadow, &g_Shadow[nIdxShadow].mtxWorldShadow, &mtxTrans);

	////���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	////���_�t�H�[�}�b�g�̐ݒ�
	//pDevice->SetFVF(FVF_VERTEX_3D);

	////�e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, g_pTextureShadow);

	////�`��
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nIdxShadow, 2);

}


void SetEndShadow(int nShadow)
{
	g_Shadow[nShadow].bUse = false;
}