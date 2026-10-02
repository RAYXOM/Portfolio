#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>
#include "scor.h"
#define DEBUG 1 


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)// Windowsアプリケーションのエントリーポイント
{
	SetWaitVSyncFlag(false);// 垂直同期を無効にする

	// ウィンドウサイズを指定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // 32は色深度

	// ウインドウモードで起動
	ChangeWindowMode(true);

	// DXライブラリの初期化
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 裏画面を使用
	SetDrawScreen(DX_SCREEN_BACK);

	Init();// ゲームの初期化

	SCENE menu = TITLE;// ゲームのシーンを表す変数:初期値はタイトルシーン
	do {
		switch (menu)
		{
		case TITLE:
			menu = TitleScene();// タイトルシーンを呼び出し、次のシーンを取得
			break;
		case PLAY:
			menu = GameScene();// ゲームシーンを呼び出し、次のシーンを取得
			break;
		case EXIT:
			menu = ExitScene();// リザルトシーンを呼び出し、次のシーンを取得
			break;
		default:
			menu = END;
			break;
		}
	} while (menu != END);

	DxLib_End();// DXライブラリの使用終了

	// ソフトの終了
	return 0;
}