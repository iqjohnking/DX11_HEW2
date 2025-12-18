#pragma once
#include "Texture2D.h"

class Shrinemaiden;

class Enemy_base :public Texture2D 
{
protected:
	//敵の速度
	float enemy_speed;

	//敵の現在の座標
	DirectX::SimpleMath::Vector3 enemy_pos;

	//巫女を追いかけるための座標配列
	DirectX::SimpleMath::Vector3 enemy_chase[61];

	//敵の速度に合わせた座標移動をするための計算用変数
	DirectX::SimpleMath::Vector3 enemy_pos_work;

	//生きているかのフラグ
	bool alive_flg_enemy;

	//巫女
	Shrinemaiden* m_Miko = nullptr;

public:
	virtual void Init();
	virtual void Update();

	//巫女を追いかけるための関数
	void Enemy_move();

	//敵を追加する関数
	void Add_Enemy();	

	//巫女の位置を取得
	void SetTarget(Shrinemaiden* sh) { m_Miko = sh; };
};