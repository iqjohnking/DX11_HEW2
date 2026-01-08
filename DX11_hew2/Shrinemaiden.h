#pragma once
#include "Character.h"
#include "Texture2D.h"	//基底クラス
#include "DeBugLine2D.h"

struct DebugTri
{
	DirectX::SimpleMath::Vector3 a;
	DirectX::SimpleMath::Vector3 b;
	DirectX::SimpleMath::Vector3 c;
};

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

	float m_serchDistance = 100.0f; //敵を探す距離
	float m_deceleration  = 0.2f;	//範囲内に敵がいなくなったら減速する速度
	float m_stop_speed    = 0.001f;	//この速度以下になると停止する

	DirectX::SimpleMath::Vector3 m_wallSlideDir = DirectX::SimpleMath::Vector3::Zero;
	float m_wallSlideTimer = 0.0f; //ターゲットを見失ったときのタイマー

	Field* m_Field = nullptr;
	bool hitBorder = false;

	enum class EscapeState
	{
		SearchEscapePoint,
		MoveToEscapePoint,
	};

	EscapeState m_EscapeState = EscapeState::SearchEscapePoint;
	DirectX::SimpleMath::Vector3 m_EscapeTarget = DirectX::SimpleMath::Vector3::Zero;
	float m_EscapeArriveDist = 10.0f; // 近づいたら再探索
	Vector3 m_LastFailedDir = Vector3::Zero;



	std::vector<struct DebugTri> m_DebugTris;
	bool m_DrawDebugTris = true;

	void DrawDebugTriangles(Camera* cam);

	static bool IsPointInTriangleXY(const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& a,
		const DirectX::SimpleMath::Vector3& b,
		const DirectX::SimpleMath::Vector3& c);

	static float DistPointToSegmentSqXY(const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& a,
		const DirectX::SimpleMath::Vector3& b);

	static bool DoesCircleIntersectTriangleXY(const DirectX::SimpleMath::Vector3& center,
		float radius,
		const DirectX::SimpleMath::Vector3& a,
		const DirectX::SimpleMath::Vector3& b,
		const DirectX::SimpleMath::Vector3& c);



public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	//巫女を移動させるための関数
	void move();

	void SetField(Field* field) { m_Field = field; };
	void SetHitBorder(bool hit) { hitBorder = hit; };
	void SetSerchDistance (float dist) { m_serchDistance = dist;};	
	void OnEscapeRouteFailed(const Vector3& now_pos);

};

