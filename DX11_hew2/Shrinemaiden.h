#pragma once
#include "Texture2D.h"	//基底クラス

class Shrinemaiden :public Texture2D
{
private:
	//敵から逃げる速度
	float run_speed = 2.0f;

	//範囲内に敵がいなくなったら減速する速度
	float deceleration_speed = 0.2f;

	//この速度以下になると停止する
	float stop_speed = 0.1f;

	//巫女の位置
	DirectX::SimpleMath::Vector3 shrinemaiden_pos;

	//巫女の前の位置
	DirectX::SimpleMath::Vector3 old_shrinemaiden_pos;

	//敵の位置に合わせた座標移動をするための計算用変数
	float shrinemaiden_work;	

	//生きているかのフラグ
	bool alive_flg_Shrinemaiden;

	//敵の判定を取る円の半径
	float range = 100.0f;

	//ステージの中心
	DirectX::SimpleMath::Vector3 stage_center;
	//ステージの半径
	float stage_radius = 500.0f;

	//方向を取ってそこにspeedをかける
public:
	void Init() override;
	void Update() override;

	void Shrinemaiden_move();

	//巫女の座標を取るゲッター
	DirectX::SimpleMath::Vector3 Get_Shrinemaiden_pos() const { return shrinemaiden_pos; }

	float get_run_speed() const { return run_speed; }

	//糸の位置参照する関数 (hitcheck)
	//敵の位置参照する関数 (敵の位置-巫女の位置)
	//ステージの位置参照する関数　(中心点-巫女の位置)
};

