#include "CheckKey.h"
#include "DxLib.h"

namespace CheckKey
{
	int downBuffer[256];	// CheckDownKey用のキーバッファ
	int upBuffer[256];		// CheckUpKey用のキーバッファ
};

/******************************************************************************
 * @fn      CheckDownKey
 * @brief   キー押下瞬間検出
 * @param   KeyCode : キーコード
 * @return  int : 押された瞬間なら1
 * @detail  指定キーが押された瞬間のみ検出する
 ******************************************************************************/
int CheckDownKey(int KeyCode)// 指定されたキーが押された瞬間だけ 1 を返す関数
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(KeyCode);

	// 前回キーが押されておらず、かつ、現在キーが押されていたら「キーを押した瞬間」とする
	if (CheckKey::downBuffer[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	// 現在のキー状態をバッファに格納
	CheckKey::downBuffer[KeyCode] = keyState;

	return result;
}

/******************************************************************************
 * @fn      CheckUpKey
 * @brief   キー離し瞬間検出
 * @param   KeyCode : キーコード
 * @return  int : 離された瞬間なら1
 * @detail  指定キーが離された瞬間のみ検出する
 ******************************************************************************/
int CheckUpKey(int KeyCode)// 指定されたキーが離された瞬間だけ 1 を返す関数
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(KeyCode);

	// 前回キーが押されており、かつ、現在キーが押されていなかったら「キーを離した瞬間」とする
	if (CheckKey::upBuffer[KeyCode] == 1 && keyState == 0)
	{
		result = 1;
	}

	// 現在のキー状態をバッファに格納
	CheckKey::upBuffer[KeyCode] = keyState;

	return result;
}
