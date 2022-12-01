//====================================================================
//
//3Dゲームビルボード[billboard.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "billboard.h"

#define MAX_BILLBOARD			(4)						//ビルボードの最大数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;			//テクスチャのポインタ
Billboard g_Billboard;

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tree000.png",
		&g_pTextureBillboard);

	g_Billboard.pos = D3DXVECTOR3(0.0f, 0.0f,190.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Billboard.pos.x - 40.0f, g_Billboard.pos.y + 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Billboard.pos.x + 40.0f, g_Billboard.pos.y + 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Billboard.pos.x - 40.0f, g_Billboard.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Billboard.pos.x + 40.0f, g_Billboard.pos.y , 0.0f);
	
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffBillboard->Unlock();
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateBillboard(void)
{

}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Billboard.mtxWorld);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

#ifdef _DEBUG
	int i=0;

	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		i++;
	}

	if (i % 2 == 0)
	{
		//Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	else
	{
		//Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

#endif

	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_Billboard.mtxWorld, NULL,&mtxView);//逆行列を求める
	/*g_Billboard.mtxWorld._12 = 0.0f;
	g_Billboard.mtxWorld._21 = 0.0f;*/
	g_Billboard.mtxWorld._41 = 0.0f;
	g_Billboard.mtxWorld._42 = 0.0f;
	g_Billboard.mtxWorld._43 = 0.0f;
	
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Billboard.pos.x, g_Billboard.pos.y, g_Billboard.pos.z);

	D3DXMatrixMultiply(&g_Billboard.mtxWorld, &g_Billboard.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Billboard.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

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