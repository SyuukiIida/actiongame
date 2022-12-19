


#include "input.h"
#include "main.h"
#include "player.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "obstacle.h"
#include "wall.h"
#include "game.h"
#include "goalflag.h"
#include "fade.h"
#include "shootarrow.h"

//�}�N����`
#define GAUGE				(0.43f)				//�Q�[�W������


//�O���[�o���ϐ�
LPD3DXMESH g_pMeshPlayer = NULL;					//���b�V���i���_���ւ̃|�C���^�j
LPD3DXBUFFER g_pBuffMatPlayer = NULL;			//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_apTexturePlayer[10] = {};	//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatPlayer = 0;						//�}�e���A���̐�
Player g_aPlayer;				
int g_nPlayerCounter;
int g_nCoolTimePlayer;

D3DXMATRIX g_mtxWorldModelPlayer;						//���[���h�}�g���b�N�X

int g_nCounterAnimPlayer;
int g_nIdxShadowPlayer = -1;							//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j

//====================================================================
//���f���̏���������
//====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//���_���̕ϐ�
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^

	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.gauge = 0;
	g_nPlayerCounter = 0;
	g_nCoolTimePlayer = 0;


	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\golfball.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatPlayer,
		NULL, &g_dwNumMatPlayer, &g_pMeshPlayer);

	//���_�����擾
	nNumVtx = g_pMeshPlayer->GetNumVertices();

	//���_�t�H�[�}�b�g
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshPlayer->GetFVF());

	//���_�o�b�t�@���b�N
	g_pMeshPlayer->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (g_aPlayer.vtxMinModel.x > vtx.x)
		{
			g_aPlayer.vtxMinModel.x = vtx.x;
		}

		if (g_aPlayer.vtxMaxModel.x < vtx.x)
		{
			g_aPlayer.vtxMaxModel.x = vtx.x;
		}

		if (g_aPlayer.vtxMinModel.y > vtx.y)
		{
			g_aPlayer.vtxMinModel.y = vtx.y;
		}

		if (g_aPlayer.vtxMaxModel.y < vtx.y)
		{
			g_aPlayer.vtxMaxModel.y = vtx.y;
		}

		if (g_aPlayer.vtxMinModel.z > vtx.z)
		{
			g_aPlayer.vtxMinModel.z = vtx.z;
		}

		if (g_aPlayer.vtxMaxModel.z < vtx.z)
		{
			g_aPlayer.vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}

	g_pMeshPlayer->UnlockVertexBuffer();

	g_aPlayer.vtxMinModel += g_aPlayer.pos;
	g_aPlayer.vtxMaxModel += g_aPlayer.pos;

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat= (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
			
				//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTexturePlayer[nCntMat]);
		}
	}
	//�e��ݒ�
	g_nIdxShadowPlayer = SetShadow();
}

//====================================================================
//���f���̏I������
//====================================================================
void UninitPlayer(void)
{
	//���b�V���̔j��
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//====================================================================
//���f���̍X�V����
//====================================================================
void UpdatePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	g_aPlayer.posold = g_aPlayer.pos;					//���݂̈ʒu��ۑ�
	g_aPlayer.vtxMaxModelold = g_aPlayer.vtxMaxModel;
	g_aPlayer.vtxMinModelold = g_aPlayer.vtxMinModel;

	Camera camera = GetCamera();

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_LEFT) == true)
	{//A(��)�L�[�������ꂽ
		g_aPlayer.move.x -= sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aPlayer.move.z -= cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//D(�E)�L�[�������ꂽ
		g_aPlayer.move.x += sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (-D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_UP) == true)
	{//W(��)�L�[�������ꂽ
		g_aPlayer.move.x += sinf(0 + camera.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(0 + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (D3DX_PI + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//S(��O)�L�[�������ꂽ
		g_aPlayer.move.x += sinf(D3DX_PI + camera.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (camera.rot.y);
	}
#endif

  	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		g_aPlayer.gauge += GAUGE;

		if (g_aPlayer.gauge >= MAX_GAUGE)
		{
			g_aPlayer.gauge = 0;
		}
		
	}

	if (GetKeyboardRelease(DIK_SPACE) == true)
	{//SPACE�L�[�������ꂽ
	 //�i�s�����̐ݒ�
		g_aPlayer.move.x += sinf(camera.rot.y) * g_aPlayer.gauge;
		g_aPlayer.move.z += cosf(camera.rot.y) * g_aPlayer.gauge;

		g_aPlayer.gauge = 0;

		g_nCoolTimePlayer = COOLTIME;
	}

	if (g_nCoolTimePlayer > 0)
	{
		g_aPlayer.gauge = 0;
	}

	//�^�C�}�[�n��
	g_nPlayerCounter++;

	if ((g_nPlayerCounter % 60) == 0)
	{//��莞�Ԍo��
		g_nPlayerCounter = 0;		//�J�E���^�[�������l�ɖ߂�

		g_nCoolTimePlayer--;
	}

	g_aPlayer.rot.y = (camera.rot.y);

	//�ʒu���X�V
	g_aPlayer.pos += g_aPlayer.move;
	g_aPlayer.vtxMaxModel += g_aPlayer.move;
	g_aPlayer.vtxMinModel += g_aPlayer.move;

	//������ǉ�
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.05f;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.05f;

	//�����蔻��------------------------------------------------
	{
		//���f�������蔻��----------------------------------------------------------
		if (CollisionObstaclePlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONOBSTACLE_Z) == true)
		{
			//���˕Ԃ�
			g_aPlayer.move.z *= -0.90f;
		}

		if (CollisionObstaclePlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONOBSTACLE_X) == true)
		{
			//���˕Ԃ�
			g_aPlayer.move.x *= -0.90f;
		}

		//�Ǔ����蔻��-------------------------------------------------------
		if (CollisionWallPlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONWALL_Z) == true)
		{
			//���˕Ԃ�
			g_aPlayer.move.z *= -0.90f;
		}

		if (CollisionWallPlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONWALL_X) == true)
		{
			//���˕Ԃ�
			g_aPlayer.move.x *= -0.90f;
		}

		//�S�[�������蔻��-------------------------------------------------------
		if (CollisionGoalFlagPlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold) == true)
		{
			SetGameState(GAMESTATE_END, 10, RESULT_CREAR);
		}
	}

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIdxShadowPlayer, g_aPlayer.pos);

	SetPositionShootArrow(g_aPlayer.pos);

	//SetGameState(GAMESTATE_END, 30, RESULT_CREAR);

}

//====================================================================
//���f���̕`�揈��
//====================================================================

void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//�}�e���A���ۑ��p
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModelPlayer);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldModelPlayer, &g_mtxWorldModelPlayer, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldModelPlayer, &g_mtxWorldModelPlayer, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModelPlayer);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//�e�N�X�`����̐ݒ�
		pDevice->SetTexture(0, g_apTexturePlayer[nCntMat]);
		//���f���i�p�[�c�j�̕`��
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	////���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	//{
	//	//�e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, g_apTextureModel[nCntMat]);
	//	////�`��
	//	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//}
	
}

//====================================================================
//���f���̏��
//====================================================================
Player GetPlayer(void)
{
	return g_aPlayer;
}

//====================================================================
//���f���̈ړ��ʐݒ菈��
//====================================================================
float GetGauge(void)
{
	return g_aPlayer.gauge;
}
