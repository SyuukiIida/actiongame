


#include "main.h"
#include "input.h"
#include "titlebg.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTitleBg = NULL; //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleBg = NULL;//頂点バッファのポインタ

//====================================================================
//タイトル背景の初期化処理
//====================================================================

void InitTitleBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\water001.jpg",
		&g_apTextureTitleBg);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleBg,
		NULL);

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);



	//頂点座標の設定	
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;


	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffTitleBg->Unlock();
}

//====================================================================
//タイトル背景の終了処理
//====================================================================

void UninitTitleBg(void)
{
	//テクスチャの破棄


	if (g_apTextureTitleBg != NULL)
	{
		g_apTextureTitleBg->Release();
		g_apTextureTitleBg = NULL;
	}


	//バッファの破棄
	if (g_pVtxBuffTitleBg != NULL)
	{
		g_pVtxBuffTitleBg->Release();
		g_pVtxBuffTitleBg = NULL;
	}
}
//====================================================================
//タイトル背景の更新処理
//====================================================================

void UpdateTitleBg(void)
{
	//VERTEX_2D *pVtx;			//頂点情報へのポインタ
	//							//頂点バッファをロックし頂点情報へのポインタを取得
	//g_pVtxBuffTitleBg->Lock(0, 0, (void**)&pVtx, 0);

	////テクスチャ座標の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//g_pVtxBuffTitleBg->Unlock();

}

//====================================================================
//タイトル背景の描画処理
//====================================================================

void DrawTitleBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureTitleBg);

	//ポリゴンの描写
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//プリミティブの種類		

}