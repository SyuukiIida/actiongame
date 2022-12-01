//====================================================================
//
//3D�Q�[�� �o���b�g[billboard.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "bullet.h"
#include "model.h"
#include "shadow.h"

#define MAX_BULLET			(128)			//�r���{�[�h�̍ő吔
#define BULLET_LIFE			(1000)			//�e�̎���

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�N�X�`���̃|�C���^
D3DXMATRIX g_mtxWorldBullet;
Bullet g_aBullet[MAX_BULLET];

int g_nIdxShadowBullet[MAX_BULLET] = { -1 };							//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j

//====================================================================
//�|���S���̏���������
//====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

								//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = BULLET_LIFE;
		g_aBullet[nCntBullet].bUse = false;
	}
	

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffBullet, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pVtx+=4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-5.0f, +10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+5.0f, +10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+5.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
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

	g_pVtxBuffBullet->Unlock();

	
}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}

//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateBullet(void)
{
	for (int nCntBllet = 0; nCntBllet < MAX_BULLET; nCntBllet++)
	{

		if (g_aBullet[nCntBllet].bUse == true)
		{//�e���g�p����Ă�
		

			//�e�̍X�V����
			g_aBullet[nCntBllet].move.x += sinf(D3DX_PI+g_aBullet[nCntBllet].rot.y) * 1.5f;
			g_aBullet[nCntBllet].move.z += cosf(D3DX_PI+g_aBullet[nCntBllet].rot.y) * 1.5f;

			//�ʒu���X�V
			g_aBullet[nCntBllet].pos += g_aBullet[nCntBllet].move;

			//������ǉ�
			g_aBullet[nCntBllet].move.x += (0.0f - g_aBullet[nCntBllet].move.x) * 0.50f;
			g_aBullet[nCntBllet].move.z += (0.0f - g_aBullet[nCntBllet].move.z) * 0.5f;

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_nIdxShadowBullet[nCntBllet], g_aBullet[nCntBllet].pos);
		}

		

		

		if (g_aBullet[nCntBllet].bUse == true)
		{
			//�����̃J�E���g�_�E��
			g_aBullet[nCntBllet].nLife -= 10;

			if (g_aBullet[nCntBllet].nLife == 0)
			{//�������s����
				SetEndShadow(g_nIdxShadowBullet[nCntBllet]);

				g_aBullet[nCntBllet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���		
			}
		}
	}
}

//====================================================================
//�|���S���̕`�揈��
//====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

											//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���

		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBullet);

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
			D3DXMatrixInverse(&g_mtxWorldBullet, NULL, &mtxView);//�t�s������߂�
																	 /*g_Billboard.mtxWorld._12 = 0.0f;
																	 g_Billboard.mtxWorld._21 = 0.0f;*/
			g_mtxWorldBullet._41 = 0.0f;
			g_mtxWorldBullet._42 = 0.0f;
			g_mtxWorldBullet._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

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
	}
}

//====================================================================
//�e�̐ݒ菈��
//====================================================================
void SetBullet(void)
{
	Model model = GetModel();

	for (int nCntBllet = 0; nCntBllet < MAX_BULLET; nCntBllet++)
	{
		if (g_aBullet[nCntBllet].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aBullet[nCntBllet].pos = model.pos+ D3DXVECTOR3(0.0f, 10.0f, 0.0f);;
			g_aBullet[nCntBllet].rot = model.rot;
			g_aBullet[nCntBllet].nLife = BULLET_LIFE;
			g_aBullet[nCntBllet].bUse = true;//�g�p����Ă����Ԃɂ���

			 //�e��ݒ�
			g_nIdxShadowBullet[nCntBllet] = SetShadow();
			break;
		}
	}
}