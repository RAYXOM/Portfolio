#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>
#include "scor.h"

// ★追加
extern int playerShotSE;

/******************************************************************************
 * @fn      Init
 * @brief   初期化処理
 ******************************************************************************/
void Init(void)// ゲームの初期化関数
{
	// スコアとタイマーのリセット
	scor = 0;// スコアを０に初期化
	damagecount = 0;// 被弾回数を０に初期化
	attackcount = 0;// 攻撃回数を０に初期化
	startTime = GetNowCount();// ゲーム開始時間を現在のカウントで初期化

	// 弾データのリセット
	for (int i = 0; i < MAX_ENEMYSHOT; i++)// 敵の弾の数だけループ
	{
		enemyShot[i].validFlag = false;// 敵の弾が無効な状態で初期化
	}
	for (int i = 0; i < MAX_PLAYERSHOT; i++) // 自機の弾の数だけループ
	{
		playerShot[i].validFlag = false;// 自機の弾が無効な状態で初期化
	}

	// 敵・自機の初期化
	//敵の初期位置を画面中央の少し上に設定
	enemy.x = 320 << SHIFT_NUM;
	enemy.y = 80 << SHIFT_NUM;

    // 敵の行動パターンを管理するカウンタをリセット
	enemy.counter = 0;
	enemy.counter2 = 0;
	enemy.angle = 0.0;

	//pulayerの初期位置を画面中央に設定
	player.x = 320 << SHIFT_NUM;
	player.y = 400 << SHIFT_NUM;

	LoadHighScore();//ハイスコアの読み込み

	//効果音をロード
	playerShotSE = LoadSoundMem(_T("resource/shoot15.ogg"));
	enemyShotSE = LoadSoundMem(_T("resource/shoot1.ogg"));
	playerDamageSE = LoadSoundMem(_T("resource/damage3.ogg"));
	enemyDamageSE = LoadSoundMem(_T("resource/damage4.ogg"));
    selectSE = LoadSoundMem(_T("resource/select2.ogg"));
	//効果音の音量調整（0～255）
	ChangeVolumeSoundMem(150, playerShotSE);
	ChangeVolumeSoundMem(150, enemyShotSE);
	ChangeVolumeSoundMem(150, playerDamageSE);
	ChangeVolumeSoundMem(150, enemyDamageSE);
    ChangeVolumeSoundMem(200, selectSE);
	//BGMをロード
	bgm_title = LoadSoundMem(_T("resource/title.mp3"));
	bgm_game = LoadSoundMem(_T("resource/maou_bgm_8bit25.mp3"));
	bgm_result = LoadSoundMem(_T("resource/maou_bgm_8bit23.mp3"));
	//BGMの音量調整（0～255）
	ChangeVolumeSoundMem(170, bgm_title);
	ChangeVolumeSoundMem(170, bgm_game);
	ChangeVolumeSoundMem(170, bgm_result);	
}
