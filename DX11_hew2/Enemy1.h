#pragma once
#include "Enemy_base.h"

class Enemy1 :public Enemy_base
{
protected:
	//敵の速度
	float enemy_speed = 0.7f;	//巫女の1/3の速度

	//敵の現在の座標
	DirectX::SimpleMath::Vector3 enemy_pos;

	//巫女を追いかけるための座標配列
	DirectX::SimpleMath::Vector3 enemy_chase[61];

	//敵の速度に合わせた座標移動をするための計算用変数
	DirectX::SimpleMath::Vector3 enemy_pos_work;

	//生きているかのフラグ
	bool alive_flg_enemy;

public:
	void Init() override;
	void Update() override;

	//巫女を追いかけるための関数
	void Enemy_move();

	//敵を追加する関数
	void Add_Enemy();
};