


#include "main.h"
#include "title.h"
#include "input.h"
#include "titlebg.h"
#include "titolelogo.h"
#include "fade.h"

//====================================================================
//タイトル画面の初期化処理
//====================================================================
void InitTitle(void)
{
	//タイトル背景の初期化処理
	InitTitleBg();

	//タイトルロゴの初期化処理
	InitTitleLogo();
}

//====================================================================
//タイトル画面の終了処理
//====================================================================
void UninitTitle(void)
{
	//タイトル背景の終了処理
	UninitTitleBg();

	//タイトルロゴの終了処理
	UninitTitleLogo();

}

//====================================================================
//タイトル画面の更新処理
//====================================================================
void UpdateTitle(void)
{
	FADE pFade;		//フェードへのポインタ
	pFade = GetFade();

	//タイトル背景の更新処理
	UpdateTitleBg();

	//タイトルロゴの更新処理
	UpdateTitleLogo();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//決定キーが押された

		 //モード設定(ゲーム画面に移行)
			SetFade(MODE_GAME);
		}
	}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{//決定キー（ENTER）が押された
	// //モード設定（チュートリアル画面に移行）
	//	SetMode(MODE_TUTORIAL);
	//}

}

//====================================================================
//タイトル画面の描画処理
//====================================================================
void DrawTitle(void)
{
	//タイトル背景の描画処理
	DrawTitleBg();

	//タイトルロゴの描画処理
	DrawTitleLogo();
}


	/*D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\titlerogo.png",
		&g_apTextureTitle[1]);
*/
	