#pragma once
#include "Object.h"


class Character :public Object {
protected:
	int m_Hitpoint = 100; // キャラクターの体力

	float m_minSpeed = 0.0f; // 最小移動速度
	float m_TargetSpeed = 1.0f; // キャラクターの目標速度
	float m_maxSpeed = 10.0f; // 最大移動速度

	float m_acceleration = 1.0f; // キャラクターの加速度
	float m_velocity = 0.0f; // キャラクターの現在の速度

	DirectX::SimpleMath::Vector3 m_direction = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); // キャラクターの移動方向（角度）

	bool IsAlive = true; // キャラクターが生存しているかどうか
public:
	virtual ~Character() {} // 仮デストラクタ
	virtual void Init() override = 0;
	virtual void Update() override = 0;
	virtual void Draw(Camera* cam) override = 0;
	virtual void Uninit() override = 0;

	int GetHitpoint() const { return m_Hitpoint; }				// キャラクターの体力を取得
	void SetHitpoint(int hitpoint) { m_Hitpoint = hitpoint; }	// キャラクターの体力を設定
	float GetMinSpeed() const { return m_minSpeed; }			// キャラクターの最小移動速度を取得
	void SetMinSpeed(float speed) { m_minSpeed = speed; }		// キャラクターの最小移動速度を設定
	float GetTargetSpeed() const { return m_TargetSpeed; }		// キャラクターの移動速度を取得
	void SetTargetSpeed(float speed) { m_TargetSpeed = speed; }	// キャラクターの移動速度を設定
	float GetMaxSpeed() const { return m_maxSpeed; }			// キャラクターの最大移動速度を取得
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }		// キャラクターの最大移動速度を設定

	float GetAcceleration() const { return m_acceleration; }	// キャラクターの加速度を取得
	void SetAcceleration(float acceleration) { m_acceleration = acceleration; } // キャラクターの加速度を設定
	float GetVelocity() const { return m_velocity; }			// キャラクターの現在の速度を取得
	void SetVelocity(float velocity) { m_velocity = velocity; } // キャラクターの現在の速度を設定

	DirectX::SimpleMath::Vector3 GetDirection() const { return m_direction; }// キャラクターの移動方向を取得
	void SetDirection(DirectX::SimpleMath::Vector3 direction) { m_direction = direction; }// キャラクターの移動方向を設定

	bool GetIsAlive() const { return IsAlive; }// キャラクターが生存しているかどうかを取得
	void SetIsAlive(bool isAlive) { IsAlive = isAlive; }// キャラクターの生存状態を設定
};