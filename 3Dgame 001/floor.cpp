//====================================================================
//
//3D�Q�[���|���S��[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "floor.h"



//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFloor = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureFloor = NULL;			//�e�N�X�`���̃|�C���^
Floor g_Floor;

//====================================================================
//�|���S���̏���������
//====================================================================
void InitFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\grass001.jpg",
		&g_pTextureFloor);

	g_Floor.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Floor.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffFloor, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFloor->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-FLOOR_SIZE,0.0f, FLOOR_SIZE);
	pVtx[1].pos = D3DXVECTOR3(FLOOR_SIZE, 0.0f, FLOOR_SIZE);
	pVtx[2].pos = D3DXVECTOR3(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
	pVtx[3].pos = D3DXVECTOR3(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);

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
	pVtx[1].tex = D3DXVECTOR2(6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 6.0f);
	pVtx[3].tex = D3DXVECTOR2(6.0f, 6.0f);

	g_pVtxBuffFloor->Unlock();
}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitFloor(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureFloor != NULL)
	{
		g_pTextureFloor->Release();
		g_pTextureFloor = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffFloor != NULL)
	{
		g_pVtxBuffFloor->Release();
		g_pVtxBuffFloor = NULL;
	}

}

//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateFloor(void)
{

}

//====================================================================
//�|���S���̕`�揈��
//====================================================================
void DrawFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Floor.mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Floor.rot.y, g_Floor.rot.x, g_Floor.rot.z);

	D3DXMatrixMultiply(&g_Floor.mtxWorld, &g_Floor.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Floor.pos.x, g_Floor.pos.y, g_Floor.pos.z);

	D3DXMatrixMultiply(&g_Floor.mtxWorld, &g_Floor.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Floor.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFloor, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFloor);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}