#pragma once
#include "Enemy_base.h"
#include "Texture2D.h"

class Enemy1 : public Enemy_base
{
protected:
	Texture2D m_Texture2D;
	//一番普通だから何もない
	float lostTargetTimer = 0.0f; //ターゲットを見失ったときのタイマー 

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	//巫女を追いかけるための関数
	void move() override;

};