#include "Game Common.h"

int GAME_TIME_LIMIT = 100;// ゲームの制限時間を格納する変数

int scor;//スコアを格納する変数
int damagecount;//被弾回数を格納する変数
int attackcount;//攻撃回数を格納する変数

int startTime = 0;// ゲーム開始時間を格納する変数

int se_shot;// 効果音のハンドルを格納する変数

//各サウンドを格納する変数
int enemyShotSE = -1;//敵のショット時の効果音
int playerShotSE = -1;//プレイヤーのショット時の効果音
int enemyDamageSE = -1;//敵の被弾時の効果音
int playerDamageSE = -1;//プレイヤーの被弾時の効果音
int selectSE = -1;//選択時の効果音

int bgm_title = -1;//タイトルシーンのBGM
int bgm_game = -1;//ゲーム中のBGM
int bgm_result = -1;//リザルトシーンのBGM

