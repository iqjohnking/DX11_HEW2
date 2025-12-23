#pragma once
#include "Texture2D.h"

class Shrinemaiden;

class Enemy_base :public Texture2D 
{
protected:
	//敵の速度
	float m_speed = 0.7f;	//巫女の1/3の速度
	float m_maxDist = 500.0f;
	float m_minSpeed = 1.00f;
	float m_maxSpeed = 2.00f;

	//生きているかのフラグ
	bool alive_flg_enemy;

	enum state {
		SPAWNING,	//出現中 (アニメーション)
		ALIVE,		//生存
		DYING,		//消滅中 (アニメーション)使わないかも、繭になるから
		DEAD		//消滅
	};

	//物理用変数
	DirectX::SimpleMath::Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	float m_Radius = 25.0f; // SetScale(50,50,0) なので半径 25 くらい

	//巫女
	Shrinemaiden* m_Miko = nullptr;

public:
	virtual void Init();
	virtual void Update();

	//巫女を追いかけるための関数
	virtual void move();

	//巫女の位置を取得
	void SetTarget(Shrinemaiden* sh) { m_Miko = sh; };
	//void SetTarget(Shrinemaiden& sh) { m_Miko = &sh; }; // 参照渡し版

	const DirectX::SimpleMath::Vector3& GetVelocity() const { return m_Velocity; }
	void SetVelocity(const DirectX::SimpleMath::Vector3& v) { m_Velocity = v; }

	float GetRadius() const { return m_Radius; }


	//敵を追加する関数
	//void Add_Enemy();	　//こちはenemy追加の基底クラスとして使わない、もしenemyManegerクラスを作るならそっちに移動するかも

};