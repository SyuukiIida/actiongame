


#include "input.h"
#include "main.h"
#include "shadow.h"

#define MAX_SHADOW			(256)			//影の最大数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//テクスチャのポインタ
Shadow g_Shadow[MAX_SHADOW];
//D3DXVECTOR3 g_posShadow;
//D3DXVECTOR3 g_rotShadow;
D3DXMATRIX g_mtxWorldShadow;							//ワールドマトリックス

//====================================================================
//影の初期化処理
//====================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

								//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].bUse = false;
	}

	//g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-10.0f/*+(20.0f*nCntShadow)*/, 0.0f, 10.0f);
		pVtx[1].pos = D3DXVECTOR3(10.0f /*+ (20.0f*nCntShadow)*/, 0.0f, 10.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f /*+ (20.0f*nCntShadow)*/, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(10.0f /*+ (20.0f*nCntShadow)*/, 0.0f, -10.0f);

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
	}

	g_pVtxBuffShadow->Unlock();
}

//====================================================================
//影の終了処理
//====================================================================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//====================================================================
//影の更新処理
//====================================================================
void UpdateShadow(void)
{

}

//====================================================================
//影の描画処理
//====================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	//減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
											
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, 0.01f, g_Shadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);			
		}
	}

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================================================
//影の設定処理
//====================================================================
int SetShadow(void)
{
	int nCntShadow = -1;

	for (nCntShadow; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (nCntShadow == -1)
		{
			continue;
		}

		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].bUse = true;
			break;
		}
	}

	return nCntShadow;
}


//====================================================================
//影の位置設定処理
//====================================================================
void SetPositionShadow(int nIdxShadow,D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxPot, mtxTrans;			//計算用マトリックス

	g_Shadow[nIdxShadow].pos = pos;

											//ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&g_Shadow[nIdxShadow].mtxWorldShadow);
	////向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxPot, g_Shadow[nIdxShadow].rot.y, g_Shadow[nIdxShadow].rot.x, g_Shadow[nIdxShadow].rot.z);

	//D3DXMatrixMultiply(&g_Shadow[nIdxShadow].mtxWorldShadow, &g_Shadow[nIdxShadow].mtxWorldShadow, &mtxPot);

	////位置を反映
	//D3DXMatrixTranslation(&mtxTrans, g_Shadow[nIdxShadow].pos.x, 0.0f, g_Shadow[nIdxShadow].pos.z);

	//D3DXMatrixMultiply(&g_Shadow[nIdxShadow].mtxWorldShadow, &g_Shadow[nIdxShadow].mtxWorldShadow, &mtxTrans);

	////頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	////頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_3D);

	////テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureShadow);

	////描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nIdxShadow, 2);

}


void SetEndShadow(int nShadow)
{
	g_Shadow[nShadow].bUse = false;
}