



#include "input.h"
#include "main.h"
#include "light.h"

#define MAX_LIGFT			(3)		//ライトの最大数

//グローバル変数
D3DLIGHT9 g_light;		//ライトの情報

//====================================================================
//ライトの初期化処理
//====================================================================

void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル

	//ライトの情報をクリアする
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	g_light.Type = D3DLIGHT_DIRECTIONAL;

	//ライトの各参考を設定
	g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8, -0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトルを正規化する
	g_light.Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(0,&g_light);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
}

//====================================================================
//ライトの終了処理
//====================================================================

void UninitLight(void)
{

}

//====================================================================
//ライトの更新処理
//====================================================================

void UpdateLight(void)
{

}