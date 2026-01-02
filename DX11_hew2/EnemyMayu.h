#pragma once
#include "Enemy_base.h"
#include "Texture2D.h"

class EnemyMayu : public Object
{
protected:
	Texture2D m_Texture2D;

	Collision::Sphere m_Collider; // “–‚½‚è”»’è‚Ìˆ×‚Ìî•ñ
	float m_Radius = 25.0f; // SetScale(50,50,0) ‚È‚Ì‚Å”¼Œa 25 ‚­‚ç‚¢


public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	float GetRadius() const { return m_Radius; }
	void SetRadius(float radius) { m_Radius = radius; }
};