#pragma once
#include "Input.h"
#include "Texture2D.h"
#include "Collision.h" 

//-----------------------------------------------------------------------------
// silkWall（蜘蛛糸の壁）
// Texture2D を見た目に使いつつ、当たり判定は Segment（線分）で持つ
//-----------------------------------------------------------------------------
class silkWall : public Texture2D
{
private:
	bool  m_IsActive    = false; // 有効フラグ 
	bool  m_IsGrowing   = false; // 伸び中フラグ
	float m_ExpandSpeed = 10.0f; // 1フレームあたりの伸び量
	float m_MaxLength   = 0.0f;  // 目標位置までの最大長さ

	DirectX::SimpleMath::Vector3 m_StartPos;
	DirectX::SimpleMath::Vector3 m_TargetPos;

	Collision::Segment m_Segment; // 当たり判定

	void UpdateCollider();        // 内部用

public:
	void Init();
	void Update();

	bool HasCollider() const override { return true; }
	
	const Collision::Segment& GetSegment() const { return m_Segment; }
	bool CheckHit(const Collision::Sphere& sphere) const;

	// 手から発射するときに呼ぶ
	void Fire(const DirectX::SimpleMath::Vector3& startPos,
			  const DirectX::SimpleMath::Vector3& targetPos);

	void SetStartPos(const DirectX::SimpleMath::Vector3& pos) { m_StartPos = pos; }
	DirectX::SimpleMath::Vector3 GetStartPos() const { return m_StartPos; }
	void SetTargetPos(const DirectX::SimpleMath::Vector3& pos) { m_TargetPos = pos; }
	DirectX::SimpleMath::Vector3 GetTargetPos() const { return m_TargetPos; }

	bool IsActive() const { return m_IsActive; }
	bool IsGrowing() const { return m_IsGrowing; }
};
