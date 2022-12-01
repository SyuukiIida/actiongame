//====================================================================
//
//3Dゲーム バレット[billboard.cpp]
//
//
//====================================================================


#include "input.h"
#include "main.h"
#include "bullet.h"
#include "model.h"
#include "shadow.h"

#define MAX_BULLET			(128)			//ビルボードの最大数
#define BULLET_LIFE			(1000)			//弾の寿命

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//テクスチャのポインタ
D3DXMATRIX g_mtxWorldBullet;
Bullet g_aBullet[MAX_BULLET];

int g_nIdxShadowBullet[MAX_BULLET] = { -1 };							//対象の影のインデックス（番号）

//====================================================================
//ポリゴンの初期化処理
//====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

								//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = BULLET_LIFE;
		g_aBullet[nCntBullet].bUse = false;
	}
	

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffBullet, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pVtx+=4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-5.0f, +10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+5.0f, +10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+5.0f, 0.0f, 0.0f);

		//rhwの設定
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

	g_pVtxBuffBullet->Unlock();

	
}

//====================================================================
//ポリゴンの終了処理
//====================================================================

void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}

//====================================================================
//ポリゴンの更新処理
//====================================================================

void UpdateBullet(void)
{
	for (int nCntBllet = 0; nCntBllet < MAX_BULLET; nCntBllet++)
	{

		if (g_aBullet[nCntBllet].bUse == true)
		{//弾が使用されてる
		

			//弾の更新処理
			g_aBullet[nCntBllet].move.x += sinf(D3DX_PI+g_aBullet[nCntBllet].rot.y) * 1.5f;
			g_aBullet[nCntBllet].move.z += cosf(D3DX_PI+g_aBullet[nCntBllet].rot.y) * 1.5f;

			//位置を更新
			g_aBullet[nCntBllet].pos += g_aBullet[nCntBllet].move;

			//感性を追加
			g_aBullet[nCntBllet].move.x += (0.0f - g_aBullet[nCntBllet].move.x) * 0.50f;
			g_aBullet[nCntBllet].move.z += (0.0f - g_aBullet[nCntBllet].move.z) * 0.5f;

			//影の位置を設定
			SetPositionShadow(g_nIdxShadowBullet[nCntBllet], g_aBullet[nCntBllet].pos);
		}

		

		

		if (g_aBullet[nCntBllet].bUse == true)
		{
			//寿命のカウントダウン
			g_aBullet[nCntBllet].nLife -= 10;

			if (g_aBullet[nCntBllet].nLife == 0)
			{//寿命が尽きた
				SetEndShadow(g_nIdxShadowBullet[nCntBllet]);

				g_aBullet[nCntBllet].bUse = false;		//使用していない状態にする		
			}
		}
	}
}

//====================================================================
//ポリゴンの描画処理
//====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

											//デバイスの取得
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBullet);

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
			D3DXMatrixInverse(&g_mtxWorldBullet, NULL, &mtxView);//逆行列を求める
																	 /*g_Billboard.mtxWorld._12 = 0.0f;
																	 g_Billboard.mtxWorld._21 = 0.0f;*/
			g_mtxWorldBullet._41 = 0.0f;
			g_mtxWorldBullet._42 = 0.0f;
			g_mtxWorldBullet._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

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
	}
}

//====================================================================
//弾の設定処理
//====================================================================
void SetBullet(void)
{
	Model model = GetModel();

	for (int nCntBllet = 0; nCntBllet < MAX_BULLET; nCntBllet++)
	{
		if (g_aBullet[nCntBllet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBllet].pos = model.pos+ D3DXVECTOR3(0.0f, 10.0f, 0.0f);;
			g_aBullet[nCntBllet].rot = model.rot;
			g_aBullet[nCntBllet].nLife = BULLET_LIFE;
			g_aBullet[nCntBllet].bUse = true;//使用されている状態にする

			 //影を設定
			g_nIdxShadowBullet[nCntBllet] = SetShadow();
			break;
		}
	}
}