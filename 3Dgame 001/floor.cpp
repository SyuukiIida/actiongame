//====================================================================
//
//3Dゲームポリゴン[polygon.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "floor.h"



//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFloor = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureFloor = NULL;			//テクスチャのポインタ
Floor g_Floor;

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\grass001.jpg",
		&g_pTextureFloor);

	g_Floor.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Floor.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffFloor, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFloor->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-FLOOR_SIZE,0.0f, FLOOR_SIZE);
	pVtx[1].pos = D3DXVECTOR3(FLOOR_SIZE, 0.0f, FLOOR_SIZE);
	pVtx[2].pos = D3DXVECTOR3(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
	pVtx[3].pos = D3DXVECTOR3(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);

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
	pVtx[1].tex = D3DXVECTOR2(6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 6.0f);
	pVtx[3].tex = D3DXVECTOR2(6.0f, 6.0f);

	g_pVtxBuffFloor->Unlock();
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitFloor(void)
{
	//テクスチャの破棄
	if (g_pTextureFloor != NULL)
	{
		g_pTextureFloor->Release();
		g_pTextureFloor = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffFloor != NULL)
	{
		g_pVtxBuffFloor->Release();
		g_pVtxBuffFloor = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateFloor(void)
{

}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawFloor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Floor.mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Floor.rot.y, g_Floor.rot.x, g_Floor.rot.z);

	D3DXMatrixMultiply(&g_Floor.mtxWorld, &g_Floor.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Floor.pos.x, g_Floor.pos.y, g_Floor.pos.z);

	D3DXMatrixMultiply(&g_Floor.mtxWorld, &g_Floor.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Floor.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFloor, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFloor);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}