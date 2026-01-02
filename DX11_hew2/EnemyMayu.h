#pragma once
#include "Enemy_base.h"
#include "Texture2D.h"

class EnemyMayu : public Enemy_base
{
protected:
	Texture2D m_Texture2D;

	float stunTimer = 0.0f; //ターゲットを見失ったときのタイマー 


public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	DirectX::SimpleMath::Vector3 GetDirectionXVelocity() const {
		return m_direction * m_velocity;
	}


};