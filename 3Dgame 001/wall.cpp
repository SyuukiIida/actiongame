//====================================================================
//
//3Dゲームポリゴン[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "wall.h"

#define MAX_WALL				(4)					//壁面数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;			//テクスチャのポインタ
Wall g_Wall[MAX_WALL];

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureWall);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 1.57f * nCntWall, 0.0f);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffWall, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx +=4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-200.0f, +50.0f, 200.0f);
		pVtx[1].pos = D3DXVECTOR3(200.0f, 50.0f, 200.0f);
		pVtx[2].pos = D3DXVECTOR3(-200.0f, 0.0f, 200.0f);
		pVtx[3].pos = D3DXVECTOR3(200.0f, 0.0f, 200.0f);

		//rhwの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f,-0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(2.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(2.0f, 1.0f);
	}

	g_pVtxBuffWall->Unlock();
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateWall(void)
{

}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//====================================================================
//壁の当たり判定
//====================================================================
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;			//当たったかどうか

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{

		if (g_Wall[nCnt].pos.z + 200.0f >= pPosOld->z
			&&g_Wall[nCnt].pos.z + 200.0f <= pPos->z)
		{
			pPos->z = g_Wall[nCnt].pos.z + 200.0f;
			pMove->z = 0.0f;
			bLand = true;

			//跳ね返り

			/*pMove->z = pMove->z*-15.0f;
			pMove->x = pMove->x*-15.0f;*/
		}

		if (g_Wall[nCnt].pos.z - 200.0f <= pPosOld->z
			&&g_Wall[nCnt].pos.z - 200.0f >= pPos->z)
		{
			pPos->z = g_Wall[nCnt].pos.z - 200.0f;
			pMove->z = 0.0f;
			bLand = true;
		}


		if (g_Wall[nCnt].pos.x + 200.0f >= pPosOld->x
			&&g_Wall[nCnt].pos.x + 200.0f <= pPos->x)
		{
			pPos->x = g_Wall[nCnt].pos.x + 200.0f;
			pMove->x = 0.0f;
			bLand = true;
		}

		if (g_Wall[nCnt].pos.x - 200.0f <= pPosOld->x
			&&g_Wall[nCnt].pos.x - 200.0f >= pPos->x)
		{
			pPos->x = g_Wall[nCnt].pos.x - 200.0f;
			pMove->x = 0.0f;
			bLand = true;
		}
	}
	

	return bLand;
}