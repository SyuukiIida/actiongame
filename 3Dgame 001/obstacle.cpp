


#include "input.h"
#include "main.h"
#include "obstacle.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//マクロ定義
#define MAX_OBSRACLE		(2)					//障害物最大数

//グローバル変数
LPD3DXMESH g_pMeshObstacle[MAX_OBSRACLE] = {};					//メッシュ（頂点情報へのポインタ）
LPD3DXBUFFER g_pBuffMatObstacle = NULL;			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTextureObstacle[10] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatObstacle = 0;						//マテリアルの数
Obstacle g_aObstacle[MAX_OBSRACLE];

D3DXMATRIX g_mtxWorldObstacle;

int g_nIdxShadowObstacle[MAX_OBSRACLE] = { -1 };							//対象の影のインデックス（番号）

//====================================================================
//障害物の初期化処理
//====================================================================
void InitObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		g_aObstacle[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f * nCnt);
		g_aObstacle[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].bUse = true;
	}
	

	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatObstacle,
		NULL, &g_dwNumMatObstacle, &g_pMeshObstacle[0]);

	D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatObstacle,
		NULL, &g_dwNumMatObstacle, &g_pMeshObstacle[1]);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObstacle->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTextureObstacle[nCntMat]);
		}
	}

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//影を設定
		g_nIdxShadowObstacle[nCnt] = SetShadow();
	}
}

//====================================================================
//障害物の終了処理
//====================================================================
void UninitObstacle(void)
{
	//メッシュの破棄
	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_pMeshObstacle[nCnt] != NULL)
		{
			g_pMeshObstacle[nCnt]->Release();
			g_pMeshObstacle[nCnt] = NULL;
		}
	}

	//バッファの破棄
	if (g_pBuffMatObstacle != NULL)
	{
		g_pBuffMatObstacle->Release();
		g_pBuffMatObstacle = NULL;
	}
}

//====================================================================
//障害物の更新処理
//====================================================================
void UpdateObstacle(void)
{
	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//影の位置を設定
		SetPositionShadow(g_nIdxShadowObstacle[nCnt], g_aObstacle[nCnt].pos);
	}
}

//====================================================================
//障害物の描画処理
//====================================================================
void DrawObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//マテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldObstacle);
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObstacle[nCnt].rot.y, g_aObstacle[nCnt].rot.x, g_aObstacle[nCnt].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldObstacle, &g_mtxWorldObstacle, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aObstacle[nCnt].pos.x, g_aObstacle[nCnt].pos.y, g_aObstacle[nCnt].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldObstacle, &g_mtxWorldObstacle, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldObstacle);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObstacle->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			//テクスチャんの設定
			pDevice->SetTexture(0, g_apTextureObstacle[nCntMat]);
			//モデル（パーツ）の描画
			g_pMeshObstacle[nCnt]->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
	}

}

//====================================================================
//障害物の当たり判定
//====================================================================
bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{

			if (g_aObstacle[nCnt].pos.x - OBSTACLE_SIZE < pPos->x
				&&g_aObstacle[nCnt].pos.x + OBSTACLE_SIZE > pPos->x)
			{//障害物のx軸の幅の中にいるとき
				if (g_aObstacle[nCnt].pos.z + OBSTACLE_SIZE <= pPosOld->z
					&&g_aObstacle[nCnt].pos.z + OBSTACLE_SIZE >= pPos->z)
				{//奥から手前に当たった時
					pPos->z = g_aObstacle[nCnt].pos.z + OBSTACLE_SIZE;
					pMove->z = 0.0f;

					//跳ね返り

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].pos.z - OBSTACLE_SIZE >= pPosOld->z
					&&g_aObstacle[nCnt].pos.z - OBSTACLE_SIZE <= pPos->z)
				{
					pPos->z = g_aObstacle[nCnt].pos.z - OBSTACLE_SIZE;
					pMove->z = 0.0f;
				}
			}

			if (g_aObstacle[nCnt].pos.z - OBSTACLE_SIZE < pPos->z
				&&g_aObstacle[nCnt].pos.z + OBSTACLE_SIZE > pPos->z)
			{
				if (g_aObstacle[nCnt].pos.x + OBSTACLE_SIZE <= pPosOld->x
					&&g_aObstacle[nCnt].pos.x + OBSTACLE_SIZE >= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].pos.x + OBSTACLE_SIZE;
					pMove->x = 0.0f;
				}

				if (g_aObstacle[nCnt].pos.x - OBSTACLE_SIZE >= pPosOld->x
					&&g_aObstacle[nCnt].pos.x - OBSTACLE_SIZE <= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].pos.x - OBSTACLE_SIZE;
					pMove->x = 0.0f;
				}
			}
		}
	}

	return bLand;
}