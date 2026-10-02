#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>

/******************************************************************************
 * @fn      TitleScene
 * @brief   タイトルシーン処理
 * @return  SCENE : 次のシーン
 * @detail  入力に応じてゲーム開始または終了
 ******************************************************************************/
SCENE TitleScene()// タイトルシーン関数
{
    ClearDrawScreen();

	static bool isFirst = true;// タイトルシーンが初めて呼ばれたかどうかを判定するフラグ

	if (isFirst)// タイトルシーンが初めて呼ばれた場合
    {
		StopSoundMem(bgm_game);//ゲームシーンのBGMを停止
		StopSoundMem(bgm_result);//リザルトシーンのBGMを停止

		PlaySoundMem(bgm_title, DX_PLAYTYPE_LOOP);//タイトルシーンのBGMを再生
        isFirst = false;
    }

    DrawFormatString(30, 30, GetColor(0, 255, 0), _T("GAME STERT"));
    DrawFormatString(30, 50, GetColor(0, 255, 0), _T("Pleas Mouse Right:ゲームスタート"));
    DrawFormatString(30, 80, GetColor(255, 255, 0), _T("Mouse Left:ショット"));
    DrawFormatString(30, 100, GetColor(255, 255, 0), _T("Mouse Cursor:移動"));
    DrawFormatString(40, 120, GetColor(255, 0, 0), _T("Esc:ゲームを途中終了する"));
    DrawFormatString(30, 150, GetColor(0, 255, 255), _T("Time Rimit:100秒"));
    DrawFormatString(30, 170, GetColor(0, 255, 255), _T("時間内にできるだけスコアを稼げ！"));

    if (CheckHitKey(KEY_INPUT_ESCAPE))//エスケープキーが押されたら
    {
        DxLib_End(); // DxLibの終了処理
        exit(0);     // プログラム終了
    }

    int mouseInput = GetMouseInput();
    if (mouseInput & MOUSE_INPUT_RIGHT) 
    {
		PlaySoundMem(selectSE, DX_PLAYTYPE_NORMAL);//選択時の効果音を再生
        isFirst = true; // 次回用にリセット
		StopSoundMem(bgm_title);//タイトルシーンのBGMを停止
        return PLAY;//マウスの右クリックが押されたらゲームシーンへ移動する
    }

    ScreenFlip();
    return TITLE;
}