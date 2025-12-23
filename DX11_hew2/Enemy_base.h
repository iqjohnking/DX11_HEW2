#pragma once
#include "Character.h"

class Shrinemaiden; // 前方宣言

class Enemy_base :public Character{
protected:
	enum state {
		SPAWNING,	//出現中(アニメーション)
		ALIVE,		//生存
		DYING,		//消滅中(アニメーション)使わないかも、繭になるから
		DEAD		//消滅	(アニメーション)使わないかも、繭になるから
	};

	float m_Radius = 25.0f; // SetScale(50,50,0) なので半径 25 くらい
	float m_maxDist = 500.0f; // アクションパタンが変化する距離（調整用）

	//巫女
	Shrinemaiden* m_Miko = nullptr;

public:
	virtual void Init() override = 0;
	virtual void Update() override = 0;
	virtual void Draw(Camera* cam) override = 0;
	virtual void Uninit() override = 0;

	//巫女を追いかけるための関数
	virtual void move();

	//巫女の位置を取得
	void SetTarget(Shrinemaiden* sh) { m_Miko = sh; };
	//void SetTarget(Shrinemaiden& sh) { m_Miko = &sh; }; // 参照渡し版

	float GetRadius() const { return m_Radius; }


	//敵を追加する関数
	//void Add_Enemy();	　//こちはenemy追加の基底クラスとして使わない、もしenemyManegerクラスを作るならそっちに移動するかも

};