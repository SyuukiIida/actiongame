

#include "input.h"
#include "main.h"
#include "shootarrow.h"
#include "player.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShootArrow = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShootArrow = NULL;			//�e�N�X�`���̃|�C���^
ShootArrow g_ShootArrow;
//D3DXVECTOR3 g_posShadow;
//D3DXVECTOR3 g_rotShadow;
D3DXMATRIX g_mtxWorldShootArrow;							//���[���h�}�g���b�N�X

//====================================================================
//�e�̏���������
//====================================================================
void InitShootArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

								//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShootArrow);


	g_ShootArrow.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ShootArrow.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShootArrow, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShootArrow->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, 20.0f);
	pVtx[1].pos = D3DXVECTOR3(10.0f , 0.0f, 20.0f);
	pVtx[2].pos = D3DXVECTOR3(-10.0f , 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(10.0f , 0.0f, 0.0f);

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


	g_pVtxBuffShootArrow->Unlock();
}

//====================================================================
//�e�̏I������
//====================================================================
void UninitShootArrow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShootArrow != NULL)
	{
		g_pTextureShootArrow->Release();
		g_pTextureShootArrow = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffShootArrow != NULL)
	{
		g_pVtxBuffShootArrow->Release();
		g_pVtxBuffShootArrow = NULL;
	}
}

//====================================================================
//�e�̍X�V����
//====================================================================
void UpdateShootArrow(void)
{
	Player player;
	player = GetPlayer();

	g_ShootArrow.rot.y = player.rot.y;
}

//====================================================================
//�e�̕`�揈��
//====================================================================
void DrawShootArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffShootArrow, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldShootArrow);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ShootArrow.rot.y, g_ShootArrow.rot.x, g_ShootArrow.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldShootArrow, &g_mtxWorldShootArrow, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_ShootArrow.pos.x, 0.01f, g_ShootArrow.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldShootArrow, &g_mtxWorldShootArrow, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShootArrow);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureShootArrow);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//====================================================================
//�e�̈ʒu�ݒ菈��
//====================================================================
void SetPositionShootArrow(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxPot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	g_ShootArrow.pos = pos;

}