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
 * @fn      GameScene
 * @brief   ゲームシーン処理
 * @return  SCENE : 次のシーン
 * @detail  制限時間の終了に応じて次のシーンに遷移する
 *          入力に応じてゲームを終了する
 ******************************************************************************/
SCENE GameScene()// ゲームシーン関数
{
	int ran = 0;//敵の行動の切り替えの時間をランダムにするための変数
	int ans = 0;//敵の行動の切り替えの時間を格納する変数
	int timex = GetNowCount();//敵の行動の切り替えの時間を格納する変数
	int timey = GetNowCount();//ゲームの経過時間を格納する変数
	int timez = GetNowCount();//敵のショットの切り替えの時間を格納する変数
	int mousex, mousey;//マウスの座標を格納する変数
	int remain = GAME_TIME_LIMIT;
	static int prevMouseInput = 0;

	// 敵のショット処理関数の数をセット
	int enemyShredNum = sizeof(enemyShred) / sizeof(enemyShred[0]);
	int shotMode = 0;//敵のショット処理関数の切り替えを管理する変数
	int shotMode2 = 0;//敵のショット処理関数の切り替えを管理する変数

	if (startTime == 0)
	{

		int playerShredNum = sizeof(playerShred) / sizeof(playerShred[0]);//プレイヤーのショット処理関数の数をセット

	Init();//初期化

	// 敵のショットタイプを初期化


	}

	static bool isFirst = true;// ゲームシーンが初めて呼ばれたかどうかを判定するフラグ

	if (isFirst)// ゲームシーンが初めて呼ばれた場合
	{
		StopSoundMem(bgm_title);//タイトルシーンのBGMを停止
		StopSoundMem(bgm_result);//リザルトシーンのBGMを停止

		PlaySoundMem(bgm_game, DX_PLAYTYPE_LOOP);//ゲームシーンのBGMを再生
		isFirst = false;// ゲームシーンが初めて呼ばれたことを記録
	}


	// ゲームループ開始　エスケープキーが押されたら終了する
	while (ProcessMessage() == 0)
	{
		// 現在のカウントを取得する
		int time = GetNowCount();

		if (CheckHitKey(KEY_INPUT_ESCAPE))//エスケープキーが押されたら
		{
			DxLib_End(); // DxLibの終了処理
			exit(0);     // プログラム終了
		}

		// ゲームループ内（whileの中）で残り時間を計算
		int elapsed = (GetNowCount() - startTime) / 1000; // 経過秒数
		 remain = GAME_TIME_LIMIT - elapsed;
		if (remain < 0) remain = 0;

		ran = GetRand(3);//敵の行動の切り替えの時間をランダムにする
		switch (ran)
		{
		case 0:
			ans = 2000;//2秒
			break;
		case 1:
			ans = 3000;//3秒
			break;
		case 2:
			ans = 4000;//4秒
			break;
		}

		// 画面の初期化
		ClearDrawScreen();

		if (scor < 0)//スコアがマイナスになったら0にする
		{
			scor = 0;
		}

		DrawFormatString(30, 30, GetColor(0, 255, 0), _T("SCORE: %d"), scor);//現在のスコアを表示

		if (damagecount == 0)//一度も被弾していなかったら 
		{
			DrawFormatString(90, 50, GetColor(255, 255, 0), _T("No Hit!!"));//被弾していないことを表示
		}

		DrawFormatString(30, 70, GetColor(255, 0, 0), _T("TIME RIMIT: %d"), remain);// 現在の残り時間を表示
		DrawFormatString(530, 460, GetColor(255, 0, 0), _T("Esc:途中終了"));

		// 自機の処理
		{

			// マウスの左クリックでショット
			int mouseInput = GetMouseInput();
			if ((mouseInput & MOUSE_INPUT_LEFT) && !(prevMouseInput & MOUSE_INPUT_LEFT))
			{
				PlayerShot();
				attackcount++;
			}
			prevMouseInput = mouseInput;

			// 自機移動
			// マウス入力で自機を移動
			GetMousePoint(&mousex, &mousey);
			// 画面外に出ないようにクリッピング
			if (mousex < 10) mousex = 10;
			if (mousex > 630) mousex = 630;
			if (mousey < 10) mousey = 10;
			if (mousey > 470) mousey = 470;
			player.x = mousex << SHIFT_NUM;
			player.y = mousey << SHIFT_NUM;

			// 自機の描画
			int x = player.x >> SHIFT_NUM;
			int y = player.y >> SHIFT_NUM;
			DrawBox(x - 5, y - 5, x + 5, y + 5, GetColor(255, 255, 0), false);
		}

		// 弾の処理
		{
			// 弾の数だけ移動処理を繰り返す
			for (int i = 0; i < MAX_ENEMYSHOT; i++)
			{
				// 弾のデータが有効な場合は処理
				if (enemyShot[i].validFlag == false)
				{
					continue;
				}

				// 点と円の当たり判定
				POINT point;
				point.x = player.x >> SHIFT_NUM;
				point.y = player.y >> SHIFT_NUM;
				int x1 = enemyShot[i].x >> SHIFT_NUM;
				int y1 = enemyShot[i].y >> SHIFT_NUM;
				int r1 = enemyShot[i].radius;
				if (CheckHitPointCircle(point, x1, y1, r1) == true)//プレイヤーが被弾した時の処理
				{
					enemyShot[i].validFlag = false;
					scor = scor - 10;
					PlaySoundMem(playerDamageSE, DX_PLAYTYPE_BACK);//プレイヤーが被弾した時の効果音を再生
					damagecount++;
					continue;
				}

				// 矩形と円の当たり判定
				RECT rect;
				rect.left = (player.x >> SHIFT_NUM) - 5;
				rect.top = (player.y >> SHIFT_NUM) - 5;
				rect.right = (player.x >> SHIFT_NUM) + 5;
				rect.bottom = (player.y >> SHIFT_NUM) + 5;
				int x2 = enemyShot[i].x >> SHIFT_NUM;
				int y2 = enemyShot[i].y >> SHIFT_NUM;
				int r2 = enemyShot[i].radius;
				if (CheckHitRectCircle(rect, x2, y2, r2) == true)//プレイヤーが被弾した時の処理
				{
					enemyShot[i].validFlag = false;
					scor = scor - 10;
					PlaySoundMem(playerDamageSE, DX_PLAYTYPE_BACK);//プレイヤーが被弾した時の効果音を再生
					damagecount++;
					continue;
				}

				// 画面外に出たら弾情報を消去する
				if (((enemyShot[i].x >> SHIFT_NUM) < -20) || ((enemyShot[i].x >> SHIFT_NUM) > 660)
					|| ((enemyShot[i].y >> SHIFT_NUM) < -20) || ((enemyShot[i].y >> SHIFT_NUM) > 500))
				{
					// データの有効フラグを倒す
					enemyShot[i].validFlag = false;
					continue;
				}

				// 移動処理
				enemyShot[i].x += enemyShot[i].speedX;
				enemyShot[i].y += enemyShot[i].speedY;

				// 弾の描画
				int x = enemyShot[i].x >> SHIFT_NUM;
				int y = enemyShot[i].y >> SHIFT_NUM;
				DrawCircle(x, y, enemyShot[i].radius, GetColor(255, 255, 255), false);
			}
		}

		// 弾の処理
		{
			// 弾の数だけ移動処理を繰り返す
			for (int i = 0; i < MAX_PLAYERSHOT; i++)
			{
				// 弾のデータが有効な場合は処理
				if (playerShot[i].validFlag == false)
				{
					continue;
				}


				// 点と円の当たり判定
				POINT point;
				point.x = enemy.x >> SHIFT_NUM;
				point.y = enemy.y >> SHIFT_NUM;
				int x1 = playerShot[i].x >> SHIFT_NUM;
				int y1 = playerShot[i].y >> SHIFT_NUM;
				int r1 = playerShot[i].radius;
				if (CheckHitPointCircle(point, x1, y1, r1) == true)//敵が被弾した時の処理
				{
					playerShot[i].validFlag = false;
					scor = scor + 20;
					PlaySoundMem(enemyDamageSE, DX_PLAYTYPE_BACK);//敵が被弾した時の効果音を再生
					continue;
				}

				// 矩形と円の当たり判定
				RECT rect;
				rect.left = (enemy.x >> SHIFT_NUM) - 5;
				rect.top = (enemy.y >> SHIFT_NUM) - 5;
				rect.right = (enemy.x >> SHIFT_NUM) + 5;
				rect.bottom = (enemy.y >> SHIFT_NUM) + 5;
				int x2 = playerShot[i].x >> SHIFT_NUM;
				int y2 = playerShot[i].y >> SHIFT_NUM;
				int r2 = playerShot[i].radius;
				if (CheckHitRectCircle(rect, x2, y2, r2) == true)//敵が被弾した時の処理
				{
					playerShot[i].validFlag = false;
					scor = scor + 20;
					PlaySoundMem(enemyDamageSE, DX_PLAYTYPE_BACK);//敵が被弾した時の効果音を再生
					continue;
				}

				// 画面外に出たら弾情報を消去する
				if (((playerShot[i].x >> SHIFT_NUM) < -20) || ((playerShot[i].x >> SHIFT_NUM) > 660)
					|| ((playerShot[i].y >> SHIFT_NUM) < -20) || ((playerShot[i].y >> SHIFT_NUM) > 500))
				{
					// データの有効フラグを倒す
					playerShot[i].validFlag = false;
					continue;
				}

				// 移動処理
				playerShot[i].x += playerShot[i].speedX;
				playerShot[i].y += playerShot[i].speedY;

				// 弾の描画
				int x = playerShot[i].x >> SHIFT_NUM;
				int y = playerShot[i].y >> SHIFT_NUM;
				DrawCircle(x, y, playerShot[i].radius, GetColor(255, 255, 0), false);
			}
		}

		// 敵の処理
		{
			// 敵のショット処理
			enemyShred[shotMode]();

			// 敵の移動
			if (time - timex > ans)//設定された時間が経過する度に敵の移動を切り替える
			{
				/*timex = time;*/
				int m = GetRand(4);//敵の移動方向をランダムにする
				int a = GetRand(3);//敵の移動距離をランダムにする
				int b = 0;//敵の移動回数を格納する変数
				int c = 0;//敵の移動距離を格納する変数

				switch (a)//移動の回数をランダムにする
				{
				case 0:
					b = 30;//移動回数を30回にする
				case 1:
					b = 40;//移動回数を40回にする
				case 2:
					b = 50;//移動回数を50回にする
				}

				if (m == 0)
				{
					while (c <= b && ((enemy.x >> SHIFT_NUM) - ENEMY_RADIUS) > 0)//乱数で決められた回数敵を移動させる
					{
						enemy.x -= 2 << SHIFT_NUM;//左に移動
						++c;
					}
					c = 0;
				}
				if (m == 1)
				{
					while (c <= b && ((enemy.x >> SHIFT_NUM) + ENEMY_RADIUS) < SCREEN_WIDTH)//乱数で決められた回数敵を移動させる
					{
						enemy.x += 2 << SHIFT_NUM;//右に移動
						++c;
					}
					c = 0;
				}
				if (m == 2)
				{
					while (c <= b && ((enemy.y >> SHIFT_NUM) - ENEMY_RADIUS) > 0)//乱数で決められた回数敵を移動させる
					{
						enemy.y -= 2 << SHIFT_NUM;//下に移動
						++c;
					}
					c = 0;
				}
				if (m == 3)
				{
					while (c <= b && ((enemy.y >> SHIFT_NUM) + ENEMY_RADIUS) < SCREEN_HEIGHT)//乱数で決められた回数敵を移動させる
					{
						enemy.y += 2 << SHIFT_NUM;//上に移動
						++c;
					}
					c = 0;
				}
			}

			// 敵の描画
			int x = enemy.x >> SHIFT_NUM;
			int y = enemy.y >> SHIFT_NUM;
			DrawCircle(x, y, 10, GetColor(255, 255, 255), false);
		}


		// 敵のショットの切り替え処理
		{
			if (time - timex > ans)//設定された時間が経過する度に敵のショットを切り替える
			{
				timex = time;
				int r = GetRand(5);//敵のショットの切り替えをランダムにする
				switch (r)
				{
				case 0:
					enemy.counter2 = 0;
					enemy.counter = 0;
					enemy.angle = 0.0;

					shotMode = 0;//敵のショット切り替え:3WAYショット

					break;
				case 1:
					enemy.counter2 = 0;
					enemy.counter = 0;
					enemy.angle = 0.0;

					shotMode = 1;//敵のショット切り替え:全方向ショット

					break;
				case 2:
					enemy.counter2 = 0;
					enemy.counter = 0;
					enemy.angle = 0.0;

					shotMode = 2;//敵のショット切り替え:一点集中時間差ショット

					break;
				case 3:
					enemy.counter2 = 0;
					enemy.counter = 0;
					enemy.angle = 0.0;

					shotMode = 3;//敵のショット切り替え:ばら撒きショット

					break;
				case 4:
					enemy.counter2 = 0;
					enemy.counter = 0;
					enemy.angle = 0.0;

					shotMode = 4;//敵のショット切り替え:回転ショット

					break;
				}
				if (shotMode == enemyShredNum)//敵のショットの切り替えが敵のショット処理関数の数と同じになったら
				{
					shotMode = 0;//敵のショット切り替え:3WAYショット
				}
			}
		}

		// 画面の更新
		ScreenFlip();

		// １７ミリ秒(約秒間６０フレームだった時の１フレームあたりの経過時間)
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
		}

		/*if (CheckHitKey(KEY_INPUT_Z))*/
		if (time - timey > GAME_TIME_LIMIT * 1000)//ゲーム時間が経過したら
		{
			if (damagecount == 0)//一度も被弾していなかったら
			{
				scor = scor + attackcount;//攻撃回数分スコアを加算する
			}
			UpdateRanking(scor);//ランキングの更新
			SaveHighScore();//スコアの保存
			isFirst = true;
			StopSoundMem(bgm_game);
			return EXIT;//リザルト画面へ移動する
		}
	}
}