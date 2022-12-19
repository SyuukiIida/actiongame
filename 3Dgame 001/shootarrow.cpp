

#include "input.h"
#include "main.h"
#include "shootarrow.h"
#include "player.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShootArrow = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShootArrow = NULL;			//テクスチャのポインタ
ShootArrow g_ShootArrow;
//D3DXVECTOR3 g_posShadow;
//D3DXVECTOR3 g_rotShadow;
D3DXMATRIX g_mtxWorldShootArrow;							//ワールドマトリックス

//====================================================================
//影の初期化処理
//====================================================================
void InitShootArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

								//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShootArrow);


	g_ShootArrow.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_ShootArrow.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShootArrow, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffShootArrow->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, 20.0f);
	pVtx[1].pos = D3DXVECTOR3(10.0f , 0.0f, 20.0f);
	pVtx[2].pos = D3DXVECTOR3(-10.0f , 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(10.0f , 0.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	g_pVtxBuffShootArrow->Unlock();
}

//====================================================================
//影の終了処理
//====================================================================
void UninitShootArrow(void)
{
	//テクスチャの破棄
	if (g_pTextureShootArrow != NULL)
	{
		g_pTextureShootArrow->Release();
		g_pTextureShootArrow = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffShootArrow != NULL)
	{
		g_pVtxBuffShootArrow->Release();
		g_pVtxBuffShootArrow = NULL;
	}
}

//====================================================================
//影の更新処理
//====================================================================
void UpdateShootArrow(void)
{
	Player player;
	player = GetPlayer();

	g_ShootArrow.rot.y = player.rot.y;
}

//====================================================================
//影の描画処理
//====================================================================
void DrawShootArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShootArrow, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldShootArrow);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ShootArrow.rot.y, g_ShootArrow.rot.x, g_ShootArrow.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldShootArrow, &g_mtxWorldShootArrow, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_ShootArrow.pos.x, 0.01f, g_ShootArrow.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldShootArrow, &g_mtxWorldShootArrow, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShootArrow);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureShootArrow);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//====================================================================
//影の位置設定処理
//====================================================================
void SetPositionShootArrow(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxPot, mtxTrans;			//計算用マトリックス

	g_ShootArrow.pos = pos;

}