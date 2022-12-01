//====================================================================
//
//3D�Q�[���|���S��[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "meshfield.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshfield;
D3DXVECTOR3 g_rotMeshfield;
D3DXMATRIX g_mtxWorldMeshfield;							//���[���h�}�g���b�N�X

//====================================================================
//�|���S���̏���������
//====================================================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	WORD *pIdx;					//�C���f�b�N�X���ւ̃|�C���^

								//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureMeshfield);

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield, NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 9, 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-250.0f, 0.0f, 250.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 250.0f);
	pVtx[2].pos = D3DXVECTOR3(250.0f, 0.0f, 250.0f);
	pVtx[3].pos = D3DXVECTOR3(-250.0f, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(250.0f, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-250.0f, 0.0f, -250.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	pVtx[8].pos = D3DXVECTOR3(250.0f, 0.0f, -250.0f);

	//pVtx[9].pos = D3DXVECTOR3(-250.0f, 0.0f, 0.0f);
	//pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	//pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[12].pos = D3DXVECTOR3(250.0f, 0.0f, -250.0f);
	//pVtx[13].pos = D3DXVECTOR3(250.0f, 0.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(2.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f, 2.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 4.0f);
	pVtx[5].tex = D3DXVECTOR2(2.0f, 4.0f);
	pVtx[6].tex = D3DXVECTOR2(2.0f, 4.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(2.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[11].tex = D3DXVECTOR2(2.0f, 2.0f);
	pVtx[12].tex = D3DXVECTOR2(0.0f, 4.0f);
	pVtx[13].tex = D3DXVECTOR2(2.0f, 4.0f);

	g_pVtxBuffMeshfield->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 3;
	pIdx[1] = 0;


	g_pVtxBuffMeshfield->Unlock();
}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitMeshfield(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

}

//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateMeshfield(void)
{

}

//====================================================================
//�|���S���̕`�揈��
//====================================================================
void DrawMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
											//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);

}