#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Collision.h"
#include "ObjectType.h"
#include <SimpleMath.h>

class Object {
protected:
	// 基本情報
	float const deltaTime = 1 / 60;  // 仮の固定フレームレート

	ObjectType m_Type = ObjectType::NONE; // オブジェクトの種類 //今は使ってない
	bool isActive = true;            // 活性状態かどうか

	// SRT情報（姿勢情報）
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	//Collision::Sphere m_Sphere;; // 当たり判定の為の情報//すべてのオブジェクトに当たり判定情報を持たせるのは無駄が多いので、派生クラスで持つようにする

	// 描画の為の情報（見た目に関わる部分）
	Shader m_Shader; // シェーダー

public:

	virtual ~Object() {} //仮デストラクタ（※派生クラスのリソース解放のために必要）

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw(Camera* cam) = 0;
	virtual void Uninit() = 0;

	virtual bool HasCollider() const { return false; }

	// getter setter
	ObjectType GetType() const { return m_Type; }
	void SetType(ObjectType type) { m_Type = type; }
	bool IsActive() const { return isActive; }
	void SetIsActive(bool isAct) { isActive = isAct; }

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_Position; }
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_Position = position; }

	DirectX::SimpleMath::Vector3 GetRotation() const { return m_Rotation; }
	void SetRotation(DirectX::SimpleMath::Vector3 rotation) { m_Rotation = rotation; }

	DirectX::SimpleMath::Vector3 GetScale() const { return m_Scale; }
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_Scale = scale; }
	
};