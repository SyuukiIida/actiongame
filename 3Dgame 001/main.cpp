//====================================================================
//
//���������
//Name �ѓc�F��
//
//====================================================================
//#include <windows.h>
#include "main.h"
#include <stdio.h>
#include "input.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "wall.h"
#include "floor.h"
#include "bullet.h"
#include "meshfield.h"
#include "obstacle.h"



//�}�N����`
#define CLASS_NAME			"windowClass"				//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME			"3Dgame"				//�E�C���h�E�̖��O
//#define SCREEN_WIDTH		(1280)						//�E�C���h�E�̕�
//#define SCREEN_HEIGHT		(720)						//�E�C���h�E�̍���
//#define ID_TIMER			(131)						//�^�C�}�[��ID
//#define TIMER_INTERVAL		(1000/60)					//�^�C�}�[�̔������o�i�~���b�j

//�O���[�o���ϐ�
LPDIRECT3D9  g_pD3D = NULL;			//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9  pDevice = NULL;	//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont =NULL;			//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;				//FPS�J�E���^�[
MODE g_mode = MODE_TITLE;			//���݂̃��[�h

//====================================================================
//���C���֐�
//====================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrew, LPSTR lpCmdline, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							//WNDCLASSEX�̃T�C�Y
		CS_CLASSDC,									//�E�B�h�E�̃X�^�C��
		WindowProc,									//�E�B���h�E�v���V�[�W���[
		0,											//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		0,											//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,									//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),				//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),					//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),					//�N���C�A���g�̈�̔w�i�F
		NULL,										//���j���[�o�[
		CLASS_NAME,									//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION) };			//�t�@�C���̃A�C�R��

	HWND hWnd;										//�E�B���h�E�n���h�����i���ʎq�j
	MSG msg;										//

													//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	DWORD dwCurrentTime;							//���ݎ���
	DWORD dwExecLastTime;							//�Ō�ɏ�����������
	DWORD dwFrameCount;								//�t���[���J�E���g
	DWORD dwFPSLastTime;							//�Ō��FPS���v����������

	//�E�C���h�E����
	hWnd = CreateWindowEx(0,						//
		CLASS_NAME,									//
		WINDOW_NAME,								//
		WS_OVERLAPPEDWINDOW,						//
		CW_USEDEFAULT,								//
		CW_USEDEFAULT,								//
		SCREEN_WIDTH,								//
		SCREEN_HEIGHT,								//
		NULL,										//
		NULL,										//
		hInstance,									//
		NULL);										//



	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return-1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;								//����������
	dwExecLastTime = timeGetTime();					//���ݎ������擾�i�ۑ��j
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);						//

	UpdateWindow(hWnd);								//

	//���b�Z�[�W���[�v
	while(1)//while (GetMessage(&msg, NULL, 0, 0) != 0)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE != 0))
		{
			if(msg.message== WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPS���v������������ۑ�
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;//�����J�n�̎���[���ݎ���]��ۑ�
				//�X�V����
				Updata();

				//�`�揈��
				Draw();

				dwFrameCount++;		//�t���[���J�E���g�����Z

			}		
		}
		
	}

	//�I������

	//����\��߂�
	timeEndPeriod(1);
	Uninit();
	//�E�C���h�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;

	//��ʃT�C�Y---------------------------------------------
	{
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };		//��ʃT�C�Y�̍\����

		//�E�C���h�E�N���X�̓o�^
		RegisterClassEx(&wcex);

		//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		//�E�C���h�E�𐶐�
		hWnd = CreateWindowEx(0,						//
			CLASS_NAME,									//
			WINDOW_NAME,								//
			WS_OVERLAPPEDWINDOW,						//
			CW_USEDEFAULT,								//
			CW_USEDEFAULT,								//
			(rect.right-rect.left),								//
			(rect.bottom-rect.top),								//
			NULL,										//
			NULL,										//
			hInstance,									//
			NULL);										//

		
	}
}

//�E�C���h�E�v���V�[�W���[
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
//	HDC hDC;					//
	PAINTSTRUCT ps;				//
//	char aString1[256] = "test";	//
//	char aString2[256] = "test";	//
//	char aString3[256] = "test";	//
//	HPEN hPen, hPenOld;
//
//	int nConditions=1;

	//static POINT leftup = { 0.0 };
	//static POINT rightdown = { 1280.720 };
	static POINT pos = { 100,100 };
	const RECT rect{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//static POINT move{ 2,3 }; //�����̎n�_�̈ړ��ʁiX����,Y�����j

	switch(uMsg)
	{

	case WM_DESTROY://�E�B���h�E�j���̃��b�Z�[�W
		//WM_QUIT���b�Z�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:	//[	ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������
				DestroyWindow(hWnd);
			}
			//�E�C���h�E��j������(KM_DSTROY���b�Z�[�W�𑗂�
			break;

		//case VK_RIGHT:	//[->]�L�[�������ꂽ
		//	pos.x++;
			////�w��̈�𖳌��ɂ���
			//InvalidateRect(hWnd, &rect, FALSE);
		//case VK_LEFT:
		//	pos.y++;
		//	//�w��̈�𖳌��ɂ���
		//	InvalidateRect(hWnd, &rect, FALSE);

		}
		break;

		//�`����I��
		EndPaint(hWnd, &ps);
		break;

	//case WM_CREATE:
	//	//�^�C�}�[�̐ݒ�iMW_TIMER���b�Z�[�W�����I�ɑ���j
	//	SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
	//	break;

	//case WM_TIMER:			//�^�C�}�[���s�̃��b�Z�[�W
	//	

	//		if (nConditions == 1)
	//		{
	//			pos.y += move.y;	//�����̎n�_�iY���W�j���ړ�����
	//		}
	//		if (pos.y <= 1 || pos.y >= 740)
	//		{
	//			nConditions = nConditions ^ 1;
	//		}
	//		else if (nConditions == 0)
	//		{
	//			pos.y -= move.y;
	//		}
	//		//pos.y += move.y;	//�����̎n�_�iY���W�j���ړ�����


	//		pos.x += move.x;	//�����̎n�_�iX���W�j���ړ�����
	//		pos.x -= move.x;

	//		
	//	

		//�w��̈�𖳌��ɂ���
		InvalidateRect(hWnd, &rect, FALSE);
		break;

		//����i�~�j�{�^�����������Ƃ�
		{
		case WM_CLOSE:  //����{�^�������̃��b�Z�[�W
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������
				DestroyWindow(hWnd);
			}

			else
			{
				return 0;		//0��Ԃ��Ȃ��ƏI�����Ă��܂�
			}
			//�E�C���h�E��j������(KM_DSTROY���b�Z�[�W�𑗂�
			break; 
		}

	case WM_LBUTTONDOWN:		//�}�E�X���N���b�N�̃��b�Z�[�W
	{					//
	SetFocus(hWnd);
	break; 
	}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//====================================================================
//����������
//====================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWinbow)
{
	D3DDISPLAYMODE d3ddm;		//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^�[

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�[�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWinbow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//DIrect3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&pDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&pDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&pDevice)))
			{
				return E_FAIL;
			}

		}
	}

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,DEFAULT_QUALITY, DEFAULT_PITCH ,"Terminal",&g_pFont);

	//�����_�[�X�e�[�g�ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	//�L�[�{�[�h����������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	////���[�h�̐ݒ�
	//SetMode(g_mode);
	////�t�F�[�h�ݒ�
	//InitFade(g_mode);
	
	//�|���S���̏���������
	//InitPolygon();

	//�ǂ̏���������
	InitWall();

	//���̏���������
	InitFloor();
	//InitMeshfield();

	//�e�̏���������
	InitShadow();

	//�r���{�[�h����������
	InitBillboard();

	//���f���̏���������
	InitModel();
	InitObstacle();

	//�e�̏���������
	InitBullet();

	//�J�����̏���������
	InitCamera();
	
	//���C�g�̏���������
	InitLight();

	return S_OK;
}


//====================================================================
//�I������
//====================================================================

void Uninit(void)
{
	////�^�C�g����ʂ̏I��
	//UninitTitle();

	////�`���[�g���A����ʂ̏I��
	//UninitTutorial();

	////�Q�[����ʂ̏I��
	//UninitGame();

	////���U���g��ʂ̏I��
	//UninitResult();

	////�t�F�[�h����
	//UninitFade();

	//�|���S���̏I������
	//UninitPolygon();

	//���̏I������
	UninitFloor();
	//UninitMeshfield();

	//�ǂ̏I������
	UninitWall();

	//�r���{�[�h�I������
	UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�e�̏I������
	UninitShadow();

	//���f���̏I������
	UninitModel();
	UninitObstacle();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�L�[�{�[�h�I������
	UninitKeyboard();

	//Direct3D�f�o�C�X�̏I������
	if (pDevice != NULL)
	{
		pDevice->Release();
		pDevice = NULL;
	}
	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//�Ńo�b�N�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//====================================================================
//�X�V����
//====================================================================

void Updata(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�|���S���̍X�V����
	//UpdatePolygon();

	//���̍X�V����
	UpdateFloor();
	//UpdateMeshfield();

	//�ǂ̍X�V����
	UpdateWall();

	//�r���{�[�h�X�V����
	UpdateBillboard();

	//���f���̍X�V����
	UpdateModel();
	UpdateObstacle();

	//�e�̍X�V����
	UpdateBullet();

	//�e�̍X�V����
	UpdateShadow();

	//�J�����̍X�V����
	UpdateCamera();

	//���C�g�̍X�V����
	UpdateLight();

	//switch (g_mode)
	//{
	//case MODE_TITLE:			//�^�C�g�����
	//	UpdateTitle();
	//	break;
	//case MODE_TUTORIAL:			//�`���[�g���A�����
	//	UpdateTutorial();
	//	break;
	//case MODE_GAME:				//�Q�[�����
	//	UpdateGame();
	//	break;
	//case MODE_RESULT:			//���U���g���
	//	UpdateResult();
	//	break;
	//}
	////�t�F�[�h�̍X�V����
	//UpdateFade();
}

//====================================================================
//�`�揈��
//====================================================================

void Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@Z�o�b�t�@�̃N���A�j
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(pDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//switch (g_mode)
		//{
		//case MODE_TITLE:			//�^�C�g�����
		//	DrawTitle();
		//	break;
		//case MODE_TUTORIAL:			//�`���[�g���A�����
		//	DrawTutorial();
		//	break;
		//case MODE_GAME:				//�Q�[�����
		//	DrawGame();
		//	break;
		//case MODE_RESULT:			//���U���g���
		//	DrawResult();
		//	break;
		//}
		////�t�F�[�h�̍X�V����
		//DrawFade();

		//�|���S���̕`�揈��
		//DrawPolygon();

		//���̍X�V����
		DrawFloor();
		//DrawMeshfield();

		//�ǂ̕`�揈��
		DrawWall();

		

		//���f���̕`�揈��
		DrawModel();
		DrawObstacle();

		//�e�̕`�揈��
		DrawShadow();

		//�r���{�[�h�`�揈��
		DrawBillboard();

		//�e�̕`�揈��
		DrawBullet();

		//�J�����̕`�揈��
		SetCamera();

#ifdef _DEBUG
		//FPS�̕\��
		/*DrawFPS();*/

		DrawPOS();
#endif

		//�`��I��
		pDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pDevice->Present(NULL, NULL, NULL, NULL);

}



//====================================================================
//FPS�`�揈��
//====================================================================

void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0], "FPS,%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}

void DrawPOS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	Camera camera= GetCamera();
	Model model = GetModel();
	char aStr[256], aStr1[256], aStr2[256];

	//������ɑ��
	sprintf(&aStr[0], "���_���W,(%0.1f:%0.1f:%0.1f)\n", camera.posR.x, camera.posR.y, camera.posR.z);
	sprintf(&aStr1[0], "\n���_���W,(%0.1f:%0.2f:%0.1f)", camera.rot.x, camera.rot.y, camera.rot.z);
	sprintf(&aStr2[0], "\n\n���f���ʒu,(%0.2f:%0.2f:%0.2f:%0.2f)", model.vtxMaxModel.z, model.vtxMaxModel.x
	, model.vtxMinModel.z, model.vtxMinModel.x);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr1[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr2[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

void SetMode(MODE mode)
{

	////���݂̉�ʁi���[�h�j�̏I������
	//switch (g_mode)
	//{
	//case MODE_TITLE:			//�^�C�g�����
	//	UninitTitle();
	//	break;
	//case MODE_TUTORIAL:			//�`���[�g���A�����
	//	UninitTutorial();
	//	break;
	//case MODE_GAME:				//�Q�[�����
	//	UninitGame();
	//	break;
	//case MODE_RESULT:			//���U���g���
	//	UninitResult();
	//	break;
	//}

	////�V������ʁi���[�h�j�̏���������
	//switch (mode)
	//{
	//case MODE_TITLE:			//�^�C�g�����
	//	InitTitle();
	//	break;
	//case MODE_TUTORIAL:			//�`���[�g���A�����
	//	InitTutorial();
	//	break;
	//case MODE_GAME:				//�Q�[�����
	//	InitGame();
	//	break;
	//case MODE_RESULT:			//���U���g���
	//	InitResult(Result);
	//	break;
	//}

	//g_mode = mode;			//���݂̉�ʁi���[�h�j��؂�ւ���

}

//====================================================================
//���[�h�̎擾
//====================================================================
MODE GetMode(void)
{
	return g_mode;
}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return pDevice;
}