#include "main.h"
#include "result.h"
#include "input.h"
#include "resultjudge.h"
#include "fade.h"
//#include "score.h"



//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureresult = NULL; //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffresult = NULL;//頂点バッファのポインタ
RESULT g_Result;
bool Bonus = false;

//====================================================================
//ポリゴンの初期化処理
//====================================================================

void InitResult(RESULT result)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	g_Result = result;
		
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\water001.jpg",
		&g_apTextureresult);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffresult,
		NULL);

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffresult->Lock(0, 0, (void**)&pVtx, 0);



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


	g_pVtxBuffresult->Unlock();

	//ゲーム結果初期化処理
	InitResultJudge(g_Result);

}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitResult(void)
{
	//テクスチャの破棄
	if (g_apTextureresult != NULL)
	{			   
		g_apTextureresult->Release();
		g_apTextureresult = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffresult != NULL)
	{			  
		g_pVtxBuffresult->Release();
		g_pVtxBuffresult = NULL;
	}

	//ゲーム結果終了処理
	UninitResultJudge();

}
//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateResult(void)
{
	FADE pFade;		//フェードへのポインタ
	pFade = GetFade();

	VERTEX_2D *pVtx;			//頂点情報へのポインタ
								//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffresult->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffresult->Unlock();

	//ゲーム結果更新処理
	UpdateResultJudge();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//決定キーが押された

		 //モード設定(ゲーム画面に移行)
			SetFade(MODE_TITLE);
		}
	}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{//決定キー（ENTER）が押された
	// //モード設定（ゲーム画面に移行）
	//	SetMode(MODE_TITLE);
	//}
}

//====================================================================
//ポリゴンの描画処理
//====================================================================

void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffresult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描写
	pDevice->SetTexture(0, g_apTextureresult);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//プリミティブの種類	


	//ゲーム結果描画処理
	DrawResultJudge();

	

}

