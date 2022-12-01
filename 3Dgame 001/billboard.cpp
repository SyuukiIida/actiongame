//====================================================================
//
//3D�Q�[���r���{�[�h[billboard.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "billboard.h"

#define MAX_BILLBOARD			(4)						//�r���{�[�h�̍ő吔

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//�e�N�X�`���̃|�C���^
Billboard g_Billboard;

//====================================================================
//�|���S���̏���������
//====================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tree000.png",
		&g_pTextureBillboard);

	g_Billboard.pos = D3DXVECTOR3(0.0f, 0.0f,190.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Billboard.pos.x - 40.0f, g_Billboard.pos.y + 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Billboard.pos.x + 40.0f, g_Billboard.pos.y + 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Billboard.pos.x - 40.0f, g_Billboard.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Billboard.pos.x + 40.0f, g_Billboard.pos.y , 0.0f);
	
	//rhw�̐ݒ�
	pVtx[0].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[1].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[2].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[3].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);

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

	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

}

//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateBillboard(void)
{

}

//====================================================================
//�|���S���̕`�揈��
//====================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Billboard.mtxWorld);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

#ifdef _DEBUG
	int i=0;

	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		i++;
	}

	if (i % 2 == 0)
	{
		//Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	else
	{
		//Z�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

#endif

	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&g_Billboard.mtxWorld, NULL,&mtxView);//�t�s������߂�
	/*g_Billboard.mtxWorld._12 = 0.0f;
	g_Billboard.mtxWorld._21 = 0.0f;*/
	g_Billboard.mtxWorld._41 = 0.0f;
	g_Billboard.mtxWorld._42 = 0.0f;
	g_Billboard.mtxWorld._43 = 0.0f;
	
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Billboard.pos.x, g_Billboard.pos.y, g_Billboard.pos.z);

	D3DXMatrixMultiply(&g_Billboard.mtxWorld, &g_Billboard.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Billboard.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBillboard);

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