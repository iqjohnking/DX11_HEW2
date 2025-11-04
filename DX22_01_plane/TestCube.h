#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

//-----------------------------------------------------------------------------
//TestCubeクラス
//-----------------------------------------------------------------------------
class TestCube {
	// SRT情報（姿勢情報）
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// 頂点データ
	std::vector<VERTEX_3D> m_Vertices;

	//インデックスデータ
	std::vector<unsigned int> m_Indices;

	// 描画の為の情報（メッシュに関わる情報）
	IndexBuffer	 m_IndexBuffer; // インデックスバッファ
	VertexBuffer<VERTEX_3D>	m_VertexBuffer; // 頂点バッファ

	// 描画の為の情報（見た目に関わる部分）
	Shader m_Shader; // シェーダー
	Texture m_Texture; // テクスチャ

public:
	void Init();
	void Update();
	void Draw(Camera* cam);
	void Uninit();
};