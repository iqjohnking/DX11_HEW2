#pragma once
#include "Texture2D.h"

using namespace DirectX::SimpleMath;

//-----------------------------------------------------------------------------
// Texture2Dクラス
//-----------------------------------------------------------------------------
class Field : public Texture2D
{
public:
	void Init();
	void Update();
	void Draw(Camera* cam) override;
	void DrawDebug(Camera* cam);

	// 16角形
	void BuildBorder(const DirectX::SimpleMath::Vector3& center, float radius, int edges = 16);

	// 反射処理：pos/velocity
	bool ResolveBorder(DirectX::SimpleMath::Vector3& pos,
		DirectX::SimpleMath::Vector3& vel,
		float objRadius) const;



private:
	// 境界を構成する 1 辺分のデータ
	struct Edge
	{
		DirectX::SimpleMath::Vector3 p0; // 辺の始点
		DirectX::SimpleMath::Vector3 p1; // 辺の終点
		DirectX::SimpleMath::Vector3 n;  // 場地内側を向いた単位法線
	};

	std::vector<Edge> m_Edges;          // 境界ポリゴンの全辺

	Vector3 m_Center { 0.0f, 0.0f, 0.0f }; // 場地中心
	float m_Radius = 500.0f;             // 場地半径
	int   m_EdgeCount = 16;               // 辺の数

	// Vector3 用の簡易内積（Dot）
	//static float Dot(const DirectX::SimpleMath::Vector3& v1,const DirectX::SimpleMath::Vector3& v2);

};
