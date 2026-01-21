#pragma once
#include "EnemyBase.h"
#include "Texture2D.h"

class silkWall;

class Enemy2 : public EnemyBase
{
protected:
	Texture2D m_Texture2D;
	//Collision::Sphere m_Collider; // 当たり判定の為の情報　
	
	float m_cutTimer = 0.0f;       // 停止（切断）タイマー
	silkWall* m_targetWall = nullptr; // 今切っている糸
	
	float stunTimer = 0.0f; //ターゲットを見失ったときのタイマー 


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


};