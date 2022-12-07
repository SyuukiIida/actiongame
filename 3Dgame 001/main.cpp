//====================================================================
//
//あくしょん
//Name 飯田洲暉
//
//====================================================================
//#include <windows.h>
#include "main.h"
#include <stdio.h>
#include "input.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "wall.h"
#include "floor.h"
#include "bullet.h"
#include "meshfield.h"
#include "obstacle.h"



//マクロ定義
#define CLASS_NAME			"windowClass"				//ウインドウクラスの名前
#define WINDOW_NAME			"3Dgame"				//ウインドウの名前
//#define SCREEN_WIDTH		(1280)						//ウインドウの幅
//#define SCREEN_HEIGHT		(720)						//ウインドウの高さ
//#define ID_TIMER			(131)						//タイマーのID
//#define TIMER_INTERVAL		(1000/60)					//タイマーの発生感覚（ミリ秒）

//グローバル変数
LPDIRECT3D9  g_pD3D = NULL;			//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9  pDevice = NULL;	//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont =NULL;			//フォントへのポインタ
int g_nCountFPS = 0;				//FPSカウンター
MODE g_mode = MODE_TITLE;			//現在のモード

//====================================================================
//メイン関数
//====================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrew, LPSTR lpCmdline, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							//WNDCLASSEXのサイズ
		CS_CLASSDC,									//ウィドウのスタイル
		WindowProc,									//ウィンドウプロシージャー
		0,											//0にすろ（通常は使用しない）
		0,											//0にすろ（通常は使用しない）
		hInstance,									//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),				//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),					//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),					//クライアント領域の背景色
		NULL,										//メニューバー
		CLASS_NAME,									//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION) };			//ファイルのアイコン

	HWND hWnd;										//ウィンドウハンドルン（識別子）
	MSG msg;										//

													//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	DWORD dwCurrentTime;							//現在時刻
	DWORD dwExecLastTime;							//最後に処理した時刻
	DWORD dwFrameCount;								//フレームカウント
	DWORD dwFPSLastTime;							//最後にFPSを計測した時刻

	//ウインドウ生成
	hWnd = CreateWindowEx(0,						//
		CLASS_NAME,									//
		WINDOW_NAME,								//
		WS_OVERLAPPEDWINDOW,						//
		CW_USEDEFAULT,								//
		CW_USEDEFAULT,								//
		SCREEN_WIDTH,								//
		SCREEN_HEIGHT,								//
		NULL,										//
		NULL,										//
		hInstance,									//
		NULL);										//



	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return-1;
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;								//初期化する
	dwExecLastTime = timeGetTime();					//現在時刻を取得（保存）
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);						//

	UpdateWindow(hWnd);								//

	//メッセージループ
	while(1)//while (GetMessage(&msg, NULL, 0, 0) != 0)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE != 0))
		{
			if(msg.message== WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPSを計測した時刻を保存
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;//処理開始の時刻[現在時刻]を保存
				//更新処理
				Updata();

				//描画処理
				Draw();

				dwFrameCount++;		//フレームカウントを加算

			}		
		}
		
	}

	//終了処理

	//分解能を戻す
	timeEndPeriod(1);
	Uninit();
	//ウインドクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;

	//画面サイズ---------------------------------------------
	{
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };		//画面サイズの構造体

		//ウインドウクラスの登録
		RegisterClassEx(&wcex);

		//クライアント領域を指定のサイズに調整
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		//ウインドウを生成
		hWnd = CreateWindowEx(0,						//
			CLASS_NAME,									//
			WINDOW_NAME,								//
			WS_OVERLAPPEDWINDOW,						//
			CW_USEDEFAULT,								//
			CW_USEDEFAULT,								//
			(rect.right-rect.left),								//
			(rect.bottom-rect.top),								//
			NULL,										//
			NULL,										//
			hInstance,									//
			NULL);										//

		
	}
}

//ウインドウプロシージャー
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
//	HDC hDC;					//
	PAINTSTRUCT ps;				//
//	char aString1[256] = "test";	//
//	char aString2[256] = "test";	//
//	char aString3[256] = "test";	//
//	HPEN hPen, hPenOld;
//
//	int nConditions=1;

	//static POINT leftup = { 0.0 };
	//static POINT rightdown = { 1280.720 };
	static POINT pos = { 100,100 };
	const RECT rect{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//static POINT move{ 2,3 }; //線分の始点の移動量（X方向,Y方向）

	switch(uMsg)
	{

	case WM_DESTROY://ウィンドウ破棄のメッセージ
		//WM_QUITメッセセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:	//[	ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する
				DestroyWindow(hWnd);
			}
			//ウインドウを破棄する(KM_DSTROYメッセージを送る
			break;

		//case VK_RIGHT:	//[->]キーが押された
		//	pos.x++;
			////指定領域を無効にする
			//InvalidateRect(hWnd, &rect, FALSE);
		//case VK_LEFT:
		//	pos.y++;
		//	//指定領域を無効にする
		//	InvalidateRect(hWnd, &rect, FALSE);

		}
		break;

		//描画を終了
		EndPaint(hWnd, &ps);
		break;

	//case WM_CREATE:
	//	//タイマーの設定（MW_TIMERメッセージを定期的に送る）
	//	SetTimer(hWnd, ID_TIMER, TIMER_INTERVAL, NULL);
	//	break;

	//case WM_TIMER:			//タイマー発行のメッセージ
	//	

	//		if (nConditions == 1)
	//		{
	//			pos.y += move.y;	//線分の始点（Y座標）を移動する
	//		}
	//		if (pos.y <= 1 || pos.y >= 740)
	//		{
	//			nConditions = nConditions ^ 1;
	//		}
	//		else if (nConditions == 0)
	//		{
	//			pos.y -= move.y;
	//		}
	//		//pos.y += move.y;	//線分の始点（Y座標）を移動する


	//		pos.x += move.x;	//線分の始点（X座標）を移動する
	//		pos.x -= move.x;

	//		
	//	

		//指定領域を無効にする
		InvalidateRect(hWnd, &rect, FALSE);
		break;

		//閉じる（×）ボタンを押したとき
		{
		case WM_CLOSE:  //閉じるボタン押下のメッセージ
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する
				DestroyWindow(hWnd);
			}

			else
			{
				return 0;		//0を返さないと終了してしまう
			}
			//ウインドウを破棄する(KM_DSTROYメッセージを送る
			break; 
		}

	case WM_LBUTTONDOWN:		//マウス左クリックのメッセージ
	{					//
	SetFocus(hWnd);
	break; 
	}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//====================================================================
//初期化処理
//====================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWinbow)
{
	D3DDISPLAYMODE d3ddm;		//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメーター

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWinbow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//DIrect3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&pDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&pDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&pDevice)))
			{
				return E_FAIL;
			}

		}
	}

	//デバック表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,DEFAULT_QUALITY, DEFAULT_PITCH ,"Terminal",&g_pFont);

	//レンダーステート設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	//キーボード初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	////モードの設定
	//SetMode(g_mode);
	////フェード設定
	//InitFade(g_mode);
	
	//ポリゴンの初期化処理
	//InitPolygon();

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

	return S_OK;
}


//====================================================================
//終了処理
//====================================================================

void Uninit(void)
{
	////タイトル画面の終了
	//UninitTitle();

	////チュートリアル画面の終了
	//UninitTutorial();

	////ゲーム画面の終了
	//UninitGame();

	////リザルト画面の終了
	//UninitResult();

	////フェード処理
	//UninitFade();

	//ポリゴンの終了処理
	//UninitPolygon();

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

	//キーボード終了処理
	UninitKeyboard();

	//Direct3Dデバイスの終了処理
	if (pDevice != NULL)
	{
		pDevice->Release();
		pDevice = NULL;
	}
	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//でバック表示用フォントの破壊
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//====================================================================
//更新処理
//====================================================================

void Updata(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ポリゴンの更新処理
	//UpdatePolygon();

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

	//switch (g_mode)
	//{
	//case MODE_TITLE:			//タイトル画面
	//	UpdateTitle();
	//	break;
	//case MODE_TUTORIAL:			//チュートリアル画面
	//	UpdateTutorial();
	//	break;
	//case MODE_GAME:				//ゲーム画面
	//	UpdateGame();
	//	break;
	//case MODE_RESULT:			//リザルト画面
	//	UpdateResult();
	//	break;
	//}
	////フェードの更新処理
	//UpdateFade();
}

//====================================================================
//描画処理
//====================================================================

void Draw(void)
{
	//画面クリア（バックバッファZバッファのクリア）
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{//描画開始が成功した場合

		//switch (g_mode)
		//{
		//case MODE_TITLE:			//タイトル画面
		//	DrawTitle();
		//	break;
		//case MODE_TUTORIAL:			//チュートリアル画面
		//	DrawTutorial();
		//	break;
		//case MODE_GAME:				//ゲーム画面
		//	DrawGame();
		//	break;
		//case MODE_RESULT:			//リザルト画面
		//	DrawResult();
		//	break;
		//}
		////フェードの更新処理
		//DrawFade();

		//ポリゴンの描画処理
		//DrawPolygon();

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

		//カメラの描画処理
		SetCamera();

#ifdef _DEBUG
		//FPSの表示
		/*DrawFPS();*/

		DrawPOS();
#endif

		//描画終了
		pDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	pDevice->Present(NULL, NULL, NULL, NULL);

}



//====================================================================
//FPS描画処理
//====================================================================

void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS,%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}

void DrawPOS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	Camera camera= GetCamera();
	Model model = GetModel();
	char aStr[256], aStr1[256], aStr2[256];

	//文字列に代入
	sprintf(&aStr[0], "視点座標,(%0.1f:%0.1f:%0.1f)\n", camera.posR.x, camera.posR.y, camera.posR.z);
	sprintf(&aStr1[0], "\n視点座標,(%0.1f:%0.2f:%0.1f)", camera.rot.x, camera.rot.y, camera.rot.z);
	sprintf(&aStr2[0], "\n\nモデル位置,(%0.2f:%0.2f:%0.2f:%0.2f)", model.vtxMaxModel.z, model.vtxMaxModel.x
	, model.vtxMinModel.z, model.vtxMinModel.x);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr1[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr2[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

void SetMode(MODE mode)
{

	////現在の画面（モード）の終了処理
	//switch (g_mode)
	//{
	//case MODE_TITLE:			//タイトル画面
	//	UninitTitle();
	//	break;
	//case MODE_TUTORIAL:			//チュートリアル画面
	//	UninitTutorial();
	//	break;
	//case MODE_GAME:				//ゲーム画面
	//	UninitGame();
	//	break;
	//case MODE_RESULT:			//リザルト画面
	//	UninitResult();
	//	break;
	//}

	////新しい画面（モード）の初期化処理
	//switch (mode)
	//{
	//case MODE_TITLE:			//タイトル画面
	//	InitTitle();
	//	break;
	//case MODE_TUTORIAL:			//チュートリアル画面
	//	InitTutorial();
	//	break;
	//case MODE_GAME:				//ゲーム画面
	//	InitGame();
	//	break;
	//case MODE_RESULT:			//リザルト画面
	//	InitResult(Result);
	//	break;
	//}

	//g_mode = mode;			//現在の画面（モード）を切り替える

}

//====================================================================
//モードの取得
//====================================================================
MODE GetMode(void)
{
	return g_mode;
}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return pDevice;
}