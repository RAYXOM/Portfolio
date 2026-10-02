#pragma once
#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include <math.h>
#include <time.h>
#define MAX_ENEMYSHOT 2000// 敵のショットの最大数
#define MAX_PLAYERSHOT 50// 自機のショットの最大数


extern int scor;//スコアを格納する変数
extern int damagecount;//被弾回数を格納する変数
extern int attackcount;//攻撃回数を格納する変数

static const int SHIFT_NUM = 15;//座標を固定小数点数で表すためのシフト量

extern int GAME_TIME_LIMIT;// ゲームの制限時間を格納する変数
extern int startTime;// ゲーム開始時間を格納する変数

static const int SCREEN_WIDTH = 640;// 画面の幅
static const int SCREEN_HEIGHT = 480;// 画面の高さ

static const int ENEMY_RADIUS = 10;// 敵の半径

// ランキング用の配列（例：トップ5まで保存）
constexpr auto RANKING_MAX = 5;// ランキングの最大数

extern int enemyShotSE;//敵のショット時の効果音
extern int playerShotSE; //プレイヤーのショット時の効果音
extern int enemyDamageSE;//敵の被弾時の効果音
extern int playerDamageSE; //プレイヤーの被弾時の効果音
extern int selectSE;//選択時の効果音

extern int bgm_title;//タイトルシーンのBGM
extern int bgm_game;//ゲーム中のBGM
extern int bgm_result;//リザルトシーンのBGM

struct ENEMYSHOT { int x, y, speedX, speedY, radius; bool validFlag; };// 敵のショットの構造体
struct PLAYERSHOT { int x, y, speedX, speedY, radius; bool validFlag; };// 自機のショットの構造体

struct ENEMY { int x, y, counter, counter2; double angle; };// 敵の構造体
struct PLAYER { int x, y; double angle; };// 自機の構造体

__declspec(selectany)  ENEMY enemy;// 敵の構造体のインスタンス
__declspec(selectany)  ENEMYSHOT enemyShot[MAX_ENEMYSHOT];// 敵のショットの構造体の配列
__declspec(selectany)  PLAYER player;// 自機の構造体のインスタンス
__declspec(selectany)  PLAYERSHOT playerShot[MAX_PLAYERSHOT];// 自機のショットの構造体の配列

enum SCENE { TITLE, PLAY, EXIT, END };// ゲームのシーンを表す列挙型

SCENE TitleScene();
/******************************************************************************
 * @fn      TitleScene
 * @brief   タイトルシーン処理
 * @return  SCENE : 次のシーン
 ******************************************************************************/


SCENE GameScene();
/******************************************************************************
 * @fn      GameScene
 * @brief   ゲームシーン処理
 * @return  SCENE : 次のシーン
 ******************************************************************************/


SCENE ExitScene();
/******************************************************************************
 * @fn      ExitScene
 * @brief   終了シーン処理
 * @return  SCENE : 次のシーン
 ******************************************************************************/