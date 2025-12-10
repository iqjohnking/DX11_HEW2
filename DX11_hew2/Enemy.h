#pragma once

class Enemy {
protected:
	int enemy_x, enemy_y;
	int enemy_chase_x[61], enemy_chase_y[61];	//巫女を追いかけるための座標配列
	int enemy_work_x, enemy_work_y;	//敵の速度に合わせた座標移動をするための計算用変数

public:
	void Enemy_move();
};