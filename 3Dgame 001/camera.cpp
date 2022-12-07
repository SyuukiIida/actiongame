


#include "input.h"
#include "main.h"
#include "camera.h"
#include "model.h"

//グローバル変数
Camera g_camera;		//カメラの情報
int g_nCounterAnimCamera;

//====================================================================
//カメラの初期化処理
//====================================================================

void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -200);		//視点
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//注視点
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向へのベクトル
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posfLength = sqrtf(g_camera.posV.x * g_camera.posV.x + g_camera.posV.z * g_camera.posV.z);//対角線の長さを算出する
	g_camera.fAngle = atan2f(g_camera.posV.x, g_camera.posV.z);						//向き
}

//====================================================================
//カメラの終了処理
//====================================================================

void UninitCamera(void)
{

}

//====================================================================
//カメラの更新処理
//====================================================================

void UpdateCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	Model model = GetModel();

	g_camera.fAngle = atan2f(g_camera.posV.x, g_camera.posV.z);

	g_camera.posR = model.pos;

	/*g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y)*g_camera.posfLength;
	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.y)*g_camera.posfLength;*/

	if (GetKeyboardPress(DIK_W) == true)
	{//W(奥)キーが押された
		g_camera.posV.x += sinf(0 + g_camera.rot.y) * 1.0f;
		g_camera.posV.z += cosf(0 + g_camera.rot.y) * 1.0f;
							   
		g_camera.posR.x += sinf(0 + g_camera.rot.y) * 1.0f;
		g_camera.posR.z += cosf(0 + g_camera.rot.y) * 1.0f;
	}

	if (GetKeyboardPress(DIK_S) == true)
	{//S(手前)キーが押された
		g_camera.posV.x += sinf(D3DX_PI + g_camera.rot.y) * 1.0f;
		g_camera.posV.z += cosf(D3DX_PI + g_camera.rot.y) * 1.0f;

		g_camera.posR.x += sinf(D3DX_PI + g_camera.rot.y) * 1.0f;
		g_camera.posR.z += cosf(D3DX_PI + g_camera.rot.y) * 1.0f;
	}

	if (GetKeyboardPress(DIK_E) == true)
	{//E(注視点右回転)キーが押された
		g_camera.rot.y += 0.01f* D3DX_PI;
	}

	if (GetKeyboardPress(DIK_Q) == true)
	{//Q(注視点左回転)キーが押された
		g_camera.rot.y -= 0.01f* D3DX_PI;
	}

	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.posfLength;
	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.posfLength;

	if (GetKeyboardPress(DIK_Z) == true)
	{//Z(視点右回転)キーが押された
		g_camera.rot.y -= 0.01f* D3DX_PI;


		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.posfLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.posfLength;
	}

	if (GetKeyboardPress(DIK_C) == true)
	{//C(視点左回転)キーが押された
		g_camera.rot.y += 0.01f* D3DX_PI;


		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.posfLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.posfLength;
	}

	//if (GetKeyboardPress(DIK_T) == true)
	//{//T(左)キーが押された
	//	g_camera.rot.y += 0.01f;


	//	g_camera.posV.z = g_camera.posR.z - sinf(D3DX_PI * g_camera.rot.y) * g_camera.posfLength;
	//	g_camera.posV.y = g_camera.posR.y - cosf(D3DX_PI * g_camera.rot.y) * g_camera.posfLength;
	//}

	if (GetKeyboardPress(DIK_T) == true)
	{//Tキーが押された
		g_camera.posV.y += 0.5f;
	}

	if (GetKeyboardPress(DIK_B) == true)
	{//Bキーが押された
		g_camera.posV.y -= 0.5f;
	}

	if (GetKeyboardPress(DIK_Y) == true)
	{//Yキーが押された
		g_camera.posR.y += 0.5f;
	}

	if (GetKeyboardPress(DIK_N) == true)
	{//Nキーが押された
		g_camera.posR.y -= 0.5f;
	}
}

//====================================================================
//カメラの描画処理
//====================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 1000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//====================================================================
//カメラの情報
//====================================================================
Camera GetCamera(void)
{
	return g_camera;
}