


#include "input.h"
#include "main.h"
#include "camera.h"
#include "model.h"

//�O���[�o���ϐ�
Camera g_camera;		//�J�����̏��
int g_nCounterAnimCamera;

//====================================================================
//�J�����̏���������
//====================================================================

void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -200);		//���_
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����_
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//������ւ̃x�N�g��
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posfLength = sqrtf(g_camera.posV.x * g_camera.posV.x + g_camera.posV.z * g_camera.posV.z);//�Ίp���̒������Z�o����
	g_camera.fAngle = atan2f(g_camera.posV.x, g_camera.posV.z);						//����
}

//====================================================================
//�J�����̏I������
//====================================================================

void UninitCamera(void)
{

}

//====================================================================
//�J�����̍X�V����
//====================================================================

void UpdateCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	Model model = GetModel();

	g_camera.fAngle = atan2f(g_camera.posV.x, g_camera.posV.z);

	g_camera.posR = model.pos;

	/*g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y)*g_camera.posfLength;
	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.y)*g_camera.posfLength;*/

	if (GetKeyboardPress(DIK_W) == true)
	{//W(��)�L�[�������ꂽ
		g_camera.posV.x += sinf(0 + g_camera.rot.y) * 1.0f;
		g_camera.posV.z += cosf(0 + g_camera.rot.y) * 1.0f;
							   
		g_camera.posR.x += sinf(0 + g_camera.rot.y) * 1.0f;
		g_camera.posR.z += cosf(0 + g_camera.rot.y) * 1.0f;
	}

	if (GetKeyboardPress(DIK_S) == true)
	{//S(��O)�L�[�������ꂽ
		g_camera.posV.x += sinf(D3DX_PI + g_camera.rot.y) * 1.0f;
		g_camera.posV.z += cosf(D3DX_PI + g_camera.rot.y) * 1.0f;

		g_camera.posR.x += sinf(D3DX_PI + g_camera.rot.y) * 1.0f;
		g_camera.posR.z += cosf(D3DX_PI + g_camera.rot.y) * 1.0f;
	}

	if (GetKeyboardPress(DIK_E) == true)
	{//E(�����_�E��])�L�[�������ꂽ
		g_camera.rot.y += 0.01f* D3DX_PI;
	}

	if (GetKeyboardPress(DIK_Q) == true)
	{//Q(�����_����])�L�[�������ꂽ
		g_camera.rot.y -= 0.01f* D3DX_PI;
	}

	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.posfLength;
	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.posfLength;

	if (GetKeyboardPress(DIK_Z) == true)
	{//Z(���_�E��])�L�[�������ꂽ
		g_camera.rot.y -= 0.01f* D3DX_PI;


		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.posfLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.posfLength;
	}

	if (GetKeyboardPress(DIK_C) == true)
	{//C(���_����])�L�[�������ꂽ
		g_camera.rot.y += 0.01f* D3DX_PI;


		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.posfLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.posfLength;
	}

	//if (GetKeyboardPress(DIK_T) == true)
	//{//T(��)�L�[�������ꂽ
	//	g_camera.rot.y += 0.01f;


	//	g_camera.posV.z = g_camera.posR.z - sinf(D3DX_PI * g_camera.rot.y) * g_camera.posfLength;
	//	g_camera.posV.y = g_camera.posR.y - cosf(D3DX_PI * g_camera.rot.y) * g_camera.posfLength;
	//}

	if (GetKeyboardPress(DIK_T) == true)
	{//T�L�[�������ꂽ
		g_camera.posV.y += 0.5f;
	}

	if (GetKeyboardPress(DIK_B) == true)
	{//B�L�[�������ꂽ
		g_camera.posV.y -= 0.5f;
	}

	if (GetKeyboardPress(DIK_Y) == true)
	{//Y�L�[�������ꂽ
		g_camera.posR.y += 0.5f;
	}

	if (GetKeyboardPress(DIK_N) == true)
	{//N�L�[�������ꂽ
		g_camera.posR.y -= 0.5f;
	}
}

//====================================================================
//�J�����̕`�揈��
//====================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 1000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//====================================================================
//�J�����̏��
//====================================================================
Camera GetCamera(void)
{
	return g_camera;
}