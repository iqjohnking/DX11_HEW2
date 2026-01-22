#pragma once
#include "EnemyBase.h"
#include "Texture2D.h"
#include <random>

class Enemy4 : public EnemyBase
{
protected:
	Texture2D m_Texture2D;
	//Collision::Sphere m_Collider; // 当たり判定の為の情報　

	float stunTimer = 0.0f; //ターゲットを見失ったときのタイマー 

	//フレームカウント
	int chargeTimer = 0;
	uint64_t chargeTiming = 2; //1秒間チャージ
	
	//ランダム
	float rand = 0.0f;

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	//巫女を追いかけるための関数
	void move() override;

	DirectX::SimpleMath::Vector3 GetDirectionXVelocity() const {
		return m_direction * m_velocity;
	}

	//min_valは2.0、max_valは4.0で使用
	uint64_t get_rand_range(uint64_t min_val, uint64_t max_val)
	{
		// min_val が max_val より大きい場合は入れ替え
		if (min_val > max_val) std::swap(min_val, max_val);

		// 乱数生成エンジン（初期化は一度だけ）
		static std::mt19937_64 mt64{ 0 };

		// [min_val, max_val] の範囲で整数の一様分布を生成
		std::uniform_int_distribution<uint64_t> dist(min_val, max_val);

		return dist(mt64);
	}

};