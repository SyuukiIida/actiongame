


#include "input.h"
#include "main.h"
#include "obstacle.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <d3dx9.h>


//�}�N����`
#define MAX_OBSRACLE		(10)					//��Q���ő吔
#define NAM_OBSRACLE		(10)					//��Q���̎��

#define MAX_LIFE			(3)					//��Q���̗�
#define TXT_MAX				(5000)				//�ő啶����

//�O���[�o���ϐ�
int g_MAX_OBSRACLE;					//��Q���ő吔
int g_NAM_OBSRACLE;					//��Q���̎��

LPD3DXMESH g_pMeshObstacle[NAM_OBSRACLE] = {};					//���b�V���i���_���ւ̃|�C���^�j
LPD3DXBUFFER g_pBuffMatObstacle[MAX_OBSRACLE] = {};			//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureObstacle[NAM_OBSRACLE][10] = {};	//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatObstacle[MAX_OBSRACLE] = { 0 };						//�}�e���A���̐�

Obstacle g_aObstacle[MAX_OBSRACLE];
char g_aFileNameObstacle[256][128];			//�t�@�C����

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
		g_aObstacle[nCnt].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].nLife = MAX_LIFE;
		g_aObstacle[nCnt].bUse = false;
	}
	

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
		//x�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(&g_aFileNameObstacle[g_aObstacle[nCnt].type][0], D3DXMESH_SYSTEMMEM,
			pDevice, NULL, &g_pBuffMatObstacle[nCnt],
			NULL, &g_dwNumMatObstacle[nCnt], &g_pMeshObstacle[nCnt])))
		{
			assert(false);
		}
	}
	////x�t�@�C���̓ǂݍ���
	//D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
	//	pDevice, NULL, &g_pBuffMatObstacle[0],
	//	NULL, &g_dwNumMatObstacle[0], &g_pMeshObstacle[0]);
	//D3DXLoadMeshFromX("data\\MODEL\\moon.x", D3DXMESH_SYSTEMMEM,
	//	pDevice, NULL, &g_pBuffMatObstacle[1],
	//	NULL, &g_dwNumMatObstacle[1], &g_pMeshObstacle[1]);


	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
	/*while (true)
	{

	}*/
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

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCnt]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCnt]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//�e�N�X�`���t�@�C�������݂���

				 //�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_apTextureObstacle[g_aObstacle[nCnt].type][nCntMat]);
				}
			}

			g_aObstacle[nCnt].bUse = true;

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
	for (int nCnt = 0; nCnt < NAM_OBSRACLE; nCnt++)
	{
		if (g_pMeshObstacle[nCnt] != NULL)
		{
			g_pMeshObstacle[nCnt]->Release();
			g_pMeshObstacle[nCnt] = NULL;
		}
	}

	//�o�b�t�@�̔j��
	for (int nCnt = 0; nCnt < NAM_OBSRACLE; nCnt++)
	{
		if (g_pBuffMatObstacle[nCnt] != NULL)
		{
			g_pBuffMatObstacle[nCnt]->Release();
			g_pBuffMatObstacle[nCnt] = NULL;
		}
	}
}

//====================================================================
//��Q���̍X�V����
//====================================================================
void UpdateObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^


	
	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[g_aObstacle[nCnt].type]->GetBufferPointer();

		if (g_aObstacle[nCnt].bUse == true)
		{
			
			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_nIdxShadowObstacle[nCnt], g_aObstacle[nCnt].pos);

			switch (g_aObstacle[nCnt].state)
			{
			case ENEMYSTATE_NORMAL:

				break;

			case ENEMYSTATE_DAMAGE:
				g_aObstacle[nCnt].nCounterState--;

				if (g_aObstacle[nCnt].nCounterState <= 0)
				{
					g_aObstacle[nCnt].state = ENEMYSTATE_NORMAL;

					for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCnt]; nCntMat++)
					{
						//���_�J���[�̐ݒ�
						pMat[nCntMat].MatD3D.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
					}
				}
				break;
			}
		}

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

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
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
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCnt]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCnt]; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`����̐ݒ�
				pDevice->SetTexture(0, g_apTextureObstacle[g_aObstacle[nCnt].type][nCntMat]);

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DMATERIAL9 matDef;							//�}�e���A���ۑ��p
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

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

	else
	{
		g_aObstacle[nCntObstacle].state = ENEMYSTATE_DAMAGE;
		g_aObstacle[nCntObstacle].nCounterState = 50;

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCntObstacle]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCntObstacle]; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`����̐ݒ�
			pDevice->SetTexture(0, g_apTextureObstacle[g_aObstacle[nCntObstacle].type][nCntMat]);

		}


		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}

}

//====================================================================
//��Q���ƒe�̓����蔻��
//====================================================================
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
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
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{

			if (g_aObstacle[nCnt].vtxMinModel.x < vtxMaxModel->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > vtxMinModel->x
				&&g_aObstacle[nCnt].vtxMinModel.y < vtxMaxModel->y
				&&g_aObstacle[nCnt].vtxMaxModel.y > vtxMinModel->y)
			{//��Q����x���̕��̒��ɂ���Ƃ�
				if (g_aObstacle[nCnt].vtxMaxModel.z <= vtxMinModelold->z
					&&g_aObstacle[nCnt].vtxMaxModel.z >= vtxMinModel->z)
				{//�������O�ɓ���������
					//pPos->z = g_aObstacle[nCnt].vtxMaxModel.z;

					pPos->z = g_aObstacle[nCnt].vtxMaxModel.z - (vtxMinModel->z - pPos->z);
					vtxMaxModel->z = g_aObstacle[nCnt].vtxMaxModel.z  - (vtxMinModel->z - vtxMaxModel->z);
					vtxMinModel->z = g_aObstacle[nCnt].vtxMaxModel.z;

					pMove->z = 0.0f;
					bLand = true;

					//���˕Ԃ�

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].vtxMinModel.z >= vtxMaxModelold->z
					&&g_aObstacle[nCnt].vtxMinModel.z <= vtxMaxModel->z)
				{//��O���牜�ɓ���������
					pPos->z = g_aObstacle[nCnt].vtxMinModel.z - (vtxMaxModel->z - pPos->z);
					vtxMinModel->z = g_aObstacle[nCnt].vtxMinModel.z - (vtxMaxModel->z - vtxMinModel->z);
					vtxMaxModel->z = g_aObstacle[nCnt].vtxMinModel.z;
					pMove->z = 0.0f;
					bLand = true;
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.z < vtxMaxModel->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > vtxMinModel->z
				&&g_aObstacle[nCnt].vtxMinModel.y < vtxMaxModel->y
				&&g_aObstacle[nCnt].vtxMaxModel.y > vtxMinModel->y)
			{//��Q����z���̕��̒��ɂ���Ƃ�

				if (g_aObstacle[nCnt].vtxMaxModel.x <= vtxMinModelold->x
					&&g_aObstacle[nCnt].vtxMaxModel.x >= vtxMinModel->x)
				{//�E���獶�ɓ���������
					pPos->x = g_aObstacle[nCnt].vtxMaxModel.x - (vtxMinModel->x - pPos->x);
					vtxMaxModel->x = g_aObstacle[nCnt].vtxMaxModel.x - (vtxMinModel->x - vtxMaxModel->x);
					vtxMinModel->x = g_aObstacle[nCnt].vtxMaxModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}

				if (g_aObstacle[nCnt].vtxMinModel.x >= vtxMaxModelold->x
					&&g_aObstacle[nCnt].vtxMinModel.x <= vtxMaxModel->x)
				{//������E�ɓ���������
					pPos->x = g_aObstacle[nCnt].vtxMinModel.x - (vtxMaxModel->x - pPos->x);
					vtxMinModel->x = g_aObstacle[nCnt].vtxMinModel.x - (vtxMaxModel->x - vtxMinModel->x);
					vtxMaxModel->x = g_aObstacle[nCnt].vtxMinModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.x < pPos->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > pPos->x
				&&g_aObstacle[nCnt].vtxMinModel.z < pPos->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > pPos->z)
			{//xz����̒��ɂ���Ƃ�
				if (g_aObstacle[nCnt].vtxMaxModel.y <= pPosOld->y
					&&g_aObstacle[nCnt].vtxMaxModel.y >= pPos->y)
				{//�E���獶�ɓ���������
					pPos->y = g_aObstacle[nCnt].vtxMaxModel.y;
					pMove->y = 0.0f;
					bLand = true;
				}

				if (g_aObstacle[nCnt].vtxMinModel.y >= pPosOld->y
					&&g_aObstacle[nCnt].vtxMinModel.y <= pPos->y)
				{
					pPos->y = g_aObstacle[nCnt].vtxMinModel.y;
					pMove->y = 0.0f;
					bLand = true;
				}
			}
		}
	}

	//for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	//{
	//	if (g_aObstacle[nCnt].bUse == true)
	//	{
	//		if (g_aObstacle[nCnt].vtxMinModel.x < vtxMaxModel->x
	//			&&g_aObstacle[nCnt].vtxMaxModel.x > vtxMinModel->x)
	//		{//��Q����x���̕��̒��ɂ���Ƃ�
	//			if (g_aObstacle[nCnt].vtxMaxModel.z <= vtxMinModelold->z
	//				&&g_aObstacle[nCnt].vtxMaxModel.z >= vtxMinModel->z)
	//			{//�������O�ɓ���������
	//				pPos->z = g_aObstacle[nCnt].vtxMaxModel.z - vtxMinModel->z;
	//				vtxMinModel->z = g_aObstacle[nCnt].vtxMaxModel.z;
	//				pMove->z = 0.0f;
	//				bLand = true;

	//				//���˕Ԃ�

	//				/*pMove->z = pMove->z*-15.0f;
	//				pMove->x = pMove->x*-15.0f;*/
	//			}

	//			if (g_aObstacle[nCnt].vtxMinModel.z >= vtxMaxModelold->z
	//				&&g_aObstacle[nCnt].vtxMinModel.z <= vtxMaxModel->z)
	//			{
	//				pPos->z = g_aObstacle[nCnt].vtxMinModel.z;
	//				pMove->z = 0.0f;
	//				bLand = true;
	//			}
	//		}

	//		if (g_aObstacle[nCnt].vtxMinModel.z < vtxMaxModel->z
	//			&&g_aObstacle[nCnt].vtxMaxModel.z > vtxMinModel->z)
	//		{
	//			if (g_aObstacle[nCnt].vtxMaxModel.x <= vtxMinModelold->x
	//				&&g_aObstacle[nCnt].vtxMaxModel.x >= vtxMinModel->x)
	//			{
	//				pPos->x = g_aObstacle[nCnt].vtxMaxModel.x;
	//				pMove->x = 0.0f;
	//				bLand = true;
	//			}

	//			if (g_aObstacle[nCnt].vtxMinModel.x >= vtxMinModelold->x
	//				&&g_aObstacle[nCnt].vtxMinModel.x <= vtxMinModel->x)
	//			{
	//				pPos->x = g_aObstacle[nCnt].vtxMinModel.x;
	//				pMove->x = 0.0f;
	//				bLand = true;
	//			}
	//		}
	//	}
	//}

	return bLand;
}

//====================================================================
//��Q���̓ǂݍ���
//====================================================================
void LoadObstacle(void)
{
	FILE *pFile;				// �t�@�C���|�C���^

	Obstacle *pObstacle = &g_aObstacle[0];
	g_MAX_OBSRACLE = 0;
	g_NAM_OBSRACLE = 0;


	char aDataSearch[TXT_MAX];	// �f�[�^�����p
	int  nCntPass = { 0 };

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen("data\\txt\\ModelData.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J���ꂽ
		while (1)
		{
			//fscanf(pFile,"%s", aDataSearch);		//��s��������
			fgets(&aDataSearch[0], 128, pFile);

			while (aDataSearch[0] == '\t')
			{//���l�߂ɂ���
				char ahoge[128];

				strcpy(&ahoge[0], &aDataSearch[0]);

				strcpy(&aDataSearch[0], &ahoge[1]);
			}

			//�T�[�`
			if (strcmp(aDataSearch, "END") == 0)
			{ 	//�t�@�C�������	
				fclose(pFile); break;
			}

			if (aDataSearch[0] == '#')
			{ // �܂�Ԃ�
				continue;
			}
			
		
			if (strncmp(aDataSearch, "NUM_MODEL", 9) == 0)
			{//���f���̎��
				char *pSprit;
				pSprit = strtok(&aDataSearch[0], " =");
				pSprit = strtok(NULL," =");
				g_NAM_OBSRACLE = atoi(pSprit);
			}

			if (strncmp(aDataSearch, "MODEL_FILENAME",13) == 0)
			{//��Q���̃t�@�C���̑��΃p�X
				char *pSprit;
				pSprit = strtok(&aDataSearch[0], " =");
				pSprit = strtok(NULL, " =");
				pSprit = strtok(pSprit, "\n");
				strcpy(&g_aFileNameObstacle[nCntPass][0], pSprit);
				nCntPass++;

				if (aDataSearch[0] == '#')
				{ // �܂�Ԃ�
					continue;
				}
			}

			if (strcmp(aDataSearch, "MODELSET\n") == 0)
			{
				// CHR:�G[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
				while (1)
				{
					fgets(&aDataSearch[0], 128, pFile);

					while (aDataSearch[0] == '\t')
					{//���l�߂ɂ���
						char ahoge[128];

						strcpy(&ahoge[0], &aDataSearch[0]);

						strcpy(&aDataSearch[0], &ahoge[1]);
					}

					if (strcmp(aDataSearch, "END_MODELSET\n") == 0)
					{// �ǂݍ��݂��I��
						pObstacle++;
						g_MAX_OBSRACLE++;
						break;
					}

					else if (strncmp(aDataSearch, "TYPE",4) == 0)
					{ 	// �G�̎��
						char *pSprit;
						pSprit = strtok(&aDataSearch[0], " =");		//�󔒂܂ł̕������g��Ȃ�
						pSprit = strtok(NULL, " =");				//�󔒂̌�̕�����ǂݍ���
						pObstacle->type = atoi(pSprit);
					}

					else if (strncmp(aDataSearch, "POS",3) == 0)
					{		// �ʒu
						char *pSprit;
						pSprit = strtok(&aDataSearch[0], " =");		//�󔒂܂ł̕������g��Ȃ�
						pSprit = strtok(NULL, " =");				//�󔒂̌�̕�����ǂݍ���
						pObstacle->pos.x = (float)atof(pSprit);

						//�󔒂̌�̕�����ǂݍ���
						pSprit = strtok(NULL, " =");
						pObstacle->pos.y = (float)atof(pSprit);

						//�󔒂̌�̕�����ǂݍ���
						pSprit = strtok(NULL, " =");
						pObstacle->pos.z = (float)atof(pSprit);

						//fscanf(pFile, "%f", &pObstacle->pos.x);	// X
						//fscanf(pFile, "%f", &pObstacle->pos.y);	// Y
						//fscanf(pFile, "%f", &pObstacle->pos.z);	// Z
					}

					else if (strcmp(aDataSearch, "ROT") == 0)
					{// ����
						char *pSprit;
						pSprit = strtok(&aDataSearch[0], " =");		//�󔒂܂ł̕������g��Ȃ�
						pSprit = strtok(NULL, " =");				//�󔒂̌�̕�����ǂݍ���
						pObstacle->rot.x = (float)atof(pSprit);			//X

						//�󔒂̌�̕�����ǂݍ���
						pSprit = strtok(NULL, " =");
						pObstacle->rot.y = (float)atof(pSprit);			//Y

						//�󔒂̌�̕�����ǂݍ���
						pSprit = strtok(NULL, " =");
						pObstacle->rot.z = (float)atoi(pSprit);			//Z
						
					}
				}
			}
			
		}



		if (pFile == NULL)
		{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		 //�������I������
			return;
		}

		//Obstacle *pObstacle = g_aObstacle;
	}
}