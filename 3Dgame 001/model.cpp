


#include "input.h"
#include "main.h"
#include "model.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "obstacle.h"
#include "wall.h"

//マクロ定義
#define GAUGE				(0.13f)				//ゲージ増加量

//グローバル変数
LPD3DXMESH g_pMeshModel = NULL;					//メッシュ（頂点情報へのポインタ）
LPD3DXBUFFER g_pBuffMatModel = NULL;			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTextureModel[10] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatModel = 0;						//マテリアルの数
Model g_aModel;				
//D3DXVECTOR3 g_posModel;							//位置
//D3DXVECTOR3 g_rotModel;							//向き
//D3DXVECTOR3 g_moveModel;						//移動量
//D3DXMATRIX g_mtxWorldModel;						//ワールドマトリックス

int g_nCounterAnimModel;
int g_nIdxShadow = -1;							//対象の影のインデックス（番号）

//====================================================================
//モデルの初期化処理
//====================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//頂点情報の変数
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファポインタ

	g_aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.vtxMaxModelold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\cat.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatModel,
		NULL, &g_dwNumMatModel, &g_pMeshModel);

	//頂点数を取得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマット
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (g_aModel.vtxMinModel.x > vtx.x)
		{
			g_aModel.vtxMinModel.x = vtx.x;
		}

		if (g_aModel.vtxMaxModel.x < vtx.x)
		{
			g_aModel.vtxMaxModel.x = vtx.x;
		}

		if (g_aModel.vtxMinModel.y > vtx.y)
		{
			g_aModel.vtxMinModel.y = vtx.y;
		}

		if (g_aModel.vtxMaxModel.y < vtx.y)
		{
			g_aModel.vtxMaxModel.y = vtx.y;
		}

		if (g_aModel.vtxMinModel.z > vtx.z)
		{
			g_aModel.vtxMinModel.z = vtx.z;
		}

		if (g_aModel.vtxMaxModel.z < vtx.z)
		{
			g_aModel.vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}

	g_pMeshModel->UnlockVertexBuffer();

	g_aModel.vtxMinModel += g_aModel.pos;
	g_aModel.vtxMaxModel += g_aModel.pos;

	//マテリアル情報に対するポインタを取得
	pMat= (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			
				//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTextureModel[nCntMat]);
		}
	}
	//影を設定
	g_nIdxShadow = SetShadow();
}

//====================================================================
//モデルの終了処理
//====================================================================
void UninitModel(void)
{
	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//バッファの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//====================================================================
//モデルの更新処理
//====================================================================
void UpdateModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	g_aModel.posold = g_aModel.pos;					//現在の位置を保存
	g_aModel.vtxMaxModelold = g_aModel.vtxMaxModel;
	g_aModel.vtxMinModelold = g_aModel.vtxMinModel;
	
	Camera camera = GetCamera();

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//A(左)キーが押された
		g_aModel.move.x -= sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aModel.move.z -= cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aModel.rot.y = (D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//D(右)キーが押された
		g_aModel.move.x += sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aModel.move.z += cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aModel.rot.y = (-D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_UP) == true)
	{//W(奥)キーが押された
		g_aModel.move.x += sinf(0 + camera.rot.y) * 1.0f;
		g_aModel.move.z += cosf(0 + camera.rot.y) * 1.0f;

		g_aModel.rot.y = (D3DX_PI + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//S(手前)キーが押された
		g_aModel.move.x += sinf(D3DX_PI + camera.rot.y) * 1.0f;
		g_aModel.move.z += cosf(D3DX_PI + camera.rot.y) * 1.0f;

		g_aModel.rot.y =(camera.rot.y);
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//SPACEキーが押された
	 //進行方向の設定
		//SetGauge();

	}


	//g_aModel.move.y -= 0.001f;

	//位置を更新
	g_aModel.pos += g_aModel.move;
	g_aModel.vtxMaxModel += g_aModel.move;
	g_aModel.vtxMinModel += g_aModel.move;

	//g_aModel.vtxMaxModel = g_aModel.pos + g_aModel.vtxMaxModel;
	//g_aModel.vtxMinModel = g_aModel.pos + g_aModel.vtxMinModel;

	//感性を追加
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

	if (CollisionObstaclePlayer(&g_aModel.pos, &g_aModel.posold, &g_aModel.move,&g_aModel.vtxMaxModel
		,&g_aModel.vtxMinModel,&g_aModel.vtxMaxModelold, &g_aModel.vtxMinModelold) == true)
	{

	}

	if (CollisionWallPlayer(&g_aModel.pos, &g_aModel.posold, &g_aModel.move, &g_aModel.vtxMaxModel
		, &g_aModel.vtxMinModel, &g_aModel.vtxMaxModelold, &g_aModel.vtxMinModelold) == true)
	{

	}


	//影の位置を設定
	SetPositionShadow(g_nIdxShadow, g_aModel.pos);



}

//====================================================================
//モデルの描画処理
//====================================================================

void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//マテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aModel.mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel.rot.y, g_aModel.rot.x, g_aModel.rot.z);

	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aModel.pos.x, g_aModel.pos.y, g_aModel.pos.z);

	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aModel.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャんの設定
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);
		//モデル（パーツ）の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	////頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	//{
	//	//テクスチャの設定
	//	pDevice->SetTexture(0, g_apTextureModel[nCntMat]);
	//	////描画
	//	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//}
	
}

//====================================================================
//モデルの情報
//====================================================================
Model GetModel(void)
{
	return g_aModel;
}

//====================================================================
//モデルの移動量設定処理
//====================================================================
//float SetGauge(void)
//{
//	g_aModel.gauge = 0;
//
//	while (GetKeyboardPress(DIK_SPACE) == false)
//	{
//		if (g_aModel.gauge > 0 && g_aModel.gauge < 10.0f)
//		{
//			g_aModel.gauge += GAUGE;
//		}
//	
//
//	}
//}
