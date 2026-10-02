#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>
#include "scor.h"

int newRecordIndex = -1;// 新記録の順位を格納する変数、初期値は-1（新記録なし）
ScoreData ranking[RANKING_MAX];// ランキング用の構造体配列

/******************************************************************************
 * @fn      SaveHighScore
 * @brief   ハイスコア保存
 * @param   なし
 * @return  なし
 * @sa
 * @detail  現在のランキング情報をファイルに保存する
 ******************************************************************************/
void SaveHighScore()//書き込み
{
	FILE* fp = fopen("highscore.txt", "w");
	if (fp == NULL) 
    {
		// ファイルが開けなかった場合はエラー処理（例: ログ出力）を行う
		return;
	}
	// 構造体配列を丸ごとファイルに書き込む
	fwrite(ranking, sizeof(ScoreData), RANKING_MAX, fp);
	fclose(fp);
}

/******************************************************************************
 * @fn      LoadHighScore
 * @brief   ハイスコア読み込み
 * @param   なし
 * @return  なし
 * @sa
 * @detail  現在のランキング情報を読み込む
 ******************************************************************************/
void LoadHighScore()//読み込み
{
	FILE* fp = fopen("highscore.txt", "r");// ファイルを読み込みモードで開く
    if (fp == NULL)
    {
        // ファイルがない場合は初期スコアを設定
        for (int i = 0; i < RANKING_MAX; i++)
        {
            ranking[i].score = (RANKING_MAX - i) * 1000; // 例: 5000, 4000, 3000...
            sprintf_s(ranking[i].name, "PLAYER %d", i + 1);
        }
        SaveHighScore(); // 初期データを保存しておく
        return;
    }

    // ファイルからデータを一括読み込み
    fread(ranking, sizeof(ScoreData), RANKING_MAX, fp);
	fclose(fp);// ファイルを閉じる
}


/******************************************************************************
 * @fn      DrewHighScore
 * @brief   ハイスコア表示
 * @param   なし
 * @return  なし
 * @sa
 * @detail  現在のランキング情報を画面に表示する
 ******************************************************************************/
void DrawHighScore()//スコアの表示
{
	unsigned int normalColor = GetColor(255, 255, 255);// 白色
    unsigned int newRecordColor = GetColor(255, 255, 0); // 黄色
	unsigned int textColor = normalColor;// デフォルトは白色
    DrawString(100, 160, _T("=== HIGH SCORE RANKING ==="), normalColor);
	for (int i = 0; i < RANKING_MAX; i++)// ランキングの数だけループ
    {
		if (i == newRecordIndex)// 新記録の順位をチェック 
        {
			textColor = newRecordColor; // 新記録の順位は黄色で表示
		}
        else
        {
			textColor = normalColor; // それ以外は白色で表示
        }
        DrawFormatString(80, 180 + i * 30, textColor,
			_T("No.%d  -  %6d pts"), i + 1, ranking[i].score);// 順位とスコアを表示
    }
}

/******************************************************************************
 * @fn      UpdateHighScore
 * @brief   ハイスコア更新
 * @param   なし
 * @return  なし
 * @sa
 * @detail  現在のランキング情報を新しいスコアで更新する
 ******************************************************************************/
void UpdateRanking(int newScore) //ランキングの更新
{
	for (int i = 0; i < RANKING_MAX; ++i) // ランキングの数だけループ
    {
		if (newScore > ranking[i].score) // 新しいスコアが現在の順位のスコアより高いかをチェック
        { // ranking[i].score を比較
            // スコアを挿入し、それ以下を一つ下げる
            for (int j = RANKING_MAX - 1; j > i; --j)
            {
                ranking[j] = ranking[j - 1];
            }
            ranking[i].score = newScore; //  ranking[i].score に代入
			newRecordIndex = i;// 新記録の順位を格納
            break;
        }
    }
}