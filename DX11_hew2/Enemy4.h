#pragma once
#include "Enemy_base.h"
#include "Texture2D.h"
#include <random>

class Enemy4 : public Enemy_base
{
protected:
	Texture2D m_Texture2D;
	//Collision::Sphere m_Collider; // 当たり判定の為の情報　

	float stunTimer = 0.0f; //ターゲットを見失ったときのタイマー 

	//フレームカウント
	int frameCount = 0;
	
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
	uint64_t get_rand_range(uint64_t min_val, uint64_t max_val) {
		// 乱数生成器
		static std::mt19937_64 mt64(0);

		// [min_val, max_val] の一様分布整数 (double) の分布生成器
		std::uniform_real_distribution<double> get_rand_uni_real(2.0, 4.0);

		// 乱数を生成
		return get_rand_uni_real(mt64);
	}

};