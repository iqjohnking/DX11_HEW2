#pragma once
#include "Character.h"
#include "Texture2D.h"	//基底クラス

class Field; // 前方宣言




class Shrinemaiden :public Character
{
protected:
	enum state {
		SPAWNING,	//出現中(アニメーション)
		ALIVE,		//生存
		DYING,		//消滅中(アニメーション)使わないかも、繭になるから
		DEAD		//消滅	(アニメーション)使わないかも、繭になるから
	};

	Texture2D m_Texture2D;

	Collision::Sphere m_Collider; // 当たり判定の為の情報
	float m_Radius = 25.0f; // SetScale(50,50,0) なので半径 25 くらい

	float m_serchDistance = 500.0f; //敵を探す距離
	float m_deceleration  = 0.2f;	//範囲内に敵がいなくなったら減速する速度
	float m_stop_speed    = 0.001f;	//この速度以下になると停止する

	DirectX::SimpleMath::Vector3 m_wallSlideDir = DirectX::SimpleMath::Vector3::Zero;
	float m_wallSlideTimer = 0.0f; //ターゲットを見失ったときのタイマー

	Field* m_Field = nullptr;
	bool hitBorder = false;

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	//巫女を移動させるための関数
	void move();

	//糸の位置参照する関数 (hitcheck)
	//敵の位置参照する関数 (敵の位置-巫女の位置)
	//ステージの位置参照する関数　(中心点-巫女の位置)



	void SetField(Field* field) { m_Field = field; };
	void SetHitBorder(bool hit) { hitBorder = hit; };

	
};

