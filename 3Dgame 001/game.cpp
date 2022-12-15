


#include "main.h"
#include "game.h"
#include "input.h"
#include "polygon.h"
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "wall.h"
#include "floor.h"
#include "bullet.h"
#include "meshfield.h"
#include "obstacle.h"
#include "camera.h"
#include "light.h"
#include "fade.h"



GAMESTATE g_gameState = GAMESTATE_NONE;	//ゲームの状態
int g_nCounterGameState=0;
RESULT g_gameresult;
bool g_bPause = false;

//====================================================================
//ゲーム画面の初期化処理
//====================================================================
void InitGame(void)
{
	//ファイル読み込み
	LoadObstacle();

	//壁の初期化処理
	InitWall();

	//床の初期化処理
	InitFloor();
	//InitMeshfield();

	//影の初期化処理
	InitShadow();

	//ビルボード初期化処理
	InitBillboard();

	//モデルの初期化処理
	InitModel();
	InitObstacle();

	//弾の初期化処理
	InitBullet();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//状態の初期化
	g_gameState = GAMESTATE_NORMAL;		//通常状態に設定

	g_nCounterGameState = 0;

	g_bPause = false;

	//サウンド設定
	//PlaySound(SOUND_LABEL_BGM000);

}

//====================================================================
//ゲーム画面の終了処理
//====================================================================
void UninitGame(void)
{
	//床の終了処理
	UninitFloor();
	//UninitMeshfield();

	//壁の終了処理
	UninitWall();

	//ビルボード終了処理
	UninitBillboard();

	//弾の終了処理
	UninitBullet();

	//影の終了処理
	UninitShadow();

	//モデルの終了処理
	UninitModel();
	UninitObstacle();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();
}

//====================================================================
//ゲーム画面の更新処理
//====================================================================
void UpdateGame(void)
{
	//床の更新処理
	UpdateFloor();
	//UpdateMeshfield();

	//壁の更新処理
	UpdateWall();

	//ビルボード更新処理
	UpdateBillboard();

	//モデルの更新処理
	UpdateModel();
	UpdateObstacle();

	//弾の更新処理
	UpdateBullet();

	//影の更新処理
	UpdateShadow();

	//カメラの更新処理
	UpdateCamera();

	//ライトの更新処理
	UpdateLight();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//決定キー（ENTER）が押された
	 //ゲームの状態を設定
		SetGameState(GAMESTATE_END, 30, RESULT_CREAR);
		//モード設定（ゲーム画面に移行）
		//SetMode(MODE_RESULT);
	}
#endif
	

	switch(g_gameState)
	{

	case GAMESTATE_NORMAL:		//通常状態
		break;
	case GAMESTATE_END:			//終了
		g_nCounterGameState--;

		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;	//何もしていない状態にする
			//モード設定（リザルト画面に移行）
			SetFade(MODE_RESULT);
		}

		break;
	}

	//if (敵全滅 || プレイヤー滅)
	//{
	//	//モード設定（リザルト画面に移行）
	//	SetMode(MODE_RESULT);
	//}
}

//====================================================================
//ゲーム画面の描画処理
//====================================================================
void DrawGame(void)
{
	
	//床の更新処理
	DrawFloor();
	//DrawMeshfield();

	//壁の描画処理
	DrawWall();



	//モデルの描画処理
	DrawModel();
	DrawObstacle();

	//影の描画処理
	DrawShadow();

	//ビルボード描画処理
	DrawBillboard();

	//弾の描画処理
	DrawBullet();

}
//====================================================================
//ゲームの状態の設定
//====================================================================
void SetGameState(GAMESTATE state, int nCounter, RESULT Result)
{
	g_gameState = state;
	g_gameresult = Result;
	g_nCounterGameState = nCounter;
}

//====================================================================
//ゲームの状態の取得
//====================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

RESULT GetResult(void)
{
	return g_gameresult;
}
