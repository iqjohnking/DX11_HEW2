#pragma once
#include "Object.h"
#include "Texture2D.h"

using namespace DirectX::SimpleMath;

//-----------------------------------------------------------------------------
// Texture2Dクラス
//-----------------------------------------------------------------------------
class Field : public Object
{
protected:
	// フィールド描画用テクスチャ
	Texture2D m_Texture2D;

	// 境界を構成する 1 辺分のデータ
	struct Edge
	{
		DirectX::SimpleMath::Vector3 p0;	// 辺の始点
		DirectX::SimpleMath::Vector3 p1;	// 辺の終点
		DirectX::SimpleMath::Vector3 n;		// 場地内側を向いた単位法線
	};

	std::vector<Edge> m_Edges;				// 境界ポリゴンの全辺

	Vector3 m_Center { 0.0f, 0.0f, 0.0f };	// 場地中心
	float m_Radius = 500.0f;				// 場地半径
	int   m_EdgeCount = 16;					// 辺の数

public:
	void Init() override;
	void Update() override;
	void Draw(Camera* cam) override;
	void DrawDebug(Camera* cam);
	void Uninit()override;

	// 16角形
	void BuildBorder(const DirectX::SimpleMath::Vector3& center, float radius, int edges = 16);

	// 反射処理：pos/velocity
	bool ResolveBorder(DirectX::SimpleMath::Vector3& pos,
		DirectX::SimpleMath::Vector3& vel,
		float objRadius) const;

	Vector3 GetCenter() const { return m_Center; }
	float GetRadius() const { return m_Radius; }

	// --- 巫女AIなどから境界辺を参照する ---
	const std::vector<Edge>& GetEdges() const { return m_Edges; }
	int GetEdgeCount() const { return (int)m_Edges.size(); }


};
