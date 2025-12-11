#include "Enemy.h"

//フレームカウントを用意して、敵の速度に合わせてこの関数を実行するのもありかも
//敵の速度が巫女の1/3なら、3フレームに1回実行する…など (←だと敵の動きがカクカクになりそう？)

void Enemy::Enemy_move()
{	
	for (int i = 60; i > 0; i--)  //座標配列の値の移動
	{
		enemy_chase[i] = enemy_chase[i - 1];
	}	

	//巫女の位置を敵の座標配列に代入
	//↓巫女のクラスを作って巫女の座標を参照できるようになったらコメント外す
	//enemy_chase[0] = player_pos;

	//↓速度が巫女の1/3の敵なら	
	//enemy_pos_work = enemy_chase[60] - enemy_chase[59];
	//enemy_pos_work = enemy_pos_work / 3;
	//enemy_pos += enemy_pos_work;
	//移動量算出→移動量を1/3に→現在の敵座標に移動量を足して移動完了、ということをしている

	//↓巫女と同じ速度の敵の場合の移動
	//現在の敵の座標を更新
	enemy_pos = enemy_chase[60];
}

void Enemy::Add_Enemy()
{
	//今のところなし
}