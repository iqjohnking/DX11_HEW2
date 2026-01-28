#pragma once
#include "Input.h"
#include "Hashira.h"
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
	
	int uid = 0;
	bool m_oldone = false;

	int m_Hitpoint   = 3;    // 体力
	bool m_IsGrowing = false; // 伸び中フラグ
	bool m_IsHashiraed = false; // 柱に当たったフラグ
	int m_PoiseLevel  = 0; // 毒状態フラグ
	int m_PoiseTimer  = 0; // 毒状態フラグ
	int hitStopTimer = 0;

	float m_ExpandSpeed = 50.0f; // 1フレームあたりの伸び量
	float m_TargetLength   = 0.0f;  // 目標位置までの最大長さ

	void UpdateCollider();        // 内部用
	void HandleCollisionAndGrowth(); // 当たり判定と伸長処理

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void Uninit() override;

	//bool HasCollider() const override { return true; }
	
	const Collision::Segment& GetSegment() const { return m_Segment; }
	//bool CheckHit(const Collision::Sphere& sphere) const;
	bool GetIsActive() const { return isActive; }

	// 手から発射するときに呼ぶ
	void Fire(const DirectX::SimpleMath::Vector3& startPos,
			  const DirectX::SimpleMath::Vector3& targetPos);
	void reInit();

	void SetStartPos(const DirectX::SimpleMath::Vector3& pos) { m_StartPos = pos; }
	DirectX::SimpleMath::Vector3 GetStartPos() const { return m_StartPos; }
	void SetEndPos(const DirectX::SimpleMath::Vector3& pos) { m_EndPos = pos; }
	DirectX::SimpleMath::Vector3 GetTargetPos() const { return m_EndPos; }

	void SetHitStopTimer(int timer) { hitStopTimer = timer; }


	void AddPoiseLevel() {
		static constexpr int kMaxPoiseLevel = 3; 
		if (m_PoiseLevel <= kMaxPoiseLevel) m_PoiseLevel++;
		m_PoiseTimer = 180;
	}
	int GetPoiseDmg() const { 
		static constexpr int kDmg[] = { 0, 2, 3, 6 };
		return kDmg[m_PoiseLevel];
	}

	bool IsGrowing() const { return m_IsGrowing; }

	void SetUID(int id) { uid = id; }
	int GetUID() const { return uid; }

	void SetOldOne(bool old) { m_oldone = old; }

};
