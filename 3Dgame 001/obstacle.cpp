


#include "input.h"
#include "main.h"
#include "obstacle.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <d3dx9.h>


//マクロ定義
#define MAX_OBSRACLE		(10)					//障害物最大数
#define NAM_OBSRACLE		(10)					//障害物の種類

#define MAX_LIFE			(3)					//障害物体力
#define TXT_MAX				(5000)				//最大文字数

//グローバル変数
int g_MAX_OBSRACLE;					//障害物最大数
int g_NAM_OBSRACLE;					//障害物の種類

LPD3DXMESH g_pMeshObstacle[NAM_OBSRACLE] = {};					//メッシュ（頂点情報へのポインタ）
LPD3DXBUFFER g_pBuffMatObstacle[MAX_OBSRACLE] = {};			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTextureObstacle[NAM_OBSRACLE][10] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatObstacle[MAX_OBSRACLE] = { 0 };						//マテリアルの数

Obstacle g_aObstacle[MAX_OBSRACLE];
char g_aFileNameObstacle[256][128];			//ファイル名

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
		g_aObstacle[nCnt].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aObstacle[nCnt].nLife = MAX_LIFE;
		g_aObstacle[nCnt].bUse = false;
	}
	

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
		//xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(&g_aFileNameObstacle[g_aObstacle[nCnt].type][0], D3DXMESH_SYSTEMMEM,
			pDevice, NULL, &g_pBuffMatObstacle[nCnt],
			NULL, &g_dwNumMatObstacle[nCnt], &g_pMeshObstacle[nCnt])))
		{
			assert(false);
		}
	}
	////xファイルの読み込み
	//D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
	//	pDevice, NULL, &g_pBuffMatObstacle[0],
	//	NULL, &g_dwNumMatObstacle[0], &g_pMeshObstacle[0]);
	//D3DXLoadMeshFromX("data\\MODEL\\moon.x", D3DXMESH_SYSTEMMEM,
	//	pDevice, NULL, &g_pBuffMatObstacle[1],
	//	NULL, &g_dwNumMatObstacle[1], &g_pMeshObstacle[1]);


	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
	/*while (true)
	{

	}*/
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

			//マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCnt]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCnt]; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//テクスチャファイルが存在する

				 //テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_apTextureObstacle[g_aObstacle[nCnt].type][nCntMat]);
				}
			}

			g_aObstacle[nCnt].bUse = true;

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
	for (int nCnt = 0; nCnt < NAM_OBSRACLE; nCnt++)
	{
		if (g_pMeshObstacle[nCnt] != NULL)
		{
			g_pMeshObstacle[nCnt]->Release();
			g_pMeshObstacle[nCnt] = NULL;
		}
	}

	//バッファの破棄
	for (int nCnt = 0; nCnt < NAM_OBSRACLE; nCnt++)
	{
		if (g_pBuffMatObstacle[nCnt] != NULL)
		{
			g_pBuffMatObstacle[nCnt]->Release();
			g_pBuffMatObstacle[nCnt] = NULL;
		}
	}
}

//====================================================================
//障害物の更新処理
//====================================================================
void UpdateObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ


	
	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[g_aObstacle[nCnt].type]->GetBufferPointer();

		if (g_aObstacle[nCnt].bUse == true)
		{
			
			//影の位置を設定
			SetPositionShadow(g_nIdxShadowObstacle[nCnt], g_aObstacle[nCnt].pos);

			switch (g_aObstacle[nCnt].state)
			{
			case ENEMYSTATE_NORMAL:

				break;

			case ENEMYSTATE_DAMAGE:
				g_aObstacle[nCnt].nCounterState--;

				if (g_aObstacle[nCnt].nCounterState <= 0)
				{
					g_aObstacle[nCnt].state = ENEMYSTATE_NORMAL;

					for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCnt]; nCntMat++)
					{
						//頂点カラーの設定
						pMat[nCntMat].MatD3D.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
					}
				}
				break;
			}
		}

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

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
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
			pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCnt]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCnt]; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャんの設定
				pDevice->SetTexture(0, g_apTextureObstacle[g_aObstacle[nCnt].type][nCntMat]);

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DMATERIAL9 matDef;							//マテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

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

	else
	{
		g_aObstacle[nCntObstacle].state = ENEMYSTATE_DAMAGE;
		g_aObstacle[nCntObstacle].nCounterState = 50;

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCntObstacle]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatObstacle[nCntObstacle]; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャんの設定
			pDevice->SetTexture(0, g_apTextureObstacle[g_aObstacle[nCntObstacle].type][nCntMat]);

		}


		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

}

//====================================================================
//障害物と弾の当たり判定
//====================================================================
bool CollisionObstacleBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
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
bool CollisionObstaclePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold)
{
	bool bLand = false;

	for (int nCnt = 0; nCnt < g_MAX_OBSRACLE; nCnt++)
	{
		if (g_aObstacle[nCnt].bUse == true)
		{

			if (g_aObstacle[nCnt].vtxMinModel.x < vtxMaxModel->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > vtxMinModel->x
				&&g_aObstacle[nCnt].vtxMinModel.y < vtxMaxModel->y
				&&g_aObstacle[nCnt].vtxMaxModel.y > vtxMinModel->y)
			{//障害物のx軸の幅の中にいるとき
				if (g_aObstacle[nCnt].vtxMaxModel.z <= vtxMinModelold->z
					&&g_aObstacle[nCnt].vtxMaxModel.z >= vtxMinModel->z)
				{//奥から手前に当たった時
					//pPos->z = g_aObstacle[nCnt].vtxMaxModel.z;

					pPos->z = g_aObstacle[nCnt].vtxMaxModel.z - (vtxMinModel->z - pPos->z);
					vtxMaxModel->z = g_aObstacle[nCnt].vtxMaxModel.z  - (vtxMinModel->z - vtxMaxModel->z);
					vtxMinModel->z = g_aObstacle[nCnt].vtxMaxModel.z;

					pMove->z = 0.0f;
					bLand = true;

					//跳ね返り

					/*pMove->z = pMove->z*-15.0f;
					pMove->x = pMove->x*-15.0f;*/
				}

				if (g_aObstacle[nCnt].vtxMinModel.z >= vtxMaxModelold->z
					&&g_aObstacle[nCnt].vtxMinModel.z <= vtxMaxModel->z)
				{//手前から奥に当たった時
					pPos->z = g_aObstacle[nCnt].vtxMinModel.z - (vtxMaxModel->z - pPos->z);
					vtxMinModel->z = g_aObstacle[nCnt].vtxMinModel.z - (vtxMaxModel->z - vtxMinModel->z);
					vtxMaxModel->z = g_aObstacle[nCnt].vtxMinModel.z;
					pMove->z = 0.0f;
					bLand = true;
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.z < vtxMaxModel->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > vtxMinModel->z
				&&g_aObstacle[nCnt].vtxMinModel.y < vtxMaxModel->y
				&&g_aObstacle[nCnt].vtxMaxModel.y > vtxMinModel->y)
			{//障害物のz軸の幅の中にいるとき

				if (g_aObstacle[nCnt].vtxMaxModel.x <= vtxMinModelold->x
					&&g_aObstacle[nCnt].vtxMaxModel.x >= vtxMinModel->x)
				{//右から左に当たった時
					pPos->x = g_aObstacle[nCnt].vtxMaxModel.x - (vtxMinModel->x - pPos->x);
					vtxMaxModel->x = g_aObstacle[nCnt].vtxMaxModel.x - (vtxMinModel->x - vtxMaxModel->x);
					vtxMinModel->x = g_aObstacle[nCnt].vtxMaxModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}

				if (g_aObstacle[nCnt].vtxMinModel.x >= vtxMaxModelold->x
					&&g_aObstacle[nCnt].vtxMinModel.x <= vtxMaxModel->x)
				{//左から右に当たった時
					pPos->x = g_aObstacle[nCnt].vtxMinModel.x - (vtxMaxModel->x - pPos->x);
					vtxMinModel->x = g_aObstacle[nCnt].vtxMinModel.x - (vtxMaxModel->x - vtxMinModel->x);
					vtxMaxModel->x = g_aObstacle[nCnt].vtxMinModel.x;
					pMove->x = 0.0f;
					bLand = true;
				}
			}

			if (g_aObstacle[nCnt].vtxMinModel.x < pPos->x
				&&g_aObstacle[nCnt].vtxMaxModel.x > pPos->x
				&&g_aObstacle[nCnt].vtxMinModel.z < pPos->z
				&&g_aObstacle[nCnt].vtxMaxModel.z > pPos->z)
			{//xz判定の中にいるとき
				if (g_aObstacle[nCnt].vtxMaxModel.y <= pPosOld->y
					&&g_aObstacle[nCnt].vtxMaxModel.y >= pPos->y)
				{//右から左に当たった時
					pPos->y = g_aObstacle[nCnt].vtxMaxModel.y;
					pMove->y = 0.0f;
					bLand = true;
				}

				if (g_aObstacle[nCnt].vtxMinModel.y >= pPosOld->y
					&&g_aObstacle[nCnt].vtxMinModel.y <= pPos->y)
				{
					pPos->y = g_aObstacle[nCnt].vtxMinModel.y;
					pMove->y = 0.0f;
					bLand = true;
				}
			}
		}
	}

	//for (int nCnt = 0; nCnt < MAX_OBSRACLE; nCnt++)
	//{
	//	if (g_aObstacle[nCnt].bUse == true)
	//	{
	//		if (g_aObstacle[nCnt].vtxMinModel.x < vtxMaxModel->x
	//			&&g_aObstacle[nCnt].vtxMaxModel.x > vtxMinModel->x)
	//		{//障害物のx軸の幅の中にいるとき
	//			if (g_aObstacle[nCnt].vtxMaxModel.z <= vtxMinModelold->z
	//				&&g_aObstacle[nCnt].vtxMaxModel.z >= vtxMinModel->z)
	//			{//奥から手前に当たった時
	//				pPos->z = g_aObstacle[nCnt].vtxMaxModel.z - vtxMinModel->z;
	//				vtxMinModel->z = g_aObstacle[nCnt].vtxMaxModel.z;
	//				pMove->z = 0.0f;
	//				bLand = true;

	//				//跳ね返り

	//				/*pMove->z = pMove->z*-15.0f;
	//				pMove->x = pMove->x*-15.0f;*/
	//			}

	//			if (g_aObstacle[nCnt].vtxMinModel.z >= vtxMaxModelold->z
	//				&&g_aObstacle[nCnt].vtxMinModel.z <= vtxMaxModel->z)
	//			{
	//				pPos->z = g_aObstacle[nCnt].vtxMinModel.z;
	//				pMove->z = 0.0f;
	//				bLand = true;
	//			}
	//		}

	//		if (g_aObstacle[nCnt].vtxMinModel.z < vtxMaxModel->z
	//			&&g_aObstacle[nCnt].vtxMaxModel.z > vtxMinModel->z)
	//		{
	//			if (g_aObstacle[nCnt].vtxMaxModel.x <= vtxMinModelold->x
	//				&&g_aObstacle[nCnt].vtxMaxModel.x >= vtxMinModel->x)
	//			{
	//				pPos->x = g_aObstacle[nCnt].vtxMaxModel.x;
	//				pMove->x = 0.0f;
	//				bLand = true;
	//			}

	//			if (g_aObstacle[nCnt].vtxMinModel.x >= vtxMinModelold->x
	//				&&g_aObstacle[nCnt].vtxMinModel.x <= vtxMinModel->x)
	//			{
	//				pPos->x = g_aObstacle[nCnt].vtxMinModel.x;
	//				pMove->x = 0.0f;
	//				bLand = true;
	//			}
	//		}
	//	}
	//}

	return bLand;
}

//====================================================================
//障害物の読み込み
//====================================================================
void LoadObstacle(void)
{
	FILE *pFile;				// ファイルポインタ

	Obstacle *pObstacle = &g_aObstacle[0];
	g_MAX_OBSRACLE = 0;
	g_NAM_OBSRACLE = 0;


	char aDataSearch[TXT_MAX];	// データ検索用
	int  nCntPass = { 0 };

	// 種類毎の情報のデータファイルを開く
	pFile = fopen("data\\txt\\ModelData.txt", "r");

	if (pFile != NULL)
	{//ファイルが開かれた
		while (1)
		{
			//fscanf(pFile,"%s", aDataSearch);		//一行書き込み
			fgets(&aDataSearch[0], 128, pFile);

			while (aDataSearch[0] == '\t')
			{//左詰めにする
				char ahoge[128];

				strcpy(&ahoge[0], &aDataSearch[0]);

				strcpy(&aDataSearch[0], &ahoge[1]);
			}

			//サーチ
			if (strcmp(aDataSearch, "END") == 0)
			{ 	//ファイルを閉じる	
				fclose(pFile); break;
			}

			if (aDataSearch[0] == '#')
			{ // 折り返す
				continue;
			}
			
		
			if (strncmp(aDataSearch, "NUM_MODEL", 9) == 0)
			{//モデルの種類
				char *pSprit;
				pSprit = strtok(&aDataSearch[0], " =");
				pSprit = strtok(NULL," =");
				g_NAM_OBSRACLE = atoi(pSprit);
			}

			if (strncmp(aDataSearch, "MODEL_FILENAME",13) == 0)
			{//障害物のファイルの相対パス
				char *pSprit;
				pSprit = strtok(&aDataSearch[0], " =");
				pSprit = strtok(NULL, " =");
				pSprit = strtok(pSprit, "\n");
				strcpy(&g_aFileNameObstacle[nCntPass][0], pSprit);
				nCntPass++;

				if (aDataSearch[0] == '#')
				{ // 折り返す
					continue;
				}
			}

			if (strcmp(aDataSearch, "MODELSET\n") == 0)
			{
				// CHR:敵[00] の種類毎の情報の読み込みを開始
				while (1)
				{
					fgets(&aDataSearch[0], 128, pFile);

					while (aDataSearch[0] == '\t')
					{//左詰めにする
						char ahoge[128];

						strcpy(&ahoge[0], &aDataSearch[0]);

						strcpy(&aDataSearch[0], &ahoge[1]);
					}

					if (strcmp(aDataSearch, "END_MODELSET\n") == 0)
					{// 読み込みを終了
						pObstacle++;
						g_MAX_OBSRACLE++;
						break;
					}

					else if (strncmp(aDataSearch, "TYPE",4) == 0)
					{ 	// 敵の種類
						char *pSprit;
						pSprit = strtok(&aDataSearch[0], " =");		//空白までの文字を使わない
						pSprit = strtok(NULL, " =");				//空白の後の文字を読み込む
						pObstacle->type = atoi(pSprit);
					}

					else if (strncmp(aDataSearch, "POS",3) == 0)
					{		// 位置
						char *pSprit;
						pSprit = strtok(&aDataSearch[0], " =");		//空白までの文字を使わない
						pSprit = strtok(NULL, " =");				//空白の後の文字を読み込む
						pObstacle->pos.x = (float)atof(pSprit);

						//空白の後の文字を読み込む
						pSprit = strtok(NULL, " =");
						pObstacle->pos.y = (float)atof(pSprit);

						//空白の後の文字を読み込む
						pSprit = strtok(NULL, " =");
						pObstacle->pos.z = (float)atof(pSprit);

						//fscanf(pFile, "%f", &pObstacle->pos.x);	// X
						//fscanf(pFile, "%f", &pObstacle->pos.y);	// Y
						//fscanf(pFile, "%f", &pObstacle->pos.z);	// Z
					}

					else if (strcmp(aDataSearch, "ROT") == 0)
					{// 向き
						char *pSprit;
						pSprit = strtok(&aDataSearch[0], " =");		//空白までの文字を使わない
						pSprit = strtok(NULL, " =");				//空白の後の文字を読み込む
						pObstacle->rot.x = (float)atof(pSprit);			//X

						//空白の後の文字を読み込む
						pSprit = strtok(NULL, " =");
						pObstacle->rot.y = (float)atof(pSprit);			//Y

						//空白の後の文字を読み込む
						pSprit = strtok(NULL, " =");
						pObstacle->rot.z = (float)atoi(pSprit);			//Z
						
					}
				}
			}
			
		}



		if (pFile == NULL)
		{// 種類毎の情報のデータファイルが開けなかった場合、
		 //処理を終了する
			return;
		}

		//Obstacle *pObstacle = g_aObstacle;
	}
}