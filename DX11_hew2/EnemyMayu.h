#pragma once
#include "Enemy_base.h"
#include "Texture2D.h"

class EnemyMayu : public Object
{
protected:
	Texture2D m_Texture2D;

	Collision::Sphere m_Collider; // 当たり判定の為の情報
	float m_Radius = 25.0f; // SetScale(50,50,0) なので半径 25 くらい
	bool isExploding = false;

	int spawnTimer = 0; //出現アニメーション用タイマー
	int explodeTimer = 0;


public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	float GetRadius() const { return m_Radius; }
	void SetRadius(float radius) { m_Radius = radius; }
};