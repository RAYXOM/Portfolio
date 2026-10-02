#include "DxLib.h"
#include "CheckKey.h"
#include "windef.h"
#include "Game Common.h"
#include "Shot.h"
#include "Init.h"
#include <math.h>
#include <time.h>

// ★追加
extern int playerShotSE;


/******************************************************************************
 * @fn      ShotAdd2
 * @brief   自機ショット追加
 ******************************************************************************/
void ShotAdd2(int x, int y, int radius, double angle, double speed)//!< 自機ショットの追加関数
{
	int i;

	// 使われていないショットを検索
	for (i = 0; i < MAX_PLAYERSHOT; i++)
	{
		if (playerShot[i].validFlag == false)
		{
			break;
		}
	}
	if (i == MAX_PLAYERSHOT)// 使われていないショットがなかった場合は何もしない
	{
		return;
	}

	// 新しいショットのデータを初期化
	{
		// 座標セット
		playerShot[i].x = x;
		playerShot[i].y = y;

		// サイズセット
		playerShot[i].radius = radius;

		// 飛んで行く方向とスピードから、Ｘ軸方向への移動速度とＹ軸方向への移動速度を得る
		playerShot[i].speedX = (int)(cos(angle) * speed);
		playerShot[i].speedY = (int)(sin(angle) * speed);

		// ショットデータの有効フラグを立てる
		playerShot[i].validFlag = true;
	}
}

/******************************************************************************
 * @fn      PlayerShot
 * @brief   自機ショット処理
 ******************************************************************************/
void PlayerShot(void)//!< 自機ショット関数
{
	double angle = atan2((double)(enemy.y - player.y), (double)(enemy.x - player.x));//自機から見た敵への方向を取得
	double d = angle - (DX_PI - 4.4);
	ShotAdd2(player.x, player.y, 4, angle, 2 << SHIFT_NUM);//自機から見た敵の方向にショット
	// ★効果音再生（最重要）
	PlaySoundMem(playerShotSE, DX_PLAYTYPE_BACK);
}

/******************************************************************************
 * @fn      ShotAdd
 * @brief   敵ショット追加
 ******************************************************************************/
void ShotAdd(int x, int y, int radius, double angle, double speed)//!< 敵のショットの追加関数
{
	int i;

	// 使われていないショットを検索
	for (i = 0; i < MAX_ENEMYSHOT; i++)
	{
		if (enemyShot[i].validFlag == false)
		{
			break;
		}
	}
	if (i == MAX_ENEMYSHOT)// 使われていないショットがなかった場合は何もしない
	{
		return;
	}

	// 新しいショットのデータを初期化
	{
		// 座標セット
		enemyShot[i].x = x;
		enemyShot[i].y = y;

		// サイズセット
		enemyShot[i].radius = radius;

		// 飛んで行く方向とスピードから、Ｘ軸方向への移動速度とＹ軸方向への移動速度を得る
		enemyShot[i].speedX = (int)(cos(angle) * speed);
		enemyShot[i].speedY = (int)(sin(angle) * speed);

		// ショットデータの有効フラグを立てる
		enemyShot[i].validFlag = true;
	}
	static int lastPlay = 0;
	if (GetNowCount() - lastPlay > 100) {
		PlaySoundMem(enemyShotSE, DX_PLAYTYPE_BACK);
		lastPlay = GetNowCount();
	}
}

/******************************************************************************
 * @fn      ShotType1
 * @brief   敵ショットパターン1
 ******************************************************************************/
void ShotType1(void)//!< 敵のショットパターン１関数:3WAYショット
{
	// カウンタが４０になったらショットする
	if (enemy.counter == 40)
	{
		double angle, d;

		// 敵から見た自機への方向を取得
		angle = atan2((double)(player.y - enemy.y), (double)(player.x - enemy.x));

		// ３ＷＡＹショット処理
		d = angle - (DX_PI / 3) / 2;
		for (int i = 0; i < 3; i++)
		{
			ShotAdd(enemy.x, enemy.y, 4, d, 2 << SHIFT_NUM);
			d += (DX_PI / 3) / 2;
		}

		// カウンタを初期化
		enemy.counter = 0;
	}
	else
	{
		// カウンタをインクリメント
		enemy.counter++;
	}
}

/******************************************************************************
 * @fn      ShotType2
 * @brief   敵ショットパターン2
 ******************************************************************************/
void ShotType2(void)//!< 敵のショットパターン２関数:全方向ショット
{
	// カウンタが１０になったらショットする
	if (enemy.counter == 10)
	{
		double angle, d;

		// 敵から見た自機への方向を取得
		angle = atan2((double)(player.y - enemy.y), (double)(player.x - enemy.x));

		// ワイドショット処理、少しずつ角度を変えて一周分ショット
		d = angle - DX_PI;
		for (int i = 0; i < 20; i++)
		{
			ShotAdd(enemy.x, enemy.y, 4, d, 4 << SHIFT_NUM);
			d += 2 * DX_PI / 20;
		}

		// カウンタを初期化
		enemy.counter = 0;
	}
	else
	{
		// カウンタをインクリメント
		enemy.counter++;
	}
}

/******************************************************************************
 * @fn      ShotType3
 * @brief   敵ショットパターン3
 ******************************************************************************/
void ShotType3(void)//!< 敵のショットパターン３関数:一点集中時間差ショット
{
	// カウンタ２の値によって処理を分岐
	switch (enemy.counter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		enemy.counter++;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (enemy.counter > 60)
		{
			enemy.counter2++;
			enemy.counter = 0;

			// このときの自機への方向を保存
			enemy.angle = atan2((double)(player.y - enemy.y), (double)(player.x - enemy.x));
		}
		break;

	case 1:
		// ショット処理

		// カウンタが５の倍数の時のみショットする
		if (enemy.counter % 5 == 0)
		{
			ShotAdd(enemy.x, enemy.y, 4, enemy.angle, (1 << SHIFT_NUM) + (enemy.counter << SHIFT_NUM) / 15);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
		enemy.counter++;
		if (enemy.counter == 50)
		{
			enemy.counter2 = 0;
			enemy.counter = 0;
		}
		break;
	}
}

/******************************************************************************
 * @fn      ShotType4
 * @brief   敵ショットパターン4
 ******************************************************************************/
void ShotType4(void)//!< 敵のショットパターン４関数:ばら撒きショット
{
	// カウンタ２の値によって処理を分岐
	switch (enemy.counter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		enemy.counter++;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (enemy.counter > 60)
		{
			enemy.counter2++;
			enemy.counter = 0;

			// このときの自機への方向を保存
			enemy.angle = atan2((double)(player.y - enemy.y), (double)(player.x - enemy.x));
		}
		break;

	case 1:
		//ショット処理

		// カウンタが２の倍数の時のみショットする
		if (enemy.counter % 2 == 0)
		{
			double angle;

			// 飛ばす方向にぶれをつける
			angle = enemy.angle + (DX_PI / 3600 * (GetRand(800) - 400));

			// ショット
			ShotAdd(enemy.x, enemy.y, 5, angle, 3 << SHIFT_NUM);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
		enemy.counter++;
		if (enemy.counter == 50)
		{
			enemy.counter2 = 0;
			enemy.counter = 0;
		}
		break;
	}
}

/******************************************************************************
 * @fn      ShotType5
 * @brief   敵ショットパターン5
 ******************************************************************************/
void ShotType5(void)//!< 敵のショットパターン５関数:回転ショットz
{
	// カウンタが２になったらショット処理を行う
	if (enemy.counter == 2)
	{
		// ショットの方向を変更
		enemy.angle += 0.2;

		// 速度の違う五つのショットを発射
		for (int i = 0; i < 5; i++)
		{
			ShotAdd(enemy.x, enemy.y, 6, enemy.angle, (3 << SHIFT_NUM) + (i * 8000));
		}

		// カウンタを初期化する
		enemy.counter = 0;
	}
	else
	{
		enemy.counter++;
	}
}

/******************************************************************************
 * @fn      CheckHitPointCircle
 * @brief   点と円の当たり判定
 ******************************************************************************/
bool CheckHitPointCircle(POINT& point, int circleX, int circleY, int circleRadius)// 点と円の当たり判定関数
{
	// 点と円の中心との距離が円の半径より小さい場合
	return pow(point.x - circleX, 2) + pow(point.y - circleY, 2) <= pow(circleRadius, 2);
}

/******************************************************************************
 * @fn      CheckHitRectCircle
 * @brief   矩形と円の当たり判定
 ******************************************************************************/
bool CheckHitRectCircle(RECT& rect, int circleX, int circleY, int circleRadius)// 矩形と円の当たり判定関数
{
	if (rect.left - circleRadius < circleX &&
		rect.right + circleRadius > circleX &&
		rect.top < circleY &&
		rect.bottom > circleY)// 矩形の内側に円の中心がある場合
	{
		return true;
	}
	if (rect.left < circleX &&
		rect.right > circleX &&
		rect.top - circleRadius < circleY &&
		rect.bottom + circleRadius > circleY)// 矩形の内側に円の中心がある場合
	{
		return true;
	}
	if (pow(rect.left - circleX, 2) + pow(rect.top - circleY, 2) <
		pow(circleRadius, 2))// 矩形の左上の角と円の中心との距離が円の半径より小さい場合
	{
		return true;
	}
	if (pow(rect.right - circleX, 2) + pow(rect.top - circleY, 2) <
		pow(circleRadius, 2))// 矩形の右上の角と円の中心との距離が円の半径より小さい場合
	{
		return true;
	}
	if (pow(rect.left - circleX, 2) + pow(rect.bottom - circleY, 2) <
		pow(circleRadius, 2))// 矩形の左下の角と円の中心との距離が円の半径より小さい場合
	{
		return true;
	}
	if (pow(rect.right - circleX, 2) + pow(rect.bottom - circleY, 2) <
		pow(circleRadius, 2))// 矩形の右下の角と円の中心との距離が円の半径より小さい場合
	{
		return true;
	}
}