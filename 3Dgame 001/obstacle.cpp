


#include "input.h"
#include "main.h"
#include "obstacle.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//マクロ定義
#define MAX_OBSRACLE		(2)					//障害物最大数
#define MAX_LIFE			(3)					//障害物体力

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

	//頂点情報の変数
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファポインタ

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		g_aObstacle[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f * nCnt);
		g_aObstacle[nCnt].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].nLife = MAX_LIFE;
		g_aObstacle[nCnt].bUse = true;
	}
	

	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatObstacle,
		NULL, &g_dwNumMatObstacle, &g_pMeshObstacle[0]);

	D3DXLoadMeshFromX("data\\MODEL\\moon.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatObstacle,
		NULL, &g_dwNumMatObstacle, &g_pMeshObstacle[1]);

	
	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		//頂点数を取得
		nNumVtx = g_pMeshObstacle[nCnt]->GetNumVertices();

		//頂点フォーマット
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshObstacle[nCnt]->GetFVF());

		//頂点バッファロック
		g_pMeshObstacle[nCnt]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (g_aObstacle[nCnt].vtxMinModel.x > vtx.x)
			{
				g_aObstacle[nCnt].vtxMinModel.x = vtx.x;
			}

			if (g_aObstacle[nCnt].vtxMaxModel.x < vtx.x)
			{
				g_aObstacle[nCnt].vtxMaxModel.x = vtx.x;
			}

			if (g_aObstacle[nCnt].vtxMinModel.y > vtx.y)
			{
				g_aObstacle[nCnt].vtxMinModel.y = vtx.y;
			}

			if (g_aObstacle[nCnt].vtxMaxModel.y < vtx.y)
			{
				g_aObstacle[nCnt].vtxMaxModel.y = vtx.y;
			}

			if (g_aObstacle[nCnt].vtxMinModel.z > vtx.z)
			{
				g_aObstacle[nCnt].vtxMinModel.z = vtx.z;
			}

			if (g_aObstacle[nCnt].vtxMaxModel.z < vtx.z)
			{
				g_aObstacle[nCnt].vtxMaxModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		g_pMeshObstacle[nCnt]->UnlockVertexBuffer();

		g_aObstacle[nCnt].vtxMinModel += g_aObstacle[nCnt].pos;
		g_aObstacle[nCnt].vtxMaxModel += g_aObstacle[nCnt].pos;
	}

	

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
		if (g_aObstacle[nCnt].bUse == true)
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
		}
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
	}

}

//====================================================================
//障害物のヒット処理
//====================================================================
void HitObstacle(int nCntObstacle, int nDamage)
{
	g_aObstacle[nCntObstacle].nLife -= nDamage;

	if (g_aObstacle[nCntObstacle].nLife == 0)
	{
		//爆発の設定
		//PlaySound(SOUND_LABEL_SE_EXPLOSION);

		//SetExplosion(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

		//
		g_aObstacle[nCntObstacle].bUse = false;

		SetEndShadow(g_nIdxShadowObstacle[nCntObstacle]);

	}

	//else
	//{
	//	//頂点バッファをロックし頂点情報へのポインタを取得
	//	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//	g_aObstacle[nCntObstacle].state = ENEMYSTATE_DAMAGE;
	//	g_aObstacle[nCntObstacle].nCounterState = 5;

	//	pVtx += nCntEnemy * 4;

	//	//頂点カラーの設定
	//	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	//	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	//	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	//	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	//	g_pVtxBuffEnemy->Unlock();
	//}

}

//====================================================================
//障害物と弾の当たり判定
//====================================================================
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{
			if (g_aObstacle[nCnt].vtxMinModel.x < pPos->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > pPos->x)
			{//障害物のx軸の幅の中にいるとき
				if (g_aObstacle[nCnt].vtxMaxModel.z  <= pPosOld->z
					&&g_aObstacle[nCnt].vtxMaxModel.z >= pPos->z)
				{//奥から手前に当たった時
					pPos->z = g_aObstacle[nCnt].vtxMaxModel.z;
					pMove->z = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);

					//跳ね返り

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].vtxMinModel.z >= pPosOld->z
					&&g_aObstacle[nCnt].vtxMinModel.z <= pPos->z)
				{
					pPos->z = g_aObstacle[nCnt].vtxMinModel.z;
					pMove->z = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.z < pPos->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > pPos->z)
			{
				if (g_aObstacle[nCnt].vtxMaxModel.x <= pPosOld->x
					&&g_aObstacle[nCnt].vtxMaxModel.x >= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMaxModel.x;
					pMove->x = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);
				}

				if (g_aObstacle[nCnt].vtxMinModel.x >= pPosOld->x
					&&g_aObstacle[nCnt].vtxMinModel.x <= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMinModel.x;
					pMove->x = 0.0f;
					bLand = true;
					HitObstacle(nCnt, 1);
				}
			}
		}
	}

	return bLand;
}

//====================================================================
//障害物とプレイヤーの当たり判定
//====================================================================
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{
			if (g_aObstacle[nCnt].vtxMinModel.x < pPos->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > pPos->x)
			{//障害物のx軸の幅の中にいるとき
				if (g_aObstacle[nCnt].vtxMaxModel.z <= pPosOld->z
					&&g_aObstacle[nCnt].vtxMaxModel.z >= pPos->z)
				{//奥から手前に当たった時
					pPos->z = g_aObstacle[nCnt].vtxMaxModel.z;
					pMove->z = 0.0f;
					bLand = true;

					//跳ね返り

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].vtxMinModel.z >= pPosOld->z
					&&g_aObstacle[nCnt].vtxMinModel.z <= pPos->z)
				{
					pPos->z = g_aObstacle[nCnt].vtxMinModel.z;
					pMove->z = 0.0f;
					bLand = true;
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.z < pPos->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > pPos->z)
			{
				if (g_aObstacle[nCnt].vtxMaxModel.x <= pPosOld->x
					&&g_aObstacle[nCnt].vtxMaxModel.x >= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMaxModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}

				if (g_aObstacle[nCnt].vtxMinModel.x >= pPosOld->x
					&&g_aObstacle[nCnt].vtxMinModel.x <= pPos->x)
				{
					pPos->x = g_aObstacle[nCnt].vtxMinModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}
			}
		}
	}

	return bLand;
}