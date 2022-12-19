




#include "main.h"
#include "goalflag.h"

//�O���[�o���ϐ�
LPD3DXMESH g_pMeshGoalFlag = NULL;					//���b�V���i���_���ւ̃|�C���^�j
LPD3DXBUFFER g_pBuffMatGoalFlag = NULL;			//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureGoalFlag[10] = {};	//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatGoalFlag = 0;						//�}�e���A���̐�
GoalFlag g_aGoalFlag;

D3DXMATRIX g_mtxWorldGoalFlag;


//====================================================================
//�S�[���̏���������
//====================================================================
void InitGoalFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//���_���̕ϐ�
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^

	g_aGoalFlag.pos = D3DXVECTOR3(0.0f, -12.5f, 250.0f);
	g_aGoalFlag.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoalFlag.vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoalFlag.vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoalFlag.state = GOALSTATE_NORMAL;

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatGoalFlag,
		NULL, &g_dwNumMatGoalFlag, &g_pMeshGoalFlag);


	//���_�����擾
	nNumVtx = g_pMeshGoalFlag->GetNumVertices();

	//���_�t�H�[�}�b�g
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshGoalFlag->GetFVF());

	//���_�o�b�t�@���b�N
	g_pMeshGoalFlag->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (g_aGoalFlag.vtxMinModel.x > vtx.x)
		{
			g_aGoalFlag.vtxMinModel.x = vtx.x;
		}

		if (g_aGoalFlag.vtxMaxModel.x < vtx.x)
		{
			g_aGoalFlag.vtxMaxModel.x = vtx.x;
		}

		if (g_aGoalFlag.vtxMinModel.y > vtx.y)
		{
			g_aGoalFlag.vtxMinModel.y = vtx.y;
		}

		if (g_aGoalFlag.vtxMaxModel.y < vtx.y)
		{
			g_aGoalFlag.vtxMaxModel.y = vtx.y;
		}

		if (g_aGoalFlag.vtxMinModel.z > vtx.z)
		{
			g_aGoalFlag.vtxMinModel.z = vtx.z;
		}

		if (g_aGoalFlag.vtxMaxModel.z < vtx.z)
		{
			g_aGoalFlag.vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}

	g_pMeshGoalFlag->UnlockVertexBuffer();

	g_aGoalFlag.vtxMinModel += g_aGoalFlag.pos;
	g_aGoalFlag.vtxMaxModel += g_aGoalFlag.pos;

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatGoalFlag->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatGoalFlag; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���

		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTextureGoalFlag[nCntMat]);
		}
	}
}

//====================================================================
//�S�[���̏I������
//====================================================================
void UninitGoalFlag(void)
{
	//���b�V���̔j��
	if (g_pMeshGoalFlag != NULL)
	{
		g_pMeshGoalFlag->Release();
		g_pMeshGoalFlag = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pBuffMatGoalFlag != NULL)
	{
		g_pBuffMatGoalFlag->Release();
		g_pBuffMatGoalFlag = NULL;
	}
}

//====================================================================
//�S�[���̍X�V����
//====================================================================
void UpdateGoalFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatGoalFlag->GetBufferPointer();//�}�e���A���f�[�^�ւ̃|�C���^

	switch (g_aGoalFlag.state)
	{
	case GOALSTATE_NORMAL:

		break;

	case GOALSTATE_GOAL:
		g_aGoalFlag.nCounterState--;

		if (g_aGoalFlag.nCounterState <= 0)
		{
			g_aGoalFlag.state = GOALSTATE_NORMAL;

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatGoalFlag; nCntMat++)
			{
				//���_�J���[�̐ݒ�
				pMat[nCntMat].MatD3D.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}
		break;
	}
}

//====================================================================
//�S�[���̕`�揈��
//====================================================================
void DrawGoalFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//�}�e���A���ۑ��p
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

													//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldGoalFlag);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoalFlag.rot.y, g_aGoalFlag.rot.x, g_aGoalFlag.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldGoalFlag, &g_mtxWorldGoalFlag, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aGoalFlag.pos.x, g_aGoalFlag.pos.y, g_aGoalFlag.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldGoalFlag, &g_mtxWorldGoalFlag, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGoalFlag);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatGoalFlag->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatGoalFlag; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//�e�N�X�`����̐ݒ�
		pDevice->SetTexture(0, g_apTextureGoalFlag[nCntMat]);
		//���f���i�p�[�c�j�̕`��
		g_pMeshGoalFlag->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	////���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//====================================================================
//�S�[���ƃv���C���[�̓����蔻��
//====================================================================
bool CollisionGoalFlagPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold)
{
	bool bLand = false;



	if (g_aGoalFlag.vtxMinModel.x < vtxMaxModel->x
		&&g_aGoalFlag.vtxMaxModel.x > vtxMinModel->x
		&&pMove->z <= 1.5f&&pMove->z >= -1.5
		&&pMove->x <= 1.5f&&pMove->x >= -1.5)
	{//��Q����x���̕��̒��ɂ���Ƃ�

		if (g_aGoalFlag.vtxMaxModel.z <= vtxMinModelold->z
			&&g_aGoalFlag.vtxMaxModel.z >= vtxMinModel->z)
		{//�������O�ɓ���������
			bLand = true;
		}

		if (g_aGoalFlag.vtxMinModel.z >= vtxMaxModelold->z
			&&g_aGoalFlag.vtxMinModel.z <= vtxMaxModel->z)
		{//��O���牜�ɓ���������
			bLand = true;
		}
	}


	if (g_aGoalFlag.vtxMinModel.z < vtxMaxModel->z
		&&g_aGoalFlag.vtxMaxModel.z > vtxMinModel->z
		&&pMove->x <= 1.5f&&pMove->x >= -1.5
		&&pMove->z <= 1.5f&&pMove->z >= -1.5)
	{//��Q����z���̕��̒��ɂ���Ƃ�

		if (g_aGoalFlag.vtxMaxModel.x <= vtxMinModelold->x
			&&g_aGoalFlag.vtxMaxModel.x >= vtxMinModel->x)
		{//�E���獶�ɓ���������
			
			bLand = true;
		}

		if (g_aGoalFlag.vtxMinModel.x >= vtxMaxModelold->x
			&&g_aGoalFlag.vtxMinModel.x <= vtxMaxModel->x)
		{//������E�ɓ���������
			bLand = true;
		}
	}
	return bLand;
}