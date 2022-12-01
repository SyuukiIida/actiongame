//====================================================================
//
//3Dゲームポリゴン[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "meshfield.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;			//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshfield;
D3DXVECTOR3 g_rotMeshfield;
D3DXMATRIX g_mtxWorldMeshfield;							//ワールドマトリックス

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	WORD *pIdx;					//インデックス情報へのポインタ

								//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureMeshfield);

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield, NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 9, 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-250.0f, 0.0f, 250.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 250.0f);
	pVtx[2].pos = D3DXVECTOR3(250.0f, 0.0f, 250.0f);
	pVtx[3].pos = D3DXVECTOR3(-250.0f, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(250.0f, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-250.0f, 0.0f, -250.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	pVtx[8].pos = D3DXVECTOR3(250.0f, 0.0f, -250.0f);

	//pVtx[9].pos = D3DXVECTOR3(-250.0f, 0.0f, 0.0f);
	//pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	//pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[12].pos = D3DXVECTOR3(250.0f, 0.0f, -250.0f);
	//pVtx[13].pos = D3DXVECTOR3(250.0f, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(2.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f, 2.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 4.0f);
	pVtx[5].tex = D3DXVECTOR2(2.0f, 4.0f);
	pVtx[6].tex = D3DXVECTOR2(2.0f, 4.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(2.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[11].tex = D3DXVECTOR2(2.0f, 2.0f);
	pVtx[12].tex = D3DXVECTOR2(0.0f, 4.0f);
	pVtx[13].tex = D3DXVECTOR2(2.0f, 4.0f);

	g_pVtxBuffMeshfield->Unlock();

	//インデックスバッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 3;
	pIdx[1] = 0;


	g_pVtxBuffMeshfield->Unlock();
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitMeshfield(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateMeshfield(void)
{

}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
											//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);

}