#pragma once
#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>

struct ScoreData {
    int score;          // スコア
    char name[16];      // プレイヤー名
};

extern int newRecordIndex;// 新記録の順位を格納する変数


/******************************************************************************
 * @fn      SaveHighScore
 * @brief   ハイスコア保存
 ******************************************************************************/
void SaveHighScore();

/******************************************************************************
 * @fn      LoadHighScore
 * @brief   ハイスコア読み込み
 ******************************************************************************/
void LoadHighScore();

/******************************************************************************
 * @fn      DrawHighScore
 * @brief   ハイスコア描画
 ******************************************************************************/
void DrawHighScore();

/******************************************************************************
 * @fn      UpdateRanking
 * @brief   ランキング更新
 ******************************************************************************/
void UpdateRanking(int newScore);
