#include "Enemy.h"

//フレームカウントを用意して、敵の速度に合わせてこの関数を実行するのもありかも
//敵の速度が巫女の1/3なら、3フレームに1回実行する…など (←だと敵の動きがカクカクになりそう？)

void Enemy::Enemy_move()
{	
	for (int i = 60; i > 0; i--)  //x座標の値の移動
	{
		enemy_chase_x[i] = enemy_chase_x[i - 1];
	}
	for (int j = 60; j > 0; j--)  //y座標の値の移動
	{
		enemy_chase_y[j] = enemy_chase_y[j - 1];
	}

	//巫女の位置を敵の座標配列に代入
	//↓巫女のクラスを作って巫女の座標を参照できるようになったらコメント外す
	//enemy_chase_x[0] = player_x;
	//enemy_chase_y[0] = player_y;

	//↓速度が巫女の1/3の敵なら	
	//enemy_work_x = enemy_chase_x[60] - enemy_chase_x[59];
	//enemy_work_y = enemy_chase_y[60] - enemy_chase_y[59];
	//enemy_work_x = enemy_work_x / 3;
	//enemy_work_y = enemy_work_y / 3;
	//enemy_x += enemy_work_x;
	//enemy_y += enemy_work_y;
	//移動量算出→移動量を1/3に→現在の敵座標に移動量を足して移動完了、ということをしている

	//↓巫女と同じ速度の敵の場合の移動
	//現在の敵の座標を更新
	enemy_x = enemy_chase_x[60];
	enemy_y = enemy_chase_y[60];	
}