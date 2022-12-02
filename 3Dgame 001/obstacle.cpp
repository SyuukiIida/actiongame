


#include "input.h"
#include "main.h"
#include "obstacle.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//�}�N����`
#define MAX_OBSRACLE		(2)					//��Q���ő吔
#define MAX_LIFE			(3)					//��Q���̗�

//�O���[�o���ϐ�
LPD3DXMESH g_pMeshObstacle[MAX_OBSRACLE] = {};					//���b�V���i���_���ւ̃|�C���^�j
LPD3DXBUFFER g_pBuffMatObstacle = NULL;			//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureObstacle[10] = {};	//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatObstacle = 0;						//�}�e���A���̐�
Obstacle g_aObstacle[MAX_OBSRACLE];

D3DXMATRIX g_mtxWorldObstacle;

int g_nIdxShadowObstacle[MAX_OBSRACLE] = { -1 };							//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j

//====================================================================
//��Q���̏���������
//====================================================================
void InitObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//���_���̕ϐ�
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		g_aObstacle[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f * nCnt);
		g_aObstacle[nCnt].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].nLife = MAX_LIFE;
		g_aObstacle[nCnt].bUse = true;
	}
	

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatObstacle,
		NULL, &g_dwNumMatObstacle, &g_pMeshObstacle[0]);

	D3DXLoadMeshFromX("data\\MODEL\\moon.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatObstacle,
		NULL, &g_dwNumMatObstacle, &g_pMeshObstacle[1]);

	
	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//���_�����擾
		nNumVtx = g_pMeshObstacle[nCnt]->GetNumVertices();

		//���_�t�H�[�}�b�g
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObstacle[nCnt]->GetFVF());

		//���_�o�b�t�@���b�N
		g_pMeshObstacle[nCnt]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (g_aObstacle[nCnt].vtxMinModel.x > vtx.x)
			{
				g_aObstacle[nCnt].vtxMinModel.x = vtx.x;
			}

			if (g_aObstacle[nCnt].vtxMaxModel.x < vtx.x)
			{
				g_aObstacle[nCnt].vtxMaxModel.x = vtx.x;
			}

			if (g_aObstacle[nCnt].vtxMinModel.y > vtx.y)
			{
				g_aObstacle[nCnt].vtxMinModel.y = vtx.y;
			}

			if (g_aObstacle[nCnt].vtxMaxModel.y < vtx.y)
			{
				g_aObstacle[nCnt].vtxMaxModel.y = vtx.y;
			}

			if (g_aObstacle[nCnt].vtxMinModel.z > vtx.z)
			{
				g_aObstacle[nCnt].vtxMinModel.z = vtx.z;
			}

			if (g_aObstacle[nCnt].vtxMaxModel.z < vtx.z)
			{
				g_aObstacle[nCnt].vtxMaxModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		g_pMeshObstacle[nCnt]->UnlockVertexBuffer();

		g_aObstacle[nCnt].vtxMinModel += g_aObstacle[nCnt].pos;
		g_aObstacle[nCnt].vtxMaxModel += g_aObstacle[nCnt].pos;
	}

	

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObstacle->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���

		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTextureObstacle[nCntMat]);
		}
	}

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//�e��ݒ�
		g_nIdxShadowObstacle[nCnt] = SetShadow();
	}
}

//====================================================================
//��Q���̏I������
//====================================================================
void UninitObstacle(void)
{
	//���b�V���̔j��
	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_pMeshObstacle[nCnt] != NULL)
		{
			g_pMeshObstacle[nCnt]->Release();
			g_pMeshObstacle[nCnt] = NULL;
		}
	}

	//�o�b�t�@�̔j��
	if (g_pBuffMatObstacle != NULL)
	{
		g_pBuffMatObstacle->Release();
		g_pBuffMatObstacle = NULL;
	}
}

//====================================================================
//��Q���̍X�V����
//====================================================================
void UpdateObstacle(void)
{
	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//�e�̈ʒu��ݒ�
		SetPositionShadow(g_nIdxShadowObstacle[nCnt], g_aObstacle[nCnt].pos);
	}
}

//====================================================================
//��Q���̕`�揈��
//====================================================================
void DrawObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//�}�e���A���ۑ��p
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldObstacle);
			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObstacle[nCnt].rot.y, g_aObstacle[nCnt].rot.x, g_aObstacle[nCnt].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldObstacle, &g_mtxWorldObstacle, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aObstacle[nCnt].pos.x, g_aObstacle[nCnt].pos.y, g_aObstacle[nCnt].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldObstacle, &g_mtxWorldObstacle, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObstacle);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				//�e�N�X�`����̐ݒ�
				pDevice->SetTexture(0, g_apTextureObstacle[nCntMat]);
				//���f���i�p�[�c�j�̕`��
				g_pMeshObstacle[nCnt]->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);
	}

}

//====================================================================
//��Q���̃q�b�g����
//====================================================================
void HitObstacle(int nCntObstacle, int nDamage)
{
	g_aObstacle[nCntObstacle].nLife -= nDamage;

	if (g_aObstacle[nCntObstacle].nLife == 0)
	{
		//�����̐ݒ�
		//PlaySound(SOUND_LABEL_SE_EXPLOSION);

		//SetExplosion(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

		//
		g_aObstacle[nCntObstacle].bUse = false;

		SetEndShadow(g_nIdxShadowObstacle[nCntObstacle]);

	}

	//else
	//{
	//	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	//	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//	g_aObstacle[nCntObstacle].state = ENEMYSTATE_DAMAGE;
	//	g_aObstacle[nCntObstacle].nCounterState = 5;

	//	pVtx += nCntEnemy * 4;

	//	//���_�J���[�̐ݒ�
	//	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	//	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	//	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	//	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	//	g_pVtxBuffEnemy->Unlock();
	//}

}

//====================================================================
//��Q���ƒe�̓����蔻��
//====================================================================
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{
			if (g_aObstacle[nCnt].vtxMinModel.x < pPos->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > pPos->x)
			{//��Q����x���̕��̒��ɂ���Ƃ�
				if (g_aObstacle[nCnt].vtxMaxModel.z  <= pPosOld->z
					&&g_aObstacle[nCnt].vtxMaxModel.z >= pPos->z)
				{//�������O�ɓ���������
					pPos->z = g_aObstacle[nCnt].vtxMaxModel.z;
					pMove->z = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);

					//���˕Ԃ�

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].vtxMinModel.z >= pPosOld->z
					&&g_aObstacle[nCnt].vtxMinModel.z <= pPos->z)
				{
					pPos->z = g_aObstacle[nCnt].vtxMinModel.z;
					pMove->z = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.z < pPos->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > pPos->z)
			{
				if (g_aObstacle[nCnt].vtxMaxModel.x <= pPosOld->x
					&&g_aObstacle[nCnt].vtxMaxModel.x >= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMaxModel.x;
					pMove->x = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);
				}

				if (g_aObstacle[nCnt].vtxMinModel.x >= pPosOld->x
					&&g_aObstacle[nCnt].vtxMinModel.x <= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMinModel.x;
					pMove->x = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);
				}
			}
		}
	}

	return bLand;
}

//====================================================================
//��Q���ƃv���C���[�̓����蔻��
//====================================================================
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{
			if (g_aObstacle[nCnt].vtxMinModel.x < pPos->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > pPos->x)
			{//��Q����x���̕��̒��ɂ���Ƃ�
				if (g_aObstacle[nCnt].vtxMaxModel.z <= pPosOld->z
					&&g_aObstacle[nCnt].vtxMaxModel.z >= pPos->z)
				{//�������O�ɓ���������
					pPos->z = g_aObstacle[nCnt].vtxMaxModel.z;
					pMove->z = 0.0f;
					bLand = true;

					//���˕Ԃ�

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].vtxMinModel.z >= pPosOld->z
					&&g_aObstacle[nCnt].vtxMinModel.z <= pPos->z)
				{
					pPos->z = g_aObstacle[nCnt].vtxMinModel.z;
					pMove->z = 0.0f;
					bLand = true;
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.z < pPos->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > pPos->z)
			{
				if (g_aObstacle[nCnt].vtxMaxModel.x <= pPosOld->x
					&&g_aObstacle[nCnt].vtxMaxModel.x >= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMaxModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}

				if (g_aObstacle[nCnt].vtxMinModel.x >= pPosOld->x
					&&g_aObstacle[nCnt].vtxMinModel.x <= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMinModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}
			}
		}
	}

	return bLand;
}