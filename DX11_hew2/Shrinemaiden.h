#pragma once

#include "Character.h"
#include "Texture2D.h"
#include "EnemyBase.h"
#include "silkWall.h"
#include "DeBugLine2D.h"

#include <vector>

struct DebugTri
{
	DirectX::SimpleMath::Vector3 a;
	DirectX::SimpleMath::Vector3 b;
	DirectX::SimpleMath::Vector3 c;
};

class Field; // forward

class Shrinemaiden : public Character
{
protected:
	//==================================================
	// States
	//==================================================
	enum mikoState
	{
		SPAWNING,	// 0: 出現中(アニメーション)
		ALIVE,		// 1: 生存
		ISMAYUING,	// 2: 繭になっている最中
		DYING,		// 3: 消滅中(ダメージ受けるもこれ)(アニメーション)
		DEAD		// 4: 消滅
	};

	enum class EscapeState
	{
		SearchEscapePoint,
		MoveToEscapePoint,
	};

	mikoState   m_MoveState   = mikoState::SPAWNING;
	EscapeState m_EscapeState = EscapeState::SearchEscapePoint;

	//==================================================
	// Render / Collision
	//==================================================
	Texture2D m_Texture2D;

	Collision::Sphere m_Collider;
	float m_Radius = 25.0f; // SetScale(50,50,0) => 半径 25 くらい

	//==================================================
	// World refs
	//==================================================
	Field* m_Field = nullptr;
	bool   hitBorder = false;

	//==================================================
	// Movement / Search
	//==================================================
	float m_serchDistance = 100.0f; // 敵を探す距離

	DirectX::SimpleMath::Vector3 m_wallSlideDir = DirectX::SimpleMath::Vector3::Zero;

	// Escape target
	DirectX::SimpleMath::Vector3 m_EscapeTarget = DirectX::SimpleMath::Vector3::Zero;
	float m_EscapeArriveDist = 10.0f; // 近づいたら再探索

	// Stuck / retry
	int   m_RetreatCooldown = 0; // 退路再探索クールタイム
	int   m_StuckTimer = 0;
	bool  m_IsStuck = false;
	DirectX::SimpleMath::Vector3 m_LastFailedDir = DirectX::SimpleMath::Vector3::Zero;

	//==================================================
	// Timers / Animation control
	//==================================================
	int m_SPAWNINGTimer = 15; // 出現アニメーション時間

	DirectX::SimpleMath::Vector3 m_StartMayuPos  = DirectX::SimpleMath::Vector3::Zero; // 起
	DirectX::SimpleMath::Vector3 m_TargetMayuPos = DirectX::SimpleMath::Vector3::Zero; // 迄

	int kMayuFrames = 15; // 繭になるまでのフレーム数
	int   m_MAYUINGTimer = 15; // 繭になる時間
	int   m_DYINGTimer   = 180; // 消滅アニメーション時間

	// Muteki
	int m_MutekiTimer = 0; // 無敵時間
	static constexpr int kMutekiFrames = 60; // 例：60f = 1秒
	void StartMuteki(int frames = kMutekiFrames)
	{
		m_MutekiTimer = frames;
		m_Texture2D.PlayAnim("getH");
	}

	bool m_IgnoreEnemyInSearch = false;

	//==================================================
	// Debug
	//==================================================
	std::vector<DebugTri> m_DebugTris;
	bool m_DrawDebugTris = true;

	void DrawDebugTriangles(Camera* cam);

public:
	//==================================================
	// Lifecycle
	//==================================================
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	//==================================================
	// Control
	//==================================================
	void move();

	void SetField(Field* field) { m_Field = field; }
	void SetHitBorder(bool hit) { hitBorder = hit; }
	void SetSerchDistance(float dist) { m_serchDistance = dist; }

	// Muteki
	int  GetMutekiTimer() const { return m_MutekiTimer; }
	bool IsMuteki() const { return m_MutekiTimer > 0; }

	// Mayu
	void SetStartMayuing(const DirectX::SimpleMath::Vector3& mayuPos)
	{
		m_Texture2D.PlayAnim("getH");
		m_StartMayuPos = GetPosition();
		m_TargetMayuPos = mayuPos;
		m_MoveState = mikoState::ISMAYUING;
	}

	int GetDYINGTimer() const { return m_DYINGTimer; }

private:
	//==================================================
	// Geometry helpers (XY)
	//==================================================
	static bool IsPointInTriangleXY(
		const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& a,
		const DirectX::SimpleMath::Vector3& b,
		const DirectX::SimpleMath::Vector3& c);

	static float DistPointToSegmentSqXY(
		const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& a,
		const DirectX::SimpleMath::Vector3& b);

	static bool DoesCircleIntersectTriangleXY(
		const DirectX::SimpleMath::Vector3& center,
		float radius,
		const DirectX::SimpleMath::Vector3& a,
		const DirectX::SimpleMath::Vector3& b,
		const DirectX::SimpleMath::Vector3& c);

	//==================================================
	// Move helpers
	//==================================================
	bool TryStuckNudge(const DirectX::SimpleMath::Vector3& now_pos);

	bool SearchEscapeTarget(
		const DirectX::SimpleMath::Vector3& now_pos,
		const std::vector<EnemyBase*>& enemies,
		const std::vector<silkWall*>& silkWalls);

	bool UpdateEscapeMove(
		const DirectX::SimpleMath::Vector3& now_pos,
		DirectX::SimpleMath::Vector3& outVel);

	bool ApplyField(
		const DirectX::SimpleMath::Vector3& now_pos,
		const DirectX::SimpleMath::Vector3& vel,
		DirectX::SimpleMath::Vector3& outNextPos);

	bool CheckSilkCollisionAt(
		const DirectX::SimpleMath::Vector3& pos,
		const std::vector<silkWall*>& silkWalls) const;

	void OnEscapeRouteFailed(
		const DirectX::SimpleMath::Vector3& now_pos,
		const std::vector<silkWall*>& silkWalls,
		const std::vector<EnemyBase*>& enemies);
};
