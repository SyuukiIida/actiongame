#include "main.h"
#include "input.h"
#include "resultjudge.h"
#include "game.h"


//マクロ定義
#define NUM_JUDGE				(2)			//	ジャッチの数

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureResultJudge[NUM_JUDGE] = {}; //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultJudge = NULL;//頂点バッファのポインタ
RESULT g_result;

//====================================================================
//ポリゴンの初期化処理
//====================================================================

void InitResultJudge()
{
	g_result = GetResult();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\crear000.png",
		&g_apTextureResultJudge[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameover000.png",
		&g_apTextureResultJudge[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_JUDGE ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultJudge,
		NULL);

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResultJudge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntJudge = 0; nCntJudge < NUM_JUDGE; nCntJudge++,pVtx+=4)
	{
		//頂点座標の設定	
		pVtx[0].pos = D3DXVECTOR3(425.0f, 240.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(855.0f, 240.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(425.0f, 480.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(855.0f, 480.0f, 0.0f);


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
	}

	


	g_pVtxBuffResultJudge->Unlock();

}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitResultJudge(void)
{
	for (int nCntJudge = 0; nCntJudge < NUM_JUDGE; nCntJudge++)
	{
		//テクスチャの破棄
		if (g_apTextureResultJudge[nCntJudge] != NULL)
		{
			g_apTextureResultJudge[nCntJudge]->Release();
			g_apTextureResultJudge[nCntJudge] = NULL;
		}
	}

	//バッファの破棄
	if (g_pVtxBuffResultJudge != NULL)
	{
		g_pVtxBuffResultJudge->Release();
		g_pVtxBuffResultJudge = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateResultJudge(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResultJudge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += g_result * 4;

	//頂点座標の設定	
	pVtx[0].pos = D3DXVECTOR3(425.0f, 240.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(855.0f, 240.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(425.0f, 480.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(855.0f, 480.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffResultJudge->Unlock();

}

//====================================================================
//ポリゴンの描画処理
//====================================================================

void DrawResultJudge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultJudge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定


	/*pDevice->SetTexture(0, g_apTextureBG[1]);

	pDevice->SetTexture(0, g_apTextureBG[2]);*/

	//ポリゴンの描写
	
		pDevice->SetTexture(0, g_apTextureResultJudge[g_result]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, g_result * 4, 2);	//プリミティブの種類	
	
}