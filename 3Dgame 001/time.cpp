

#include "main.h"
#include "game.h"
#include "time.h"
#include "player.h"
#include "camera.h"

//�}�N����`
#define NUM_PLACE  (2)								 //�X�R�A�̌���

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//���_�o�b�t�@�ւ̃|�C���^
TIME g_aTime[NUM_PLACE];
int g_nTime;											//�^�C���̒l
int g_nTimeCounter;									//�^�C���̃J�E���^�[	
int g_nTimePattern;									//�^�C���̃p�^�[��

D3DXMATRIX mtxWorldTime;

//===============================
//�X�R�A�̏������̏��� 
//===============================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number01.png",
		&g_pTextureTime);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		g_aTime[nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_nTimePattern = 0;
	g_nTimeCounter = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - 10.0f, g_aTime[nCntTime].pos.y + 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + 10.0f, g_aTime[nCntTime].pos.y + 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - 10.0f, g_aTime[nCntTime].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + 10.0f, g_aTime[nCntTime].pos.y, 0.0);
			
		//nor(�@��)�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//===============================
//�X�R�A�̏I������
//===============================
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//===============================
//�X�R�A�̍X�V����
//===============================
void UpdateTime(void)
{
	Player model = GetPlayer();
	Camera camera = GetCamera();
	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx+=4)
	{
		g_aTime[nCntTime].pos = camera.posR + (D3DXVECTOR3(0.0f,60.0f, 0.0f));

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-15.0f + (nCntTime * 15.0f), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f + (nCntTime * 15.0f), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-15.0f + (nCntTime * 15.0f), 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f + (nCntTime * 15.0f), 0.0f, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();

	g_nTimeCounter++;

	if ((g_nTimeCounter % 60) == 0)
	{//��莞�Ԍo��
		g_nTimeCounter = 0;		//�J�E���^�[�������l�ɖ߂�

			AddTime(1);
	}
}

//===============================
//�X�R�A�̕`�揈��
//===============================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

											//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorldTime);

		//Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//�A���t�@�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 10);


		//�r���[�}�g���b�N�X�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&mtxWorldTime, NULL, &mtxView);//�t�s������߂�														
		mtxWorldTime._41 = 0.0f;
		mtxWorldTime._42 = 0.0f;
		mtxWorldTime._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aTime[nCntTime].pos.x, g_aTime[nCntTime].pos.y, g_aTime[nCntTime].pos.z);

		D3DXMatrixMultiply(&mtxWorldTime, &mtxWorldTime, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorldTime);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTime);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);

		//Z�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


		//�A���t�@�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	}
}
//{
//	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
//	int nCntTime;
//
//	pDevice = GetDevice();	    //�f�o�C�X�̎擾
//
//								//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));
//
//	//���_�̃t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	//�e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, g_pTextureTime);
//
//	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
//	{
//		//�|���S���̕`��
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
//	}
//}

//===============================
//�X�R�A�̐ݒ菈��
//===============================
void SetTime(int nTime)
{
	int aTexU[NUM_PLACE];
	int nCntTime;
	VERTEX_3D * pVtx;

	g_nTime = nTime;
	
	aTexU[0] = g_nTime % 100 / 10;
	aTexU[1] = g_nTime % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//===============================
//�X�R�A�̌��Z����
//===============================
void AddTime(int nValue)
{
	if (GetGameState() == GAMESTATE_NORMAL )
	{

		int aTexU[NUM_PLACE];
		int nCntTime;
		VERTEX_3D * pVtx;

		g_nTime -= nValue;



		aTexU[0] = g_nTime % 100 / 10;
		aTexU[1] = g_nTime % 10 / 1;

		//�������Ԃ�0�ɂȂ�����Q�[���I�[�o�[
		if (g_nTime == 0)
		{
			SetGameState(GAMESTATE_END, 20, RESULT_OVER);
		}

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 1.0f);
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTime->Unlock();
	}
}