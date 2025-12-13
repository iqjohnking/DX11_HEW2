#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Collision.h"

class Object {
protected:
	// SRT情報（姿勢情報）
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// 描画の為の情報（見た目に関わる部分）
	Shader m_Shader; // シェーダー

public:

	virtual ~Object() {} //仮デストラクタ（※派生クラスのリソース解放のために必要）

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw(Camera* cam) = 0;
	virtual void Uninit() = 0;

	virtual bool HasCollider() const { return false; }


	// 位置の取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_Position; }
	DirectX::SimpleMath::Vector3 GetRotation() const { return m_Rotation; }

	//位置の設定
	void SetPosition(const DirectX::SimpleMath::Vector3& p) { m_Position = p; }

};