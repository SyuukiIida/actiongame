#ifndef _MAIN_H_
#define _MAIN_H_
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド処理に必要

//ライブラリのリンク
#pragma comment (lib,"d3d9.lib")		//
#pragma comment (lib,"d3dx9.lib")		//
#pragma comment (lib,"dxguid.lib")		//
#pragma comment (lib,"winmm.lib")		//システム時刻処理に必要
#pragma comment (lib,"dinput8.lib")		//入力処理に必要

//マクロ定義
#define SCREEN_WIDTH		(1280)						//ウインドウの幅
#define SCREEN_HEIGHT		(720)						//ウインドウの高さ
//#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define GRAVITY				(0.8f)						//重力
#define UNDERWATER_GRAVITY	(0.08f)						//水中重力

//頂点情報[2D]の構造体を定義
//typedef struct
//{
//	D3DXVECTOR3 pos;		//頂点座標
//	float rhw;				//座標返還用変数
//	D3DCOLOR col;			//頂点カラー
//	D3DXVECTOR2 tex;		//テクスチャ座標
//}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
}VERTEX_3D;

//画面（モード）の種類
typedef enum
{
	MODE_TITLE = 0,		//タイトル画面
	MODE_TUTORIAL,		//チュートリアル画面
	MODE_GAME,			//ゲーム画面
	MODE_RESULT,		//リザルト画面
	MODE_MAX
}MODE;

//リザルトの種類
typedef enum
{
	RESULT_CREAR=0,		//クリア
	RESULT_OVER,		//負け
	RESULT_MAX
}RESULT;

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hwnd, BOOL bWinbow);
void Uninit(void);
void Updata(void);
void Draw(void);
void DrawFPS(void);
void DrawPOS(void);
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
#endif