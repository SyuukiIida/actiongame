//====================================================================
//
//3Dゲームポリゴン[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "polygon.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;			//テクスチャのポインタ
D3DXVECTOR3 g_posPolygon;
D3DXVECTOR3 g_rotPolygon;
D3DXMATRIX g_mtxWorldPolygon;							//ワールドマトリックス

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTexturePolygon);

	g_posPolygon = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-500.0f,0.0f, 500.0f);
	pVtx[1].pos = D3DXVECTOR3(500.0f, 0.0f, 500.0f);
	pVtx[2].pos = D3DXVECTOR3(-500.0f, 0.0f, -500.0f);
	pVtx[3].pos = D3DXVECTOR3(500.0f, 0.0f, -500.0f);

	//rhwの設定
	pVtx[0].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[1].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[2].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[3].nor =D3DXVECTOR3(0.0f,1.0f,0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 10.0f);
	pVtx[3].tex = D3DXVECTOR2(10.0f, 10.0f);

	g_pVtxBuffPolygon->Unlock();
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdatePolygon(void)
{

}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}