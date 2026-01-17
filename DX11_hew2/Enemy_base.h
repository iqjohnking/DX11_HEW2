#pragma once
#include "Character.h"

class Shrinemaiden; // 前方宣言
class Field; // 前方宣言

class Enemy_base :public Character{
protected:
	enum state {
		SPAWNING,	//出現中(アニメーション)
		ALIVE,		//生存
		DYING,		//消滅中(アニメーション)使わないかも、繭になるから
		DEAD		//消滅	(アニメーション)使わないかも、繭になるから
	};

	Collision::Sphere m_Collider; // 当たり判定の為の情報
	float m_Radius = 25.0f; // SetScale(50,50,0) なので半径 25 くらい
	float m_maxDist = 500.0f; // アクションパタンが変化する距離（調整用）

	bool isSpdDown = false; //速度低下中かどうか
	bool isBecomingMayu = false; //繭になっているかどうか

	//巫女
	Shrinemaiden* m_Miko = nullptr;
	Field* m_Field = nullptr;	

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
	void SetField(Field* field) { m_Field = field; };


	float GetRadius() const { return m_Radius; }
	void SetRadius(float radius) { m_Radius = radius; }
	Collision::Sphere& GetCollider() { return m_Collider; }
	const Collision::Sphere& GetCollider() const { return m_Collider; }



	void SetIsSpdDown(bool isDown) { isSpdDown = isDown; };
	bool GetIsSpdDown() const { return isSpdDown; };

};