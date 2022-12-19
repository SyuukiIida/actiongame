//====================================================================
//
//3D�Q�[���|���S��[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "wall.h"
#include "floor.h"

#define MAX_WALL				(4)					//�ǖʐ�


//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//�e�N�X�`���̃|�C���^
Wall g_Wall[MAX_WALL];

//====================================================================
//�|���S���̏���������
//====================================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureWall);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 1.57f * nCntWall, 0.0f);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffWall, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx +=4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-FLOOR_SIZE, +50.0f, FLOOR_SIZE);
		pVtx[1].pos = D3DXVECTOR3(FLOOR_SIZE, 50.0f, FLOOR_SIZE);
		pVtx[2].pos = D3DXVECTOR3(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
		pVtx[3].pos = D3DXVECTOR3(FLOOR_SIZE, 0.0f, FLOOR_SIZE);

		//rhw�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(3.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.5f);
		pVtx[3].tex = D3DXVECTOR2(3.0f, 1.5f);
	}

	g_pVtxBuffWall->Unlock();
}

//====================================================================
//�|���S���̏I������
//====================================================================

void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

}

//====================================================================
//�|���S���̍X�V����
//====================================================================

void UpdateWall(void)
{

}

//====================================================================
//�|���S���̕`�揈��
//====================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);
		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWall);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}


bool CollisionWallBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;			//�����������ǂ���

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{

		if (g_Wall[nCnt].pos.z + FLOOR_SIZE >= pPosOld->z
			&&g_Wall[nCnt].pos.z + FLOOR_SIZE <= pPos->z)
		{
			pPos->z = g_Wall[nCnt].pos.z + FLOOR_SIZE;
			pMove->z = 0.0f;
			bLand = true;

			//���˕Ԃ�

			/*pMove->z = pMove->z*-15.0f;
			pMove->x = pMove->x*-15.0f;*/
		}

		if (g_Wall[nCnt].pos.z - FLOOR_SIZE <= pPosOld->z
			&&g_Wall[nCnt].pos.z - FLOOR_SIZE >= pPos->z)
		{
			pPos->z = g_Wall[nCnt].pos.z - FLOOR_SIZE;
			pMove->z = 0.0f;
			bLand = true;
		}


		if (g_Wall[nCnt].pos.x + FLOOR_SIZE >= pPosOld->x
			&&g_Wall[nCnt].pos.x + FLOOR_SIZE <= pPos->x)
		{
			pPos->x = g_Wall[nCnt].pos.x + FLOOR_SIZE;
			pMove->x = 0.0f;
			bLand = true;
		}

		if (g_Wall[nCnt].pos.x - FLOOR_SIZE <= pPosOld->x
			&&g_Wall[nCnt].pos.x - FLOOR_SIZE >= pPos->x)
		{
			pPos->x = g_Wall[nCnt].pos.x - FLOOR_SIZE;
			pMove->x = 0.0f;
			bLand = true;
		}
	}


	return bLand;
}

//====================================================================
//�ǂ̓����蔻��
//====================================================================
bool CollisionWallPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold
	, COLLISIONWALL collisionWall)
{
	bool bLand = false;			//�����������ǂ���

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		switch (collisionWall)
		{
		case COLLISIONWALL_Z:

			if (g_Wall[nCnt].pos.x + FLOOR_SIZE >= vtxMinModel->x
				&&g_Wall[nCnt].pos.x - FLOOR_SIZE <= vtxMaxModel->x)
			{//�͈͓��ɂ���Ƃ�

				if (g_Wall[nCnt].pos.z + FLOOR_SIZE >= vtxMaxModelold->z
					&&g_Wall[nCnt].pos.z + FLOOR_SIZE <= vtxMaxModel->z)
				{//�E�ǂɓ�������

					pPos->z = g_Wall[nCnt].pos.z + FLOOR_SIZE - (vtxMaxModel->z - pPos->z);
					vtxMinModel->z = g_Wall[nCnt].pos.z + FLOOR_SIZE - (vtxMaxModel->z - vtxMinModel->z);
					vtxMaxModel->z = g_Wall[nCnt].pos.z + FLOOR_SIZE;
					bLand = true;
				}

				if (g_Wall[nCnt].pos.z - FLOOR_SIZE <= vtxMinModelold->z
					&&g_Wall[nCnt].pos.z - FLOOR_SIZE >= vtxMinModel->z)
				{//���ǂɓ�������

					pPos->z = g_Wall[nCnt].pos.z - FLOOR_SIZE - (vtxMinModel->z - pPos->z);
					vtxMaxModel->z = g_Wall[nCnt].pos.z - FLOOR_SIZE - (vtxMinModel->z - vtxMaxModel->z);
					vtxMinModel->z = g_Wall[nCnt].pos.z - FLOOR_SIZE;
					bLand = true;
				}
			}

			break;

		case COLLISIONWALL_X:

			if (g_Wall[nCnt].pos.z + FLOOR_SIZE >= vtxMinModel->z
				&&g_Wall[nCnt].pos.z - FLOOR_SIZE <= vtxMaxModel->z)
			{
				if (g_Wall[nCnt].pos.x + FLOOR_SIZE >= vtxMaxModelold->x
					&&g_Wall[nCnt].pos.x + FLOOR_SIZE <= vtxMaxModel->x)
				{
					pPos->x = g_Wall[nCnt].pos.x + FLOOR_SIZE - (vtxMaxModel->x - pPos->x);
					vtxMinModel->x = g_Wall[nCnt].pos.x + FLOOR_SIZE - (vtxMaxModel->x - vtxMinModel->x);
					vtxMaxModel->x = g_Wall[nCnt].pos.x + FLOOR_SIZE;
					bLand = true;
				}

				if (g_Wall[nCnt].pos.x - FLOOR_SIZE <= vtxMinModelold->x
					&&g_Wall[nCnt].pos.x - FLOOR_SIZE >= vtxMinModel->x)
				{
					pPos->x = g_Wall[nCnt].pos.x - FLOOR_SIZE - (vtxMinModel->x - pPos->x);
					vtxMaxModel->x = g_Wall[nCnt].pos.x - FLOOR_SIZE - (vtxMinModel->x - vtxMaxModel->x);
					vtxMinModel->x = g_Wall[nCnt].pos.x - FLOOR_SIZE;
					bLand = true;
				}
			}

			break;

		}
	}
	return bLand;
}