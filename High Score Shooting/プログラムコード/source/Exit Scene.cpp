#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>
#include "scor.h"

/******************************************************************************
 * @fn      ExitScene
 * @brief   リザルトシーン処理
 * @return  SCENE : ゲーム終了またはタイトルシーン
 * @detail  入力に応じてゲーム終了またはタイトルシーンに遷移する
 ******************************************************************************/
SCENE ExitScene()// リザルトシーン関数
{
	char Nextname[16];
	strcpy_s(Nextname, "PLAYER"); // 文字列の初期化

	static bool isFirst = true;// リザルトシーンが初めて呼ばれたかどうかを判定するフラグ

	if (isFirst)// リザルトシーンが初めて呼ばれた場合
	{
		StopSoundMem(bgm_game);//ゲームシーンのBGMを停止
		PlaySoundMem(bgm_result, DX_PLAYTYPE_LOOP);//リザルトシーンのBGMを再生
		isFirst = false;
	}

	ClearDrawScreen();
	
	DrawFormatString(30, 30, GetColor(0, 255, 0), _T("GAME OVER"));
	DrawFormatString(30, 50, GetColor(255, 255, 0), _T("SCORE: %d"), scor);
	DrawFormatString(40, 70, GetColor(255, 255, 0), _T("Attack: %d"), attackcount);
	DrawFormatString(40, 90, GetColor(255, 255, 0), _T("Damage: %d"), damagecount);
	DrawHighScore();//これまでのハイスコアを表示する
	DrawFormatString(30, 110, GetColor(0, 255, 255), _T("SPACE:GAME TITLE"));
	DrawFormatString(30, 130, GetColor(0, 255, 255), _T("Mouse Right:GAME END"));
	if (damagecount == 0)//一度も被弾していなかったら
	{
		DrawFormatString(150,50, GetColor(255, 0, 0), _T("No Damage Bonus! score+%d!!"),attackcount);
	}

	int mouseInput = GetMouseInput();
	if (CheckHitKey(KEY_INPUT_SPACE))//キーボードのスペースキーが押されたら
	{
		PlaySoundMem(selectSE, DX_PLAYTYPE_BACK);//選択時の効果音を再生
		isFirst = true;
		StopSoundMem(bgm_result);
		Init();//ゲームを初期化する
		
		return TITLE;// タイトル画面に戻る
	}
	else if (mouseInput & MOUSE_INPUT_RIGHT)//マウスの右クリックが押されたら
	{
		isFirst = true;
		StopSoundMem(bgm_result);
		DxLib_End(); // DxLibの終了処理
		exit(0);     // プログラム終了
	}
	ScreenFlip();
	return EXIT;
}


