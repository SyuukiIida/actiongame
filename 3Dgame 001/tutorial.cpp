
//
//
#include "main.h"
#include "tutorial.h"
#include "input.h"

//マクロ定義
#define NUM_TUTORIAL				(1)			//	チュートリアルの数

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTUTORIAL[NUM_TUTORIAL] = {}; //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTUTORIAL = NULL;//頂点バッファのポインタ
float g_aTexVt[NUM_TUTORIAL];

//====================================================================
//ポリゴンの初期化処理
//====================================================================

void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	int nCntBG;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\water001.jpg",
		&g_apTextureTUTORIAL[0]);

	//テクスチャ座標の開始位置（V値）の初期化
	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		g_aTexVt[nCntBG] = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTUTORIAL,
		NULL);

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTUTORIAL->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG] + 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffTUTORIAL->Unlock();
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitTutorial(void)
{

	for (int nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		//テクスチャの破棄
		if (g_apTextureTUTORIAL[nCntBG] != NULL)
		{
			g_apTextureTUTORIAL[nCntBG]->Release();
			g_apTextureTUTORIAL[nCntBG] = NULL;
		}
	}

	//バッファの破棄
	if (g_pVtxBuffTUTORIAL != NULL)
	{
		g_pVtxBuffTUTORIAL->Release();
		g_pVtxBuffTUTORIAL = NULL;
	}
}
//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateTutorial(void)
{
	int nCntBG;

	VERTEX_2D *pVtx;			//頂点情報へのポインタ
								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTUTORIAL->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		//g_aTexV[nCntBG] -=0.0005f*((nCntBG+1)*3);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexVt[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexVt[nCntBG] + 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffTUTORIAL->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//決定キー（ENTER）が押された
	 //モード設定（ゲーム画面に移行）
		SetMode(MODE_GAME);
	}
}

//====================================================================
//ポリゴンの描画処理
//====================================================================

void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	int nCntBG;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTUTORIAL, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描写
	for (nCntBG = 0; nCntBG < NUM_TUTORIAL; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTUTORIAL[nCntBG]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBG, 2);	//プリミティブの種類	
	}

}