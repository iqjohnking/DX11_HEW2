#pragma once
#include "Character.h"

class Shrinemaiden; // 前方宣言
class Field; // 前方宣言

class Enemy_base :public Character{
protected:
	enum EnemyState {
		SPAWNING,	//0出現中(アニメーション)
		ALIVE,		//1生存
		ISMAYUING,	//2繭になっている最中
		DYING,		//3消滅中(アニメーション)使わないかも、繭になるから
		DEAD		//4消滅	(アニメーション)使わないかも、繭になるから
	};
	EnemyState state = EnemyState::SPAWNING;
	int spawnTimer = 0; //出現アニメーション用タイマー
	int mayuingTimer = 0; //繭になる用タイマー

	// hp = 12
	// 毒壁の攻撃力は12、6、4、（１回、２回、３回ダメージが受けたら死ぬ）
	// 三角囲むの攻撃力は12
	int m_Hitpoint = 3;

	Collision::Sphere m_Collider; // 当たり判定の為の情報
	float m_Radius = 25.0f; // SetScale(50,50,0) なので半径 25 くらい
	float m_maxDist = 500.0f; // アクションパタンが変化する距離（調整用）

	bool isSpdDown = false; //速度低下中かどうか
	bool isBecomingMayu = false; //繭になっているかどうか

	DirectX::SimpleMath::Vector3 m_StartMayuPos = DirectX::SimpleMath::Vector3::Zero; // 起
	DirectX::SimpleMath::Vector3 m_TargetMayuPos = DirectX::SimpleMath::Vector3::Zero; // 迄

	//巫女//field
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
	void SetField(Field* field) { m_Field = field; };
	//半径の取得・設定
	float GetRadius() const { return m_Radius; }
	void SetRadius(float radius) { m_Radius = radius; }
	//当たり判定情報の取得
	Collision::Sphere& GetCollider() { return m_Collider; }
	const Collision::Sphere& GetCollider() const { return m_Collider; }
	//体力の取得・設定
	void SetIsSpdDown(bool isDown) { isSpdDown = isDown; };
	bool GetIsSpdDown() const { return isSpdDown; };
	//状態の取得
	int GetState() const { return static_cast<int>(state); }

	void StartMayuing(const Vector3& mayuPos)
	{
		m_StartMayuPos = GetPosition();
		m_TargetMayuPos = mayuPos;
		mayuingTimer = 60;                 // 1秒 = 60frame（假設60fps）
		state = EnemyState::ISMAYUING;
	}

};