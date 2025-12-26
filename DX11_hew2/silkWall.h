#pragma once
#include "Input.h"
#include "Texture2D.h"
#include "Collision.h" 

//-----------------------------------------------------------------------------
// silkWall（蜘蛛糸の壁）
// Texture2D を見た目に使いつつ、当たり判定は Segment（線分）で持つ
//-----------------------------------------------------------------------------
class silkWall : public Object
{
protected:
	Texture2D m_Texture2D;

	Collision::Segment m_Segment; // 当たり判定

	DirectX::SimpleMath::Vector3 m_StartPos;
	DirectX::SimpleMath::Vector3 m_EndPos;

	int m_Hitpoint   = 3;    // 体力
	bool m_IsGrowing = false; // 伸び中フラグ
	bool m_IsPoised  = false; // 毒状態フラグ

	float m_ExpandSpeed = 50.0f; // 1フレームあたりの伸び量
	float m_TargetLength   = 0.0f;  // 目標位置までの最大長さ

	void UpdateCollider();        // 内部用

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	bool HasCollider() const override { return true; }
	
	const Collision::Segment& GetSegment() const { return m_Segment; }
	bool CheckHit(const Collision::Sphere& sphere) const;

	// 手から発射するときに呼ぶ
	void Fire(const DirectX::SimpleMath::Vector3& startPos,
			  const DirectX::SimpleMath::Vector3& targetPos);

	void SetStartPos(const DirectX::SimpleMath::Vector3& pos) { m_StartPos = pos; }
	DirectX::SimpleMath::Vector3 GetStartPos() const { return m_StartPos; }
	void SetEndPos(const DirectX::SimpleMath::Vector3& pos) { m_EndPos = pos; }
	DirectX::SimpleMath::Vector3 GetTargetPos() const { return m_EndPos; }


	bool IsGrowing() const { return m_IsGrowing; }
};
