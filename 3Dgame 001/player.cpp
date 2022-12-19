


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

//マクロ定義
#define GAUGE				(0.43f)				//ゲージ増加量


//グローバル変数
LPD3DXMESH g_pMeshPlayer = NULL;					//メッシュ（頂点情報へのポインタ）
LPD3DXBUFFER g_pBuffMatPlayer = NULL;			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTexturePlayer[10] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatPlayer = 0;						//マテリアルの数
Player g_aPlayer;				
int g_nPlayerCounter;
int g_nCoolTimePlayer;

D3DXMATRIX g_mtxWorldModelPlayer;						//ワールドマトリックス

int g_nCounterAnimPlayer;
int g_nIdxShadowPlayer = -1;							//対象の影のインデックス（番号）

//====================================================================
//モデルの初期化処理
//====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//頂点情報の変数
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファポインタ

	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.gauge = 0;
	g_nPlayerCounter = 0;
	g_nCoolTimePlayer = 0;


	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\golfball.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatPlayer,
		NULL, &g_dwNumMatPlayer, &g_pMeshPlayer);

	//頂点数を取得
	nNumVtx = g_pMeshPlayer->GetNumVertices();

	//頂点フォーマット
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshPlayer->GetFVF());

	//頂点バッファロック
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

	//マテリアル情報に対するポインタを取得
	pMat= (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			
				//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTexturePlayer[nCntMat]);
		}
	}
	//影を設定
	g_nIdxShadowPlayer = SetShadow();
}

//====================================================================
//モデルの終了処理
//====================================================================
void UninitPlayer(void)
{
	//メッシュの破棄
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//バッファの破棄
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//====================================================================
//モデルの更新処理
//====================================================================
void UpdatePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	g_aPlayer.posold = g_aPlayer.pos;					//現在の位置を保存
	g_aPlayer.vtxMaxModelold = g_aPlayer.vtxMaxModel;
	g_aPlayer.vtxMinModelold = g_aPlayer.vtxMinModel;

	Camera camera = GetCamera();

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_LEFT) == true)
	{//A(左)キーが押された
		g_aPlayer.move.x -= sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aPlayer.move.z -= cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//D(右)キーが押された
		g_aPlayer.move.x += sinf(D3DX_PI / 2 + camera.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI / 2 + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (-D3DX_PI / 2 + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_UP) == true)
	{//W(奥)キーが押された
		g_aPlayer.move.x += sinf(0 + camera.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(0 + camera.rot.y) * 1.0f;

		g_aPlayer.rot.y = (D3DX_PI + camera.rot.y);
	}

	if (GetKeyboardPress(DIK_DOWN) == true)
	{//S(手前)キーが押された
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
	{//SPACEキーが離された
	 //進行方向の設定
		g_aPlayer.move.x += sinf(camera.rot.y) * g_aPlayer.gauge;
		g_aPlayer.move.z += cosf(camera.rot.y) * g_aPlayer.gauge;

		g_aPlayer.gauge = 0;

		g_nCoolTimePlayer = COOLTIME;
	}

	if (g_nCoolTimePlayer > 0)
	{
		g_aPlayer.gauge = 0;
	}

	//タイマー始動
	g_nPlayerCounter++;

	if ((g_nPlayerCounter % 60) == 0)
	{//一定時間経過
		g_nPlayerCounter = 0;		//カウンターを初期値に戻す

		g_nCoolTimePlayer--;
	}

	g_aPlayer.rot.y = (camera.rot.y);

	//位置を更新
	g_aPlayer.pos += g_aPlayer.move;
	g_aPlayer.vtxMaxModel += g_aPlayer.move;
	g_aPlayer.vtxMinModel += g_aPlayer.move;

	//感性を追加
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.05f;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.05f;

	//当たり判定------------------------------------------------
	{
		//モデル当たり判定----------------------------------------------------------
		if (CollisionObstaclePlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONOBSTACLE_Z) == true)
		{
			//跳ね返り
			g_aPlayer.move.z *= -0.90f;
		}

		if (CollisionObstaclePlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONOBSTACLE_X) == true)
		{
			//跳ね返り
			g_aPlayer.move.x *= -0.90f;
		}

		//壁当たり判定-------------------------------------------------------
		if (CollisionWallPlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONWALL_Z) == true)
		{
			//跳ね返り
			g_aPlayer.move.z *= -0.90f;
		}

		if (CollisionWallPlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold, COLLISIONWALL_X) == true)
		{
			//跳ね返り
			g_aPlayer.move.x *= -0.90f;
		}

		//ゴール当たり判定-------------------------------------------------------
		if (CollisionGoalFlagPlayer(&g_aPlayer.pos, &g_aPlayer.posold, &g_aPlayer.move, &g_aPlayer.vtxMaxModel
			, &g_aPlayer.vtxMinModel, &g_aPlayer.vtxMaxModelold, &g_aPlayer.vtxMinModelold) == true)
		{
			SetGameState(GAMESTATE_END, 10, RESULT_CREAR);
		}
	}

	//影の位置を設定
	SetPositionShadow(g_nIdxShadowPlayer, g_aPlayer.pos);

	SetPositionShootArrow(g_aPlayer.pos);

	//SetGameState(GAMESTATE_END, 30, RESULT_CREAR);

}

//====================================================================
//モデルの描画処理
//====================================================================

void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//マテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModelPlayer);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldModelPlayer, &g_mtxWorldModelPlayer, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldModelPlayer, &g_mtxWorldModelPlayer, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModelPlayer);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャんの設定
		pDevice->SetTexture(0, g_apTexturePlayer[nCntMat]);
		//モデル（パーツ）の描画
		g_pMeshPlayer->DrawSubset(nCntMat);
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
Player GetPlayer(void)
{
	return g_aPlayer;
}

//====================================================================
//モデルの移動量設定処理
//====================================================================
float GetGauge(void)
{
	return g_aPlayer.gauge;
}
