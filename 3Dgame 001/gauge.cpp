

#include "main.h"
#include "game.h"
#include "gauge.h"
#include "player.h"
#include "input.h"



//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGauge = NULL;			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;		//頂点バッファへのポインタ
Gauge g_aGauge;
int g_nGaugeCounter;
int g_nCoolTime;

D3DXMATRIX mtxWorldGauge;

//===============================
//スコアの初期化の処理 
//===============================
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

								//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\numbr01.png",
		&g_pTextureGauge);

	g_aGauge.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nGaugeCounter = 0;
	g_nCoolTime = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//nor(法線)の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


	//頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//===============================
//スコアの終了処理
//===============================
void UninitGauge(void)
{
	//テクスチャの破棄
	if (g_pTextureGauge != NULL)
	{
		g_pTextureGauge->Release();
		g_pTextureGauge = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//===============================
//スコアの更新処理
//===============================
void UpdateGauge(void)
{
	Player model = GetPlayer();
	float nGauge;
	VERTEX_3D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	
	g_aGauge.pos = model.pos + (D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		nGauge = model.gauge / MAX_GAUGE;

		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(-15.0f, MAX_GAUGE * nGauge, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(-5.0f, MAX_GAUGE * nGauge, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);

		if(nGauge>=0.9f)
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
			pVtx[1].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
			pVtx[2].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
			pVtx[3].col = D3DXCOLOR(1.0f , 1.0f , 0.0f, 0.9f);
		}

		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);
			pVtx[1].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);
			pVtx[2].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);
			pVtx[3].col = D3DXCOLOR(1.0f * nGauge, 1.0f - (1.0f * nGauge), 0.0f, 0.6f);

		}

	}

	if (GetKeyboardRelease(DIK_SPACE) == true)
	{//SPACEキーが離された
		g_nCoolTime = COOLTIME;
	}

	g_nGaugeCounter++;

	if ((g_nGaugeCounter % 60) == 0)
	{//一定時間経過
		g_nGaugeCounter = 0;		//カウンターを初期値に戻す

		g_nCoolTime--;

		if (g_nCoolTime == 0)
		{
			//テクスチャ座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	//頂点バッファをアンロックする
		g_pVtxBuffGauge->Unlock();

}

//===============================
//スコアの描画処理
//===============================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

											//デバイスの取得
	pDevice = GetDevice();


	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorldGauge);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);


	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&mtxWorldGauge, NULL, &mtxView);//逆行列を求める														
	mtxWorldGauge._41 = 0.0f;
	mtxWorldGauge._42 = 0.0f;
	mtxWorldGauge._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aGauge.pos.x, g_aGauge.pos.y, g_aGauge.pos.z);

	D3DXMatrixMultiply(&mtxWorldGauge, &mtxWorldGauge, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldGauge);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGauge);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
}
