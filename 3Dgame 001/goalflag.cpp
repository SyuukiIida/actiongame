




#include "main.h"
#include "goalflag.h"

//グローバル変数
LPD3DXMESH g_pMeshGoalFlag = NULL;					//メッシュ（頂点情報へのポインタ）
LPD3DXBUFFER g_pBuffMatGoalFlag = NULL;			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_apTextureGoalFlag[10] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatGoalFlag = 0;						//マテリアルの数
GoalFlag g_aGoalFlag;

D3DXMATRIX g_mtxWorldGoalFlag;


//====================================================================
//ゴールの初期化処理
//====================================================================
void InitGoalFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//頂点情報の変数
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファポインタ

	g_aGoalFlag.pos = D3DXVECTOR3(0.0f, -12.5f, 250.0f);
	g_aGoalFlag.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoalFlag.vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoalFlag.vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoalFlag.state = GOALSTATE_NORMAL;

	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\pot.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &g_pBuffMatGoalFlag,
		NULL, &g_dwNumMatGoalFlag, &g_pMeshGoalFlag);


	//頂点数を取得
	nNumVtx = g_pMeshGoalFlag->GetNumVertices();

	//頂点フォーマット
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshGoalFlag->GetFVF());

	//頂点バッファロック
	g_pMeshGoalFlag->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (g_aGoalFlag.vtxMinModel.x > vtx.x)
		{
			g_aGoalFlag.vtxMinModel.x = vtx.x;
		}

		if (g_aGoalFlag.vtxMaxModel.x < vtx.x)
		{
			g_aGoalFlag.vtxMaxModel.x = vtx.x;
		}

		if (g_aGoalFlag.vtxMinModel.y > vtx.y)
		{
			g_aGoalFlag.vtxMinModel.y = vtx.y;
		}

		if (g_aGoalFlag.vtxMaxModel.y < vtx.y)
		{
			g_aGoalFlag.vtxMaxModel.y = vtx.y;
		}

		if (g_aGoalFlag.vtxMinModel.z > vtx.z)
		{
			g_aGoalFlag.vtxMinModel.z = vtx.z;
		}

		if (g_aGoalFlag.vtxMaxModel.z < vtx.z)
		{
			g_aGoalFlag.vtxMaxModel.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}

	g_pMeshGoalFlag->UnlockVertexBuffer();

	g_aGoalFlag.vtxMinModel += g_aGoalFlag.pos;
	g_aGoalFlag.vtxMaxModel += g_aGoalFlag.pos;

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatGoalFlag->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatGoalFlag; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_apTextureGoalFlag[nCntMat]);
		}
	}
}

//====================================================================
//ゴールの終了処理
//====================================================================
void UninitGoalFlag(void)
{
	//メッシュの破棄
	if (g_pMeshGoalFlag != NULL)
	{
		g_pMeshGoalFlag->Release();
		g_pMeshGoalFlag = NULL;
	}

	//バッファの破棄
	if (g_pBuffMatGoalFlag != NULL)
	{
		g_pBuffMatGoalFlag->Release();
		g_pBuffMatGoalFlag = NULL;
	}
}

//====================================================================
//ゴールの更新処理
//====================================================================
void UpdateGoalFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatGoalFlag->GetBufferPointer();//マテリアルデータへのポインタ

	switch (g_aGoalFlag.state)
	{
	case GOALSTATE_NORMAL:

		break;

	case GOALSTATE_GOAL:
		g_aGoalFlag.nCounterState--;

		if (g_aGoalFlag.nCounterState <= 0)
		{
			g_aGoalFlag.state = GOALSTATE_NORMAL;

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatGoalFlag; nCntMat++)
			{
				//頂点カラーの設定
				pMat[nCntMat].MatD3D.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}
		break;
	}
}

//====================================================================
//ゴールの描画処理
//====================================================================
void DrawGoalFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//マテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

													//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldGoalFlag);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoalFlag.rot.y, g_aGoalFlag.rot.x, g_aGoalFlag.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldGoalFlag, &g_mtxWorldGoalFlag, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aGoalFlag.pos.x, g_aGoalFlag.pos.y, g_aGoalFlag.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldGoalFlag, &g_mtxWorldGoalFlag, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGoalFlag);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatGoalFlag->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatGoalFlag; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャんの設定
		pDevice->SetTexture(0, g_apTextureGoalFlag[nCntMat]);
		//モデル（パーツ）の描画
		g_pMeshGoalFlag->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	////頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//====================================================================
//ゴールとプレイヤーの当たり判定
//====================================================================
bool CollisionGoalFlagPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove
	, D3DXVECTOR3 *vtxMaxModel, D3DXVECTOR3 *vtxMinModel, D3DXVECTOR3 *vtxMaxModelold, D3DXVECTOR3 *vtxMinModelold)
{
	bool bLand = false;



	if (g_aGoalFlag.vtxMinModel.x < vtxMaxModel->x
		&&g_aGoalFlag.vtxMaxModel.x > vtxMinModel->x
		&&pMove->z <= 1.5f&&pMove->z >= -1.5
		&&pMove->x <= 1.5f&&pMove->x >= -1.5)
	{//障害物のx軸の幅の中にいるとき

		if (g_aGoalFlag.vtxMaxModel.z <= vtxMinModelold->z
			&&g_aGoalFlag.vtxMaxModel.z >= vtxMinModel->z)
		{//奥から手前に当たった時
			bLand = true;
		}

		if (g_aGoalFlag.vtxMinModel.z >= vtxMaxModelold->z
			&&g_aGoalFlag.vtxMinModel.z <= vtxMaxModel->z)
		{//手前から奥に当たった時
			bLand = true;
		}
	}


	if (g_aGoalFlag.vtxMinModel.z < vtxMaxModel->z
		&&g_aGoalFlag.vtxMaxModel.z > vtxMinModel->z
		&&pMove->x <= 1.5f&&pMove->x >= -1.5
		&&pMove->z <= 1.5f&&pMove->z >= -1.5)
	{//障害物のz軸の幅の中にいるとき

		if (g_aGoalFlag.vtxMaxModel.x <= vtxMinModelold->x
			&&g_aGoalFlag.vtxMaxModel.x >= vtxMinModel->x)
		{//右から左に当たった時
			
			bLand = true;
		}

		if (g_aGoalFlag.vtxMinModel.x >= vtxMaxModelold->x
			&&g_aGoalFlag.vtxMinModel.x <= vtxMaxModel->x)
		{//左から右に当たった時
			bLand = true;
		}
	}
	return bLand;
}