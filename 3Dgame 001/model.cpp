


#include "input.h"
#include "main.h"
#include "model.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "obstacle.h"
#include "wall.h"

//�O���[�o���ϐ�
LPD3DXMESH g_pMeshModel = NULL;					//���b�V���i���_���ւ̃|�C���^�j
LPD3DXBUFFER g_pBuffMatModel = NULL;			//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureModel[10] = {};	//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;						//�}�e���A���̐�
Model g_aModel;				
//D3DXVECTOR3 g_posModel;							//�ʒu
//D3DXVECTOR3 g_rotModel;							//����
//D3DXVECTOR3 g_moveModel;						//�ړ���
//D3DXMATRIX g_mtxWorldModel;						//���[���h�}�g���b�N�X

int g_nCounterAnimModel;
int g_nIdxShadow = -1;							//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j

//====================================================================
//���f���̏���������
//====================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	g_aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\cat.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatModel,
		NULL, &g_dwNumMatModel, &g_pMeshModel);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat= (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
			
				//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTextureModel[nCntMat]);
		}
	}
	//�e��ݒ�
	g_nIdxShadow = SetShadow();
}

//====================================================================
//���f���̏I������
//====================================================================
void UninitModel(void)
{
	//���b�V���̔j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//====================================================================
//���f���̍X�V����
//====================================================================
void UpdateModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	g_aModel.posold = g_aModel.pos;					//���݂̈ʒu��ۑ�

	Camera camera = GetCamera();


	if (GetKeyboardPress(DIK_LEFT) == true)
	{//A(��)�L�[�������ꂽ
		g_aModel.move.x -= sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aModel.move.z -= cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aModel.rot.y = (D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//D(�E)�L�[�������ꂽ
		g_aModel.move.x += sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aModel.move.z += cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aModel.rot.y = (-D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_UP) == true)
	{//W(��)�L�[�������ꂽ
		g_aModel.move.x += sinf(0 + camera.rot.y) * 1.0f;
		g_aModel.move.z += cosf(0 + camera.rot.y) * 1.0f;

		g_aModel.rot.y = (D3DX_PI + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//S(��O)�L�[�������ꂽ
		g_aModel.move.x += sinf(D3DX_PI + camera.rot.y) * 1.0f;
		g_aModel.move.z += cosf(D3DX_PI + camera.rot.y) * 1.0f;

		g_aModel.rot.y =(camera.rot.y);
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//SPACE�L�[�������ꂽ
	 //�e�̐ݒ�
		SetBullet();
	}

	//�ʒu���X�V
	g_aModel.pos += g_aModel.move;

	//������ǉ�
	g_aModel.move.x += (0.0f - g_aModel.move.x) * 0.5f;
	g_aModel.move.z += (0.0f - g_aModel.move.z) * 0.5f;

	//if (g_aModel.pos.z >= 190.0f)
	//{
	//	g_aModel.pos.z = 189.0f;
	//}

	//if (g_aModel.pos.z <= -190.0f)
	//{
	//	g_aModel.pos.z = -189.0f;
	//}

	//if (g_aModel.pos.x >= 190.0f)
	//{
	//	g_aModel.pos.x = 189.0f;
	//}

	//if (g_aModel.pos.x <= -190.0f)
	//{
	//	g_aModel.pos.x = -189.0f;
	//}

	if(CollisionObstacle(&g_aModel.pos, &g_aModel.posold, &g_aModel.move)==true);
	{

	}

	if (CollisionWall(&g_aModel.pos, &g_aModel.posold, &g_aModel.move) == true);
	{

	}


	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIdxShadow, g_aModel.pos);



}

//====================================================================
//���f���̕`�揈��
//====================================================================

void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//�}�e���A���ۑ��p
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aModel.mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel.rot.y, g_aModel.rot.x, g_aModel.rot.z);

	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aModel.pos.x, g_aModel.pos.y, g_aModel.pos.z);

	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aModel.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//�e�N�X�`����̐ݒ�
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);
		//���f���i�p�[�c�j�̕`��
		g_pMeshModel->DrawSubset(nCntMat);
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
Model GetModel(void)
{
	return g_aModel;
}