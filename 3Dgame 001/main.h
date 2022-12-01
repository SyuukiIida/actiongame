#ifndef _MAIN_H_
#define _MAIN_H_
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�����ɕK�v

//���C�u�����̃����N
#pragma comment (lib,"d3d9.lib")		//
#pragma comment (lib,"d3dx9.lib")		//
#pragma comment (lib,"dxguid.lib")		//
#pragma comment (lib,"winmm.lib")		//�V�X�e�����������ɕK�v
#pragma comment (lib,"dinput8.lib")		//���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH		(1280)						//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)						//�E�C���h�E�̍���
//#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define GRAVITY				(0.8f)						//�d��
#define UNDERWATER_GRAVITY	(0.08f)						//�����d��

//���_���[2D]�̍\���̂��`
//typedef struct
//{
//	D3DXVECTOR3 pos;		//���_���W
//	float rhw;				//���W�Ԋҗp�ϐ�
//	D3DCOLOR col;			//���_�J���[
//	D3DXVECTOR2 tex;		//�e�N�X�`�����W
//}VERTEX_2D;

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
}VERTEX_3D;

//��ʁi���[�h�j�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A�����
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
	MODE_MAX
}MODE;

//���U���g�̎��
typedef enum
{
	RESULT_CREAR=0,		//�N���A
	RESULT_OVER,		//����
	RESULT_MAX
}RESULT;

//�v���g�^�C�v�錾
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